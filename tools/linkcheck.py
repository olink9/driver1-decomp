#!/usr/bin/env python3
"""Link check: relink SLUS_008.42 with matched C functions in place of their asm.

    tools/linkcheck.py                      # every src/pilot/*.c (not .wip.c/.best.c)
    tools/linkcheck.py SYM...               # only these (src/pilot/SYM.c)
    tools/linkcheck.py --wip SYM...         # also accept src/pilot/SYM.wip.c for these
    tools/linkcheck.py --all --wip SYM...   # the default set plus these
    [-o pilot/linkcheck.tsv] [-j 8]

How it works (all output under build/linkcheck/, nothing else is written):
  * The committed baseline asm (asm/2C8C.s, asm/data/*.s, asm/header.s) - the same
    files `make` assembles - is cut into one object per `nonmatching` block: one per
    function in .text, one per data label in .rodata/.data. A linker script is
    generated from slus_008.42.ld with each `build/asm/X.o(.sec)` line expanded into
    those block objects in order (text blocks laid out back to back as in the original;
    data blocks pinned to their original address). It is linked with the same
    undefined_*_auto.txt scripts, and this baseline must give the original sha1.
  * Each C file is compiled with tools/fnscore.py's pipeline (cpp, cc1 2.7.2-cdk -O2
    -G8, maspsx, as). Its .text replaces the function's asm block in the link. Its own
    .rodata/.sdata/.data (string literals, jump tables, initialised local aggregates)
    is placed at the original address, found by pairing the C object's %lo/%gp_rel
    relocations against its sections with the asm's relocations at the same
    instruction; the original data labels that range covers are dropped. If .rodata
    pairs to several bases (its strings/jump tables live apart in the original), the
    maspsx output is reassembled with .rodata cut at its labels: first one section per
    label to pair each one alone, then one section .rodata.lc<k> per run of labels that
    sit back to back in the original (C alignment), each placed at its own address and
    re-paired to exactly one base (the unchanged reassembly must reproduce the fnscore
    object first, and the .text must keep its size). Its .bss/.sbss (tentative
    definitions of variables in the original bss, kept in C because being .comm changes
    how maspsx addresses them) has no bytes: each label in it must be a symbol of the
    original (its address from the baseline link), and all labels of a section must give
    one base (address - offset); otherwise the maspsx output is reassembled the same way
    with each bss label in its own section .bss.lc<i>/.sbss.lc<i>. Each bss piece gets a
    NOLOAD output section at that address (SUBALIGN(1): the original address wins over
    C's alignment), at or above main_BSS_START, and must land there after the link;
    the exe byte compare is what proves the relocations against it. A global the C
    object defines in bss is removed from the undefined_*_auto.txt copies used for that
    link, so the C definition is the only one. Every symbol
    defined only in a dropped block gets PROVIDE(sym = original address), so remaining
    asm still links, but all relocations of the C object resolve through the linker.
    Merged string blocks are cut at line granularity when a C piece covers only part.
    The five j/%hi/%lo references from one function to a plain .L label inside another
    get a `.global` on that label in the block copy (no byte changes).
  * A C .text shorter than its asm block passes only if the remainder is zero padding
    (inter-object alignment), which is then kept. Two C files carrying an identical
    copy of the same table at the same address (a table shared in the original) get
    one loaded copy; the other is linked NOLOAD at the same address.
  * One link per function (only that function replaced): PASS when the C .text is the
    original size and the final exe's bytes over the function's range (and over any
    data the C object placed) equal orig/SLUS_008.42. Then one combined link with every
    passing function at once, compared by sha1.
Run inside distrobox `build`.
"""
import argparse, concurrent.futures as cf, glob, hashlib, os, re, struct, subprocess, sys, threading, types

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
import fnscore  # noqa: E402
from elftools.elf.elffile import ELFFile  # noqa: E402

LC = os.path.join(ROOT, "build", "linkcheck")
BASE = os.path.join(LC, "base")
ORIG = os.path.join(ROOT, "orig", "SLUS_008.42")
LDSCRIPT = os.path.join(ROOT, "slus_008.42.ld")
SHA1 = "94b44a84de2f9fa2dcaf120db2e7b3d74bc70d3d"
AS = fnscore.AS
LD = "mips-linux-gnu-ld"
OBJCOPY = "mips-linux-gnu-objcopy"
VRAM, FILEOFF = 0x10000, 0x800
# asm file -> build/asm object name used in slus_008.42.ld
ASMFILES = {"asm/2C8C.s": "build/asm/2C8C.o", "asm/data/800.rodata.s": "build/asm/data/800.rodata.o",
            "asm/data/75B14.data.s": "build/asm/data/75B14.data.o", "asm/data/87800.bss.s": "build/asm/data/87800.bss.o",
            "asm/header.s": "build/asm/header.o"}
ADDR_RE = re.compile(r"/\*\s+[0-9A-F]+\s+([0-9A-F]{8})\b")


def sh(cmd, **kw):
    return subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True, **kw)


def write_if_changed(path, text):
    if os.path.exists(path) and open(path).read() == text:
        return
    open(path, "w").write(text)


# ---------------------------------------------------------------- baseline blocks
class Block:
    def __init__(self, src, idx, name, lines):
        self.src, self.idx, self.name, self.lines = src, idx, name, lines
        m = next((ADDR_RE.search(l) for l in lines if ADDR_RE.search(l)), None)
        self.addr = int(m.group(1), 16) if m else None
        tag = os.path.basename(src).replace(".", "_")
        self.s = os.path.join(BASE, "blk", f"{tag}_{idx:04}_{name}.s")
        self.o = self.s[:-2] + ".o"
        self.size = 0
        self.syms = []


def split_file(src):
    """Cut an asm file into (section name, [Block], header) at every `nonmatching` line
    (a directly preceding `.align` belongs to the block it aligns)."""
    lines = open(os.path.join(ROOT, src)).read().split("\n")
    starts, open_ = [], set()
    for i, l in enumerate(lines):
        w = l.split()
        if len(w) >= 2 and w[0] in ("glabel", "dlabel"):
            open_.add(w[1].rstrip(","))
        elif len(w) >= 2 and w[0] in ("endlabel", "enddlabel"):
            open_.discard(w[1].rstrip(","))
        if l.startswith("nonmatching ") and not open_:   # never cut inside an open label
            # nor at an address that isn't 4-aligned: gas aligns .word relative to the
            # object start, so such a piece can't be assembled on its own
            m = next((ADDR_RE.search(x) for x in lines[i:i + 40] if ADDR_RE.search(x)), None)
            if starts and m and int(m.group(1), 16) % 4:
                continue
            j = i
            while j > 0 and (lines[j - 1].strip().startswith(".align") or not lines[j - 1].strip()):
                j -= 1
            starts.append((j, l.split()[1].rstrip(",")))
    sec = next((l for l in lines if l.startswith(".section")), ".section .text")
    secname = sec.split()[1].rstrip(",")
    if not starts:
        return secname, [Block(src, 0, "all", lines)], ""
    head = [l for l in lines[:starts[0][0]] if l.startswith((".include", ".set", ".section"))]
    header = "\n".join(head) + "\n"
    # a few branches/jumps (j, %hi/%lo) go to a plain .L label of another function:
    # make exactly those labels global so the pieces still link (bytes unchanged)
    defs, refs = {}, set()
    for i, l in enumerate(lines):
        m = re.match(r"\s*(\.L[0-9A-Fa-f_]+):", l)
        if m:
            defs[m.group(1)] = i
        elif "*/" in l:
            refs |= {(x, i) for x in re.findall(r"\.L[0-9A-Fa-f_]+", l.split("*/", 1)[1])}
    piece = lambda i: max(k for k, (j, _) in enumerate(starts) if j <= i) if i >= starts[0][0] else -1
    glob_ = {x for x, i in refs if x in defs and piece(defs[x]) != piece(i)}
    blocks = []
    for k, (j, name) in enumerate(starts):
        end = starts[k + 1][0] if k + 1 < len(starts) else len(lines)
        bl = []
        for l in lines[j:end]:
            m = re.match(r"\s*(\.L[0-9A-Fa-f_]+):", l)
            if m and m.group(1) in glob_:
                bl.append(f".global {m.group(1)}")
            bl.append(l)
        blocks.append(Block(src, k, name, bl))
    return secname, blocks, header


def assemble(s, o):
    if os.path.exists(o) and os.path.getmtime(o) >= os.path.getmtime(s):
        return None
    r = sh(AS + ["-o", o, s])
    return None if r.returncode == 0 else f"{s}: {r.stderr[-800:]}"


def elf_info(path):
    """sections {name: size}, defined symbol names, relocs of .text."""
    with open(path, "rb") as f:
        e = ELFFile(f)
        secs = {s.name: s["sh_size"] for s in e.iter_sections()}
        st = e.get_section_by_name(".symtab")
        syms = [x.name for x in st.iter_symbols()
                if x.name and isinstance(x["st_shndx"], int) and x["st_info"]["type"] != "STT_SECTION"
                and x["st_info"]["bind"] == "STB_GLOBAL"] if st else []
    return secs, syms


def prepare(jobs):
    os.makedirs(os.path.join(BASE, "blk"), exist_ok=True)
    files = {}
    allblocks = []
    for src in ASMFILES:
        secname, blocks, header = split_file(src)
        files[src] = (secname, blocks)
        for b in blocks:
            b.header = header
            if src == "asm/header.s" or len(blocks) == 1 and b.name == "all":
                write_if_changed(b.s, "\n".join(b.lines) + "\n")
            else:
                write_if_changed(b.s, header + "\n".join(b.lines) + "\n")
            allblocks.append(b)
    with cf.ThreadPoolExecutor(jobs) as ex:
        errs = [e for e in ex.map(lambda b: assemble(b.s, b.o), allblocks) if e]
    if errs:
        sys.exit("baseline asm failed:\n" + "\n".join(errs[:5]))
    for b in allblocks:
        secs, b.syms = elf_info(b.o)
        b.size = sum(secs.get(n, 0) for n in (".text", ".data", ".rodata"))
    return files


# ---------------------------------------------------------------- linker script
_sub_lock = threading.Lock()


def line_addrs(b):
    return [(int(m.group(1), 16) if m else None) for m in (ADDR_RE.search(l) for l in b.lines)]


def sub_piece(b, lo, hi):
    """Object for lines [lo, hi) of data block b (assembled on demand). `enddlabel`
    of a label that starts in another piece is dropped (it is only .size metadata)."""
    s = f"{b.s[:-2]}__{lo}_{hi}.s"
    o = s[:-2] + ".o"
    with _sub_lock:
        if not os.path.exists(o):
            ls = b.lines[lo:hi]
            opened = {l.split()[1] for l in ls if l.split()[:1] in (["dlabel"], ["glabel"])}
            ls = [l for l in ls if not (l.split()[:1] in (["enddlabel"], ["endlabel"]) and l.split()[1] not in opened)]
            write_if_changed(s, b.header + "\n".join(ls) + "\n")
            err = assemble(s, o)
            if err:
                raise LinkErr("data split: " + err[-300:])
    return o


def data_layout(files, pieces):
    """Lay out the data files with the C objects' data pieces at their addresses.
    Returns ({src: [(addr, input-section text)]}, [(addr, input-section text)] for
    duplicate pieces, set of touched blocks)."""
    pieces = [p for p in pieces if p[4] is not None]        # bss: see bss_pieces
    layout, dups, touched = {}, [], set()
    placed = []                                              # (addr, end, bytes)
    for src, (secname, blocks) in files.items():
        if src == "asm/2C8C.s" or blocks[0].addr is None:
            continue
        lo, hi = blocks[0].addr, blocks[-1].addr + blocks[-1].size
        mine = sorted(p for p in pieces if lo <= p[0] < hi)
        cuts = []
        for a, n, o, sec, data in mine:
            dup = next((x for x in placed if x[0] < a + n and a < x[1]), None)
            if dup:
                if dup[0] == a and dup[2] == data:
                    dups.append((a, f"{o}({sec});"))       # identical shared table: one copy
                    continue
                raise LinkErr(f"C {sec} at 0x{a:X} overlaps another C object's data at 0x{dup[0]:X}")
            placed.append((a, a + n, data))
            cuts.append((a, a + n, f"{o}({sec});"))
        items = [(a, txt) for a, _, txt in cuts]
        for b in blocks:
            bend = b.addr + b.size
            mycuts = [(a, e) for a, e, _ in cuts if a < bend and b.addr < e]
            if not mycuts:
                items.append((b.addr, f"{b.o}({secname});"))
                continue
            touched.add((src, b.idx))
            la = line_addrs(b)
            first = next(i for i, x in enumerate(la) if x is not None)
            keep, pos = [], b.addr                           # kept [start, end) address ranges
            for a, e in sorted(mycuts):
                if a > pos:
                    keep.append((pos, a))
                pos = max(pos, e)
            if pos < bend:
                keep.append((pos, bend))
            for ks, ke in keep:
                # start: block start, or the first data line at/after ks (C data ends
                # on padding); end: the line at exactly ke (a C piece must start on a line)
                if ks == b.addr:
                    i0 = 0
                else:
                    i0 = next((i for i, x in enumerate(la) if x is not None and x >= ks), None)
                    if i0 is None or la[i0] >= ke:
                        continue                              # only padding left
                    if la[i0] % 4:
                        raise LinkErr(f"data after a C piece resumes at unaligned 0x{la[i0]:X}")
                    ks = la[i0]
                if ke == bend:
                    i1 = len(b.lines)
                else:
                    i1 = next((i for i, x in enumerate(la) if x == ke), None)
                    if i1 is None:
                        raise LinkErr(f"C data at 0x{ke:X} starts inside a data line of {b.name}")
                if i0 == 0 and i1 == len(b.lines):
                    items.append((ks, f"{b.o}({secname});"))
                else:
                    items.append((ks, f"{sub_piece(b, i0, i1)}({secname});"))
        layout[src] = (sorted(items), hi)
    for a, n, o, sec, data in pieces:
        if not any(x[0] <= a < x[1] for x in placed) and not any(d[0] == a for d in dups):
            raise LinkErr(f"C data at 0x{a:X} is outside every data file")
    return layout, dups, touched


UNDEF = ["undefined_syms_auto.txt", "undefined_syms_manual.txt", "undefined_funcs_auto.txt"]


def bss_pieces(pieces):
    """The bss pieces, in the order gen_script gives them output sections .lcbss<k>.
    Overlapping ones must be the same variable (same address and size) of two C files."""
    bss = sorted((p for p in pieces if p[4] is None), key=lambda p: (p[0], p[1], p[2], p[3]))
    for x, y in zip(bss, bss[1:]):
        if y[0] < x[0] + x[1] and (x[0], x[1]) != (y[0], y[1]):
            raise LinkErr(f"C {y[3]} at 0x{y[0]:X} overlaps another C object's {x[3]} at 0x{x[0]:X}")
    return bss


def gen_script(files, repl, pieces, provides, tag, pads=None):
    """repl: {text block name: C object}; pieces: [(addr, size, obj, section, bytes or
    None for bss)]; provides: {sym: addr}; pads: {text block name: zero bytes after the
    C .text}. Returns (path of the generated linker script, symbol scripts to link with)."""
    layout, dups, _ = data_layout(files, pieces)
    bss = bss_pieces(pieces)
    pads = pads or {}
    out = []
    for line in open(LDSCRIPT).read().split("\n"):
        m = re.match(r"(\s*)(build/asm/\S+\.o)\((\.\w+)\);$", line)
        if not m:
            if line.strip() == "/DISCARD/ :":
                for k, (a, txt) in enumerate(dups):
                    out.append(f"    .lcdup{k} 0x{a:X} (NOLOAD) : {{ {txt} }}")
                # bss has no bytes in the exe; SUBALIGN(1): the original address wins
                # over C's alignment of the variable (the exe compare proves the relocs)
                for k, (a, _, o, sec, _) in enumerate(bss):
                    out.append(f"    .lcbss{k} 0x{a:X} (NOLOAD) : SUBALIGN(1) {{ {o}({sec}); }}")
            out.append(line)
            continue
        ind, objname, sec = m.groups()
        src = next(s for s, o in ASMFILES.items() if o == objname)
        secname, blocks = files[src]
        if secname != sec:
            continue                                   # e.g. 800.rodata.o(.bss): empty
        if src == "asm/2C8C.s":
            for b in blocks:
                if b.name in repl:
                    out.append(f"{ind}{repl[b.name]}(.text);")
                    if pads.get(b.name):
                        out.append(f"{ind}. += 0x{pads[b.name]:X};")
                else:
                    out.append(f"{ind}{b.o}({sec});")
            continue
        if src not in layout:
            out.append(f"{ind}{blocks[0].o}({sec});")
            continue
        items, hi = layout[src]
        for a, txt in items:
            out.append(f"{ind}. = 0x{a - VRAM:X};")
            out.append(ind + txt)
        out.append(f"{ind}. = 0x{hi - VRAM:X};")
    prov = "".join(f"PROVIDE({s} = 0x{a:X});\n" for s, a in sorted(provides.items()))
    text = prov + "\n".join(out) + "\n"
    path = os.path.join(LC, "ld", tag + ".ld")
    os.makedirs(os.path.dirname(path), exist_ok=True)
    open(path, "w").write(text)
    # a global the C object defines in bss replaces the symbol script's `sym = addr;`
    # (an assignment there would silently win over the C definition)
    drop = {n for _, _, o, sec, _ in bss for _, n, g in bss_labels(o).get(sec, []) if g}
    undef = []
    for u in UNDEF:
        if not drop:
            undef.append(u)
            continue
        keep = [l for l in open(os.path.join(ROOT, u)).read().split("\n")
                if not (re.match(r"\s*(\w+)\s*=", l) and re.match(r"\s*(\w+)\s*=", l).group(1) in drop)]
        undef.append(os.path.join(LC, "ld", f"{tag}.{u}"))
        open(undef[-1], "w").write("\n".join(keep))
    return path, undef


class LinkErr(Exception):
    pass


def link(gen, tag, pieces=()):
    """Link the (script, symbol scripts) from gen_script; the bss pieces must land at
    their addresses."""
    script, undef = gen
    elf = os.path.join(LC, "out", tag + ".elf")
    exe = elf[:-4] + ".exe"
    os.makedirs(os.path.dirname(elf), exist_ok=True)
    r = sh([LD, "-EL"] + [x for u in undef for x in ("-T", u)] + ["-T", script,
            "-Map", elf[:-4] + ".map", "--no-check-sections", "-o", elf])
    if r.returncode:
        raise LinkErr("ld: " + " | ".join(l for l in r.stderr.strip().split("\n")[:4]))
    with open(elf, "rb") as f:
        e = ELFFile(f)
        for k, (a, n, _, sec, _) in enumerate(bss_pieces(pieces)):
            x = e.get_section_by_name(f".lcbss{k}")
            if x is None or x["sh_addr"] != a or x["sh_size"] != n:
                raise LinkErr(f"C {sec} did not land at 0x{a:X} (0x{n:X} bytes)")
    r = sh([OBJCOPY, "-O", "binary", elf, exe])
    if r.returncode:
        raise LinkErr("objcopy: " + r.stderr[-300:])
    return elf, exe


def symtab(elf):
    with open(elf, "rb") as f:
        st = ELFFile(f).get_section_by_name(".symtab")
        return {x.name: x["st_value"] for x in st.iter_symbols() if x.name and x["st_info"]["type"] != "STT_SECTION"}


# ---------------------------------------------------------------- C objects
def text_relocs(path):
    """{offset: (type, symbol or section name, is_section)} for .text, plus .text bytes."""
    out = {}
    with open(path, "rb") as f:
        e = ELFFile(f)
        text = e.get_section_by_name(".text").data()
        rel = e.get_section_by_name(".rel.text")
        if rel:
            st = e.get_section(rel["sh_link"])
            for r in rel.iter_relocations():
                s = st.get_symbol(r["r_info_sym"])
                if s["st_info"]["type"] == "STT_SECTION":
                    out[r["r_offset"]] = (r["r_info_type"], e.get_section(s["st_shndx"]).name, True)
                else:
                    out[r["r_offset"]] = (r["r_info_type"], s.name, False)
    return out, text


def imm16(text, off):
    v = struct.unpack_from("<I", text, off)[0] & 0xFFFF
    return v - 0x10000 if v & 0x8000 else v


DATASECS = (".rodata", ".sdata", ".data")


def pair_bases(cobj, block, addrs, names):
    """{section: set of original addresses its offset 0 maps to}, from pairing the C
    object's %lo/%gp_rel relocations against `names` with the asm block's at the same
    instruction."""
    cr, ct = text_relocs(cobj)
    tr, tt = text_relocs(block.o)
    bases = {s: set() for s in names}
    for off, (typ, name, issec) in cr.items():
        if not issec or name not in bases or typ not in (6, 7):     # R_MIPS_LO16, R_MIPS_GPREL16
            continue
        t = tr.get(off)
        if not t or t[1] not in addrs or off + 4 > len(tt):
            continue
        bases[name].add(addrs[t[1]] + imm16(tt, off) - imm16(ct, off))
    return bases


BSSSECS = (".bss", ".sbss")
RAM_END = 0x200000                                  # 2 MB main RAM (addresses as linked)


def is_bss(name):
    return name in BSSSECS or name.startswith((".bss.lc", ".sbss.lc"))


def bss_labels(cobj):
    """{bss section: [(offset, symbol, is_global)]} for the C object's .bss/.sbss
    (and .bss.lc<k>/.sbss.lc<k> pieces made by split_bss)."""
    out = {}
    with open(cobj, "rb") as f:
        e = ELFFile(f)
        st = e.get_section_by_name(".symtab")
        for x in st.iter_symbols() if st else []:
            if not x.name or not isinstance(x["st_shndx"], int) or x["st_info"]["type"] == "STT_SECTION":
                continue
            sec = e.get_section(x["st_shndx"]).name
            if is_bss(sec):
                out.setdefault(sec, []).append((x["st_value"], x.name, x["st_info"]["bind"] == "STB_GLOBAL"))
    return out


def place_c(cobj, block, addrs):
    """[(addr, size, obj, section, bytes)] for the C object's own data sections
    (.rodata, .sdata, .data, and .rodata.lc<k> pieces made by split_rodata), and
    [(addr, size, obj, section, None)] for its .bss/.sbss (or .bss.lc<k>/.sbss.lc<k>
    pieces made by split_bss): every label in a bss section must sit at its original
    address, i.e. all give the same base = address - offset."""
    with open(cobj, "rb") as f:
        e = ELFFile(f)
        secs = {s.name: s["sh_size"] for s in e.iter_sections()}
        content = {s.name: s.data() for s in e.iter_sections()
                   if s.name in DATASECS or s.name.startswith(".rodata.lc")}
    need = [s for s in content if secs.get(s)]
    bss = [s for s in secs if is_bss(s) and secs[s]]
    if not need and not bss:
        return []
    bases = pair_bases(cobj, block, addrs, need)
    out = []
    labels = bss_labels(cobj) if bss else {}
    for s in bss:
        if not labels.get(s):
            raise LinkErr(f"cannot place C {s} ({secs[s]} bytes): no label in it")
        miss = [n for _, n, _ in labels[s] if n not in addrs]
        if miss:
            raise LinkErr(f"cannot place C {s} ({secs[s]} bytes): no original address for {miss[0]}")
        b = {addrs[n] - off for off, n, _ in labels[s]}
        if len(b) != 1:
            raise LinkErr(f"cannot place C {s} ({secs[s]} bytes): inconsistent bases " +
                          ",".join(f"0x{x:X}" for x in sorted(b)))
        a = b.pop()
        if "main_BSS_START" not in addrs or a < addrs["main_BSS_START"] or a + secs[s] > RAM_END:
            raise LinkErr(f"C {s} at 0x{a:X} (0x{secs[s]:X} bytes) is outside the original bss")
        out.append((a, secs[s], cobj, s, None))
    for s in need:
        if len(bases[s]) != 1:
            raise LinkErr(f"cannot place C {s} ({secs[s]} bytes): " +
                          ("no paired relocation" if not bases[s] else
                           "inconsistent bases " + ",".join(f"0x{b:X}" for b in sorted(bases[s]))))
        out.append((bases[s].pop(), secs[s], cobj, s, content[s]))
    return out


LABEL_RE = re.compile(r"\s*[A-Za-z_.$][\w.$]*:\s*$")
SECDIR = (".section", ".text", ".data", ".sdata", ".rdata", ".bss", ".sbss")


def _split_asm(ms, groups):
    """maspsx output with every `.section .rodata` chunk cut at its labels into units
    (a label plus the .align lines just before it). groups=None puts unit i in section
    .rodata.u<i>; otherwise groups[i] is the piece number k (section .rodata.lc<k>).
    Returns (text, number of units)."""
    out, n, inro = [], 0, False
    for l in ms.split("\n"):
        st = l.strip()
        if st.startswith(SECDIR):
            if inro:
                out += pend
            inro = st == ".section .rodata"
            if inro:
                pend, started = [], False
                continue
        if not inro:
            out.append(l)
            continue
        if st.startswith(".align") or not st or st.startswith("#"):
            pend.append(l)
            continue
        if LABEL_RE.match(l) or not started:
            name = f".rodata.u{n}" if groups is None else f".rodata.lc{groups[n]}"
            out.append(f'.section {name},"a",@progbits')
            n += 1
            started = True
        out += pend + [l]
        pend = []
    if inro:
        out += pend
    return "\n".join(out), n


def _assemble_ms(text, o):
    s = o[:-2] + ".m.s"
    open(s, "w").write(text)
    r = sh(AS + ["-o", o, s])
    if r.returncode:
        raise LinkErr("split: as: " + r.stderr[-300:])


def _sections(o):
    with open(o, "rb") as f:
        e = ELFFile(f)
        return {s.name: (s["sh_size"], s["sh_addralign"], s.data() if s["sh_type"] != "SHT_NOBITS" else b"")
                for s in e.iter_sections() if s.name}


def maspsx_text(cpath, cobj):
    """The maspsx output (assembler input) of cpath, with fnscore.compile_c's settings;
    checked: assembled unchanged, it reproduces cobj."""
    o_keep = cobj[:-2] + ".keep.o"
    a = types.SimpleNamespace(cc="2.7.2-cdk", opt="-O2", G="8", aspsx="2.79", extra="", keep_s=True)
    err = fnscore.compile_c(cpath, o_keep, a)
    if err:
        raise LinkErr("compile error: " + " ".join(err.split())[:200])
    # the same maspsx step as fnscore.compile_c; checked below by reassembling unchanged
    mx = [sys.executable, os.path.join(ROOT, "tools/maspsx/maspsx.py"), "--aspsx-version=2.79", "--expand-div", "-G8"]
    env = dict(os.environ, MASPSX_EXTERN_G=os.environ.get("MASPSX_EXTERN_G", "4"),
               MASPSX_GP_ALLOW_OFFSET=os.environ.get("MASPSX_GP_ALLOW_OFFSET", "0"))
    r = subprocess.run(mx, stdin=open(o_keep[:-2] + ".cc1.s"), cwd=ROOT, capture_output=True, text=True, env=env)
    if r.returncode:
        raise LinkErr("split: maspsx: " + r.stderr[-300:])
    ms = '.include "macro.inc"\n' + r.stdout
    same = cobj[:-2] + ".same.o"
    _assemble_ms(ms, same)
    if _sections(same) != _sections(cobj):
        raise LinkErr("split: reassembling the maspsx output does not reproduce the C object")
    return ms


BSS_RE = re.compile(r"(?:\.section\s+)?(\.s?bss)(?=[\s,]|$)")


def split_bss(ms):
    """maspsx output with every .bss/.sbss chunk cut at its labels: label i (with the
    .align lines just before it) goes in its own section <.bss|.sbss>.lc<i>."""
    out, n, inb = [], 0, None
    for l in ms.split("\n"):
        st = l.strip()
        if st.startswith(SECDIR):
            if inb:
                out += pend
            m = BSS_RE.fullmatch(st.split(",")[0].strip())
            inb = m.group(1) if m else None
            if inb:
                pend, started = [], False
                continue
        if not inb:
            out.append(l)
            continue
        if st.startswith((".align", "#", ".globl", ".global")) or not st:
            pend.append(l)
            continue
        if LABEL_RE.match(l):
            out.append(f'.section {inb}.lc{n},"aw",@nobits')
            n += 1
            started = True
        elif not started:
            raise LinkErr(f"bss split: {inb} has content before its first label: {st[:60]}")
        out += pend + [l]
        pend = []
    if inb:
        out += pend
    return "\n".join(out)


def split_rodata(ms, cobj, block, addrs):
    """For a C object whose .rodata pairs to several original ranges: return its maspsx
    output `ms` with .rodata in one section per contiguous original range
    (.rodata.lc<k>). Every unit (label) must pair to exactly one base."""
    # 1) one section per unit, to learn each unit's original address
    text, n = _split_asm(ms, None)
    uo = cobj[:-2] + ".units.o"
    _assemble_ms(text, uo)
    usec = _sections(uo)
    units = [f".rodata.u{i}" for i in range(n)]
    if sum(usec[u][0] for u in units) == 0 or ".rodata" in usec and usec[".rodata"][0]:
        raise LinkErr("rodata split: could not cut .rodata at its labels")
    ub = pair_bases(uo, block, addrs, units)
    for i, u in enumerate(units):
        if len(ub[u]) > 1:
            raise LinkErr(f"cannot place C .rodata: unit {i} ({usec[u][0]} bytes) has inconsistent bases " +
                          ",".join(f"0x{b:X}" for b in sorted(ub[u])))
    ubase = {u: min(b) for u, b in ub.items() if b}
    # 2) group units that sit back to back (with C's alignment) in the original
    groups, k, prev_end = [], -1, None
    for i, u in enumerate(units):
        size, align, _ = usec[u]
        if size == 0:
            groups.append(max(k, 0))
            continue
        if u not in ubase:
            raise LinkErr(f"cannot place C .rodata: unit {i} ({size} bytes) has no paired relocation")
        if prev_end is not None:
            al = max(align, 1)
            if ubase[u] == (prev_end + al - 1) // al * al:
                groups.append(k)
                prev_end = ubase[u] + size
                continue
        k += 1
        groups.append(k)
        prev_end = ubase[u] + size
    if k < 0:
        raise LinkErr("rodata split: no .rodata units")
    if k == 0:
        raise LinkErr("rodata split: every unit is contiguous, yet the bases disagree")
    return _split_asm(ms, groups)[0]


def split_kind(e):
    """(needs split_rodata, needs split_bss) for a place_c error."""
    m = str(e)
    return (m.startswith("cannot place C .rodata ") and "inconsistent bases" in m,
            m.startswith(("cannot place C .bss ", "cannot place C .sbss ")) and "inconsistent bases" in m)


def split_c(cpath, cobj, block, addrs, rodata, bss):
    """Rebuild a C object from its maspsx output with .rodata split by split_rodata
    and/or .bss/.sbss split by split_bss, and return (new object, its data pieces).
    The .text must keep its size; every piece must pair to exactly one base."""
    ms = maspsx_text(cpath, cobj)
    while True:
        text = split_bss(ms) if bss else ms
        if rodata:
            text = split_rodata(text, cobj, block, addrs)
        so = cobj[:-2] + ".split.o"
        _assemble_ms(text, so)
        if _sections(so)[".text"][0] != _sections(cobj)[".text"][0]:
            raise LinkErr("split: .text size changed")
        try:
            return so, place_c(so, block, addrs)      # 3) raises unless every piece has one base
        except LinkErr as e:
            r, b = split_kind(e)
            if (r and not rodata) or (b and not bss):
                rodata, bss = rodata or r, bss or b
                continue
            raise


def compile_one(sym, path):
    o = os.path.join(LC, "obj", os.path.basename(path)[:-2] + ".o")
    os.makedirs(os.path.dirname(o), exist_ok=True)
    a = types.SimpleNamespace(cc="2.7.2-cdk", opt="-O2", G="8", aspsx="2.79", extra="", keep_s=False)
    os.makedirs(fnscore.B, exist_ok=True)
    err = fnscore.compile_c(path, o, a)
    return o, err


# ---------------------------------------------------------------- main
def first_diff(a, b, lo, hi):
    for i in range(lo, hi):
        if i >= len(a) or i >= len(b) or a[i] != b[i]:
            return i
    return None


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("symbols", nargs="*")
    ap.add_argument("--wip", nargs="+", default=[], metavar="SYMBOL", help="allow src/pilot/SYMBOL.wip.c")
    ap.add_argument("--all", action="store_true", help="include every matched src/pilot/*.c as well")
    ap.add_argument("-o", "--out", default=None, help="write a TSV (symbol, pass/fail, detail)")
    ap.add_argument("-j", "--jobs", type=int, default=8)
    a = ap.parse_args()

    pilot = os.path.join(ROOT, "src", "pilot")
    todo = []                                                   # (sym, path) - snapshot now
    if a.all or (not a.symbols and not a.wip):
        for p in sorted(glob.glob(os.path.join(pilot, "*.c"))):
            if not p.endswith((".wip.c", ".best.c")):
                todo.append((os.path.basename(p)[:-2], p))
    for s in a.symbols:
        todo.append((s, os.path.join(pilot, s + ".c")))
    for s in a.wip:
        p = os.path.join(pilot, s + ".c")
        todo.append((s, p if os.path.exists(p) else os.path.join(pilot, s + ".wip.c")))
    seen, uniq = set(), []
    for s, p in todo:
        if s not in seen:
            seen.add(s); uniq.append((s, p))
    todo = uniq
    snap = {}
    for s, p in todo:                                           # read sources once, up front
        try:
            snap[s] = open(p).read()
        except OSError:
            snap[s] = None
    srcdir = os.path.join(LC, "src")
    os.makedirs(srcdir, exist_ok=True)

    files = prepare(a.jobs)
    tblocks = {b.name: b for b in files["asm/2C8C.s"][1]}
    base_script = gen_script(files, {}, [], {}, "baseline")
    elf, exe = link(base_script, "baseline")
    orig = open(ORIG, "rb").read()
    got = open(exe, "rb").read()
    if hashlib.sha1(got).hexdigest() != SHA1 or got != orig:
        sys.exit(f"baseline relink is not byte-identical (first diff at file offset "
                 f"0x{first_diff(orig, got, 0, max(len(orig), len(got))) or 0:X}); the block split is wrong")
    addrs = symtab(elf)
    owner = {}                                                  # symbol -> block that defines it
    for src, (sec, blocks) in files.items():
        for b in blocks:
            for s in b.syms:
                owner.setdefault(s, b)
    print(f"baseline relink from {sum(len(b) for _, b in files.values())} blocks: sha1 OK", flush=True)

    def prep(item):
        sym, path = item
        if snap[sym] is None:
            raise LinkErr(f"{os.path.relpath(path, ROOT)} vanished")
        if sym not in tblocks:
            raise LinkErr(f"no asm block named {sym} in asm/2C8C.s")
        # compile the snapshot, not the live file (other agents edit src/pilot)
        cpath = os.path.join(srcdir, os.path.basename(path))
        write_if_changed(cpath, snap[sym])
        o, err = compile_one(sym, cpath)
        if err:
            raise LinkErr("compile error: " + " ".join(err.split())[:200])
        secs, _ = elf_info(o)
        b = tblocks[sym]
        n = secs.get(".text", 0)
        pad = b.size - n
        lo = b.addr - VRAM + FILEOFF
        # a shorter C .text is accepted only if the rest of the asm block is zero
        # padding (an alignment nop between objects); the byte compare covers it
        if pad < 0 or (pad and any(orig[lo + n:lo + b.size])):
            raise LinkErr(f"C .text is 0x{n:X} bytes, asm block is 0x{b.size:X}")
        try:
            pieces = place_c(o, b, addrs)
        except LinkErr as e:
            # .rodata spread over several original ranges: one piece per range;
            # bss variables that are not laid out as in C: one piece per variable
            r, bs = split_kind(e)
            if not (r or bs):
                raise
            o, pieces = split_c(cpath, o, b, addrs, r, bs)
        return o, pieces, pad

    def provides_for(repl, pieces):
        dropped = set(repl)
        cut = data_layout(files, pieces)[2]
        prov = {}
        for s, b in owner.items():
            if (b.src == "asm/2C8C.s" and b.name in dropped) or (b.src, b.idx) in cut:
                if s in addrs and s not in repl:
                    prov[s] = addrs[s]
        return prov

    def check(item):
        sym, path = item
        rel = os.path.relpath(path, ROOT)
        try:
            o, pieces, pad = prep(item)
            repl = {sym: o}
            script = gen_script(files, repl, pieces, provides_for(repl, pieces), sym, {sym: pad})
            _, exe = link(script, sym, pieces)
        except LinkErr as e:
            return sym, "fail", f"{rel}: {e}", None
        out = open(exe, "rb").read()
        b = tblocks[sym]
        lo = b.addr - VRAM + FILEOFF
        d = first_diff(orig, out, lo, lo + b.size)
        exe_ok = out == orig
        tail = "exe identical" if exe_ok else f"exe differs at file offset 0x{first_diff(orig, out, 0, max(len(orig), len(out))):X}"
        if d is not None:
            w = orig[d & ~3:(d & ~3) + 4][::-1].hex()
            g = out[d & ~3:(d & ~3) + 4][::-1].hex()
            return sym, "fail", f"{rel}: first diff at +0x{d - lo:X} (0x{d - FILEOFF + VRAM:08X}) want {w} got {g}; {tail}", None
        for addr, size, _, sec, data in pieces:
            if data is None:
                continue                                      # bss: no bytes; link() checked it
            p = addr - VRAM + FILEOFF
            dd = first_diff(orig, out, p, p + size)
            if dd is not None:
                return sym, "fail", f"{rel}: code identical but C {sec} at 0x{addr:X} differs at +0x{dd - p:X}; {tail}", None
        extra = "".join(f", {s} at 0x{x:X}" for x, _, _, s, _ in pieces)
        extra += f", +0x{pad:X} zero padding" if pad else ""
        return sym, "pass", f"{rel}: 0x{b.addr:08X} size 0x{b.size:X}{extra}; {tail}", (o, pieces, pad)

    results = []
    with cf.ThreadPoolExecutor(a.jobs) as ex:
        for i, r in enumerate(ex.map(check, todo)):
            results.append(r)
            print(f"{r[0]}\t{r[1]}\t{r[2]}", flush=True)

    passing = [(r[0], r[3]) for r in results if r[1] == "pass"]
    repl = {s: x[0] for s, x in passing}
    pieces = [p for _, x in passing for p in x[1]]
    pads = {s: x[2] for s, x in passing}
    try:
        script = gen_script(files, repl, pieces, provides_for(repl, pieces), "combined", pads)
        _, exe = link(script, "combined", pieces)
        out = open(exe, "rb").read()
        h = hashlib.sha1(out).hexdigest()
        ok = h == SHA1
        comb = ("pass" if ok else "fail",
                f"{len(passing)} passing functions linked together: sha1 {h}" +
                ("" if ok else f" (first diff at file offset 0x{first_diff(orig, out, 0, max(len(orig), len(out))):X})"))
    except LinkErr as e:
        comb = ("fail", f"combined link failed: {e}")
    print(f"(combined)\t{comb[0]}\t{comb[1]}")
    n = len(results)
    print(f"{len(passing)}/{n} pass")
    if a.out:
        with open(os.path.join(ROOT, a.out) if not os.path.isabs(a.out) else a.out, "w") as f:
            f.write("symbol\tresult\tdetail\n")
            for s, res, det, _ in results:
                f.write(f"{s}\t{res}\t{det}\n")
            f.write(f"(combined)\t{comb[0]}\t{comb[1]}\n")
    sys.exit(0 if comb[0] == "pass" and len(passing) == n else 1)


if __name__ == "__main__":
    main()
