#!/usr/bin/env python3
"""Driver 1 PC port: the data side of the link (a build step, see port/CMakeLists.txt).

    pc_gendata.py fixups REPO OUT/pc_fixups.s
    pc_gendata.py syms   REPO OUT/pc_syms.ld OBJ... [--lib=ARCHIVE_OR_SO ...]
                    (OBJ = every object of the exe; --lib only sorts the report)
    pc_gendata.py symfile EXE OUT/driver1.sym     (Windows: the exe's symbols with sizes, for pc_trace.c/lockstep.py)
    --pe: Windows (PE/COFF) objects, see below

Outputs are build products, never committed. The model (port/README.md): PS1 RAM is mapped at its own
addresses (pc_psxram = 0), SLUS_008.42 is loaded there at startup and each overlay when the game loads it, so
all PS1 data the C files do not own yet lives at its PS1 address with its PS1 contents, and every pointer
inside it (0x001C00D8 ...) is already a valid PC pointer. Two things need help:

pc_fixups.s: the data words splat identified as pointers (`.word func_x`, `.word D_x + n`), per image, as
  {PS1 address, PC symbol}. pc_main.c rewrites them after loading the image: function pointers get the PC
  function, pointers to C-owned data the C object (other pointers are rewritten to the value they had).

pc_syms.ld: one PROVIDE() per symbol the objects leave undefined (a PROVIDE only binds undefined symbols):
  1. PsyQ library functions known only by address -> their implementation (PSYQ_ALIAS)
  2. a name inside an object a C file defines -> owner + offset, from the PS1 addresses encoded in both
     names (D_000C4C7C = D_000C4690 + 0x5EC)
  3. data in a PS1 image (main 0x10000-0xD0054 incl. .bss, the overlays' ranges) -> the PS1 address itself
  4. splat's short overlay call names (func_1E0290) -> the overlay's function at that address
  A name none of these resolve is left to the linker and listed in pc_syms.txt, next to every resolution.

--pe (Windows, MinGW i686): C names carry a leading underscore in the objects and the scripts. COFF keeps no
  symbol sizes: a data object's size comes from its type in the DWARF debug info (the build is -g; objdump
  --dwarf=info). (Not -fdata-sections: COFF pads every such section to 4 bytes, which moves the neighbours.) PS1 RAM
  is part of the exe image (port/pc/pc_main.c), whose first 0x400 bytes, PS1 0x10000-0x103FF, stay the PE
  headers: that part of SLUS_008.42 lives in pc_hdrshadow, and names inside it resolve there like any C-owned
  object.
"""
import glob, os, re, subprocess, sys

PE = "--pe" in sys.argv
U = "_" if PE else ""                  # the C-name prefix in objects and scripts
HDR_SHADOW = ("pc_hdrshadow", 0x10000, 0x10400)
RAW = set()                            # --pe: names referenced through an asm label (`__asm__("D_x")`), no prefix

DATA_FILES = [
    ("main", "asm/data/800.rodata.s"), ("main", "asm/data/75B14.data.s"),
    ("frontend", "asm/frontend/data/frontend_rodata.rodata.s"), ("frontend", "asm/frontend/data/frontend_data.data.s"),
    ("mcard", "asm/mcard/data/mcard_rodata.rodata.s"), ("mcard", "asm/mcard/data/mcard_data.data.s"),
    ("iview", "asm/iview/data/iview_rodata.rodata.s"), ("iview", "asm/iview/data/iview_data.data.s"),
    ("cred", "asm/cred/data/cred_rodata.rodata.s"), ("cred", "asm/cred/data/cred_data.data.s"),
]
# PS1 images resident in PS1 RAM: main (rodata+text+data+bss), overlays (file incl. zero tail; CRED/IVIEW share)
IMAGE_RANGES = [("main", 0x10000, 0xD0054), ("mcard", 0xE6600, 0xF2A88), ("frontend", 0x1C0000, 0x1D3308),
                ("cred/iview", 0x1E0000, 0x1F3878)]
CODE_RANGES = [("mcard", 0xE6600, 0xED52C), ("frontend", 0x1C0000, 0x1CDC80),
               ("iview", 0x1E0000, 0x1E3358), ("cred", 0x1E0000, 0x1E1AB4)]

# PsyQ functions the game calls by address -> implementation (PsyCross, or port/pc/pc_cd.c for libcd).
# How each was identified: port/README.md. The ones with PC-only behaviour are defined in port/pc/pc_psyq.c.
PSYQ_ALIAS = {
    "func_000739B4": "SetGeomOffset", "func_00077D6C": "SetGeomScreen",
    "func_00073F88": "ResetGraph", "func_000740FC": "SetGraphDebug",
    "func_00074374": "ClearImage", "func_00074404": "LoadImage", "func_000744C4": "MoveImage",
    "func_0007457C": "ClearOTagR", "func_00074684": "DrawOTag", "func_00074CAC": "SetDrawArea",
    "func_000766A8": "StoreImage", "func_000776F4": "ResetCallback", "func_000777E8": "StopCallback",
    "func_00079800": "SetVideoMode", "func_0007A7F4": "GetClut", "func_0007D608": "LoadClut2",
    "func_0007D66C": "SetDefDrawEnv", "func_0007D720": "SetDefDispEnv",
    "func_0007AEB4": "PadStartCom", "func_0007AED4": "PadGetState", "func_0007AF94": "PadInfoMode",
    "func_0007B160": "PadSetActAlign", "func_0007B198": "PadSetAct",
    "func_0007C7D4": "SpuSetReverbVoice",
    # (libmcrd: pc_psyq.c - PsyCross's LIBMCRD indexes a 2-entry array with chan = port << 4 and fseeks a NULL
    # FILE when 0.MCD is missing)
    # libcd (port/pc/pc_cd.c)
    "func_00079F70": "CdSync", "func_0007A12C": "CdReadyCallback", "func_0007A27C": "CdControlF",
    "func_0007A4FC": "CdGetSector", "func_0007A53C": "CdDataCallback", "func_0007A580": "CdIntToPos",
    "func_0007E8FC": "CdReadFile",
    # (the BIOS libc calls - memcpy, strcmp ... - are wrappers in pc_psyq.c: a linker-script alias can't name a
    # shared-library symbol)
}


def gen_fixups(repo, out_path):
    """pc_fixups.s: per PS1 image (main, each overlay) the words of its .rodata/.data that hold a pointer splat
    identified (`.word func_x` / `.word D_x + n`): {PS1 address, PC symbol}. pc_main.c applies them after the
    image is loaded into PS1 RAM, so tables of function pointers and pointers to C-owned data point at the PC
    objects. Pointers into PS1 RAM itself need nothing: the image sits at its own addresses."""
    line_re = re.compile(r"^\s*/\*\s*([0-9A-F]+)\s+([0-9A-F]{8})(?:\s+([0-9A-F]+))?\s*\*/\s*\.word\s+(.*)$")
    fix = {}
    for ov, path in DATA_FILES:
        for l in open(os.path.join(repo, path), encoding="latin1"):
            m = line_re.match(l)
            if not m:
                continue
            arg = m.group(4).split("/*")[0].strip()
            if re.fullmatch(r"-?(0x[0-9A-Fa-f]+|\d+)", arg) or arg.startswith(".L"):
                continue                     # a number, or a jump table entry (only the PS1 asm used those)
            rv = int.from_bytes(bytes.fromhex(m.group(3)), "little")
            if "+" in arg and rv % 4:
                continue                     # splat's `D_00010000 + 0x1` etc.: flag bytes, not pointers
            t = re.match(r"(?:func|D)_([0-9A-F]+)", arg)
            ta = int(t.group(1), 16) if t else None
            if ta is not None and arg.startswith("func_") and 0x738F0 <= ta < 0x85314:
                continue                     # PsyQ's own tables (GPU/pad/SPU dispatch): only the replaced library read them
            if ta is not None and 0x1F800000 <= ta < 0x1F800400:
                arg = f"pc_scratchpad + 0x{ta - 0x1F800000:X}" + arg[t.end():]
            fix.setdefault(ov, []).append((int(m.group(2), 16), arg))
    # Function pointers splat printed as .byte/.short runs (menu tables like frontend D_001CE048, mission.c's
    # handler tables): an aligned word of such a run whose value is the exact entry address of a game function
    # (a `glabel func_X` in the asm) is a pointer to it as well.
    funcs = set()
    for f in glob.glob(os.path.join(repo, "asm", "**", "*.s"), recursive=True):
        for l in open(f, encoding="latin1"):
            m = re.match(r"\s*glabel\s+func_([0-9A-F]{8})\b", l)
            if m:
                funcs.add(int(m.group(1), 16))
    small_re = re.compile(r"^\s*/\*\s*[0-9A-F]+\s+([0-9A-F]{8})\s*\*/\s*\.(byte|short)\s+(0x[0-9A-Fa-f]+|\d+)\s*$")
    for ov, path in DATA_FILES:
        mem = {}
        for l in open(os.path.join(repo, path), encoding="latin1"):
            m = small_re.match(l)
            if m:
                a, v = int(m.group(1), 16), int(m.group(3), 0)
                mem[a] = v & 0xFF
                if m.group(2) == "short":
                    mem[a + 1] = v >> 8 & 0xFF
        for a in sorted(mem):
            if a % 4 == 0 and all(a + i in mem for i in range(4)):
                v = mem[a] | mem[a + 1] << 8 | mem[a + 2] << 16 | mem[a + 3] << 24
                if v in funcs and not 0x738F0 <= v < 0x85314:
                    fix.setdefault(ov, []).append((a, f"func_{v:08X}"))
    out = ["# generated by port/pc/pc_gendata.py from the PS1 data asm - do not edit",
           '\t.section .rdata,"dr"' if PE else "\t.section .rodata", "\t.p2align 2"]
    for ov in ("main", "frontend", "mcard", "cred", "iview"):
        f = fix.get(ov, [])
        out += [f"\t.globl {U}pc_fixups_{ov}", f"{U}pc_fixups_{ov}:", f"\t.long {len(f)}"]
        out += [f"\t.long 0x{a:08X}, {U}{e}" for a, e in f]
    if not PE:
        out.append('\t.section .note.GNU-stack,"",@progbits')
    open(out_path, "w").write("\n".join(out) + "\n")


def gen_syms(repo, out_path, objs, libs):
    sym_addr, addr_names, funcs = {}, {}, set()
    for f in sorted(glob.glob(os.path.join(repo, "symbol_addrs*.txt"))):
        ov = os.path.basename(f).split(".")[1] if f.count(".") > 1 else "main"
        for l in open(f, encoding="latin1"):
            m = re.match(r"\s*([A-Za-z_]\w*)\s*=\s*(0x[0-9A-Fa-f]+)", l)
            if m:
                sym_addr.setdefault(m.group(1), int(m.group(2), 16))
                addr_names.setdefault((ov, int(m.group(2), 16)), m.group(1))
                if "type:func" in l:
                    funcs.add(m.group(1))

    def addr_of(n):
        m = re.fullmatch(r"(?:D|func)_([0-9A-F]{5,8})", n)
        return int(m.group(1), 16) if m else sym_addr.get(n)

    nm = os.environ.get("NM", "nm")
    defined, undef = {}, set()
    for o in objs:
        lines = subprocess.run([nm, "-S", o], capture_output=True, text=True, check=True).stdout.splitlines()
        if PE:
            lines = coff_sized(o, lines)
        for l in lines:
            p = l.split()
            if len(p) == 2 and p[0] == "U":
                undef.add(p[1])
            elif len(p) == 4:
                t, n = p[2], p[3]
                strong = t not in "vVwW"
                if n not in defined or (strong and not defined[n][1]):
                    defined[n] = (int(p[1], 16), strong, t)
            elif len(p) == 3 and p[1] not in "Uu":
                defined.setdefault(p[2], (0, p[1] not in "vVwW", p[1]))
    undef -= set(defined)
    owners = []                     # C-defined data objects with a known PS1 address
    for n, (size, strong, t) in defined.items():
        a = addr_of(n)
        if a is not None and size > 0 and strong and t.upper() in "BDRCGS":
            owners.append((a, a + size, strong, n))
    if PE:
        owners.append((HDR_SHADOW[1], HDR_SHADOW[2], True, HDR_SHADOW[0]))
    owners.sort()
    ld = ["/* generated by port/pc/pc_gendata.py - do not edit */",
          f"PROVIDE({U}pc_psxram = 0x0);   /* PS1 main RAM: pages 0x10000-0x1FFFFF, port/pc/pc_main.c */"]
    rep, unres = [], []
    for n in sorted(undef):
        if n == "pc_psxram" or n.startswith(("__wrap_", "__real_")):
            continue
        if n in PSYQ_ALIAS:
            ld += [f"EXTERN({U}{PSYQ_ALIAS[n]});", f"PROVIDE({U}{n} = {U}{PSYQ_ALIAS[n]});"]
            rep.append(f"psyq   {n} -> {PSYQ_ALIAS[n]}")
            continue
        a = addr_of(n)
        if a is None:
            unres.append(n)
            continue
        isfunc = n.startswith("func_") or n in funcs
        hits = [] if isfunc else [o for o in owners if o[0] <= a < o[1]]
        if hits:
            o = max(hits)                                   # the innermost
            ld.append(f"PROVIDE({U}{n} = {U}{o[3]} + 0x{a - o[0]:X});")
            rep.append(f"owner  {n} -> {o[3]} + 0x{a - o[0]:X}")
            continue
        img = [r for r in IMAGE_RANGES if r[1] <= a < r[2]]
        if img and not isfunc:
            ld.append(f"PROVIDE({U}{n} = 0x{a:08X});")
            rep.append(f"ps1    {n} -> PS1 RAM 0x{a:X} ({img[0][0]})")
            continue
        m = re.fullmatch(r"func_([0-9A-F]{5,6})", n)
        tgt = None
        if m:
            for ov, lo, hi in CODE_RANGES:
                if lo <= a < hi:
                    for c in (addr_names.get((ov, a)), f"func_{a:08X}"):
                        if c and c in defined:
                            tgt = c
                            break
                if tgt:
                    break
        if tgt:
            ld.append(f"PROVIDE({U}{n} = {U}{tgt});")
            rep.append(f"ovcall {n} -> {tgt}")
            continue
        unres.append(n)
    libdef = set()
    for lib in libs:
        r = subprocess.run([nm, "-D" if ".so" in lib else "-g", "--defined-only", lib], capture_output=True, text=True)
        for l in r.stdout.splitlines():
            p = l.split()
            if len(p) >= 3:
                libdef.add(unprefix(p[2]).split("@")[0])
    if PE:
        # an asm-label reference gets the same resolution under its unprefixed name
        rhs = {}
        for l in ld:
            m = re.match(r"PROVIDE\(_(\w+) = (.*)\);$", l)
            if m:
                rhs[m.group(1)] = m.group(2)
        for n in sorted(RAW):
            if n in rhs:
                ld.append(f"PROVIDE({n} = {rhs[n]});")
            elif n in defined:
                ld.append(f"PROVIDE({n} = _{n});")
    fromlib = [n for n in unres if n in libdef]
    unres = [n for n in unres if n not in libdef]
    open(out_path, "w").write("\n".join(ld) + "\n")
    with open(os.path.splitext(out_path)[0] + ".txt", "w") as f:
        f.write("\n".join(rep) + "\n\n# from the libraries (PsyCross / libc):\n" + " ".join(fromlib) +
                "\n\n# MISSING (left to the linker):\n" + "\n".join(unres) + "\n")
    kinds = {}
    for r in rep:
        kinds[r.split()[0]] = kinds.get(r.split()[0], 0) + 1
    print("pc_gendata syms: " + ", ".join(f"{k} {v}" for k, v in sorted(kinds.items())) +
          f", from libraries {len(fromlib)}, MISSING {len(unres)}" + (": " + " ".join(unres) if unres else ""))


def unprefix(n):
    return n[1:] if PE and n.startswith("_") else n


def dwarf_sizes(path):
    """{name: byte size} of the file-scope variables in an object's or exe's DWARF info (None where two
    compilation units define different objects of that name)."""
    txt = subprocess.run([os.environ.get("OBJDUMP", "objdump"), "--dwarf=info", path], capture_output=True,
                         text=True, errors="replace").stdout
    dies, stack, cur = {}, [], None
    die_re = re.compile(r"^\s*<(\d+)><([0-9a-f]+)>: Abbrev Number: (\d+)(?: \((\w+)\))?")
    at_re = re.compile(r"^\s*<[0-9a-f]+>\s+(DW_AT_\w+)\s*:\s*(.*?)\s*$")
    for l in txt.splitlines():
        m = die_re.match(l)
        if m:
            depth = int(m.group(1))
            if m.group(3) == "0":
                cur = None
                continue
            cur = {"tag": m.group(4), "depth": depth, "at": {}, "kids": []}
            off = int(m.group(2), 16)
            dies[off] = cur
            del stack[depth:]
            if stack:
                stack[-1]["kids"].append(cur)
            stack.append(cur)
            continue
        m = at_re.match(l)
        if m and cur is not None:
            v = m.group(2)
            if v.startswith("(indirect"):
                v = v.split("): ", 1)[-1]
            cur["at"][m.group(1)] = v

    def num(v):
        try:
            return int(v.split()[0], 0)
        except (ValueError, IndexError):
            return None

    def ref(v):
        m = re.match(r"<0x([0-9a-f]+)>", v or "")
        return dies.get(int(m.group(1), 16)) if m else None

    memo = {}

    def size(d, depth=0):
        if d is None or depth > 40:
            return None
        if id(d) in memo:
            return memo[id(d)]
        a, t, r = d["at"], d["tag"], None
        if "DW_AT_byte_size" in a:
            r = num(a["DW_AT_byte_size"])
        elif t == "DW_TAG_pointer_type":
            r = 4
        elif t == "DW_TAG_array_type":
            r = size(ref(a.get("DW_AT_type")), depth + 1)
            for k in d["kids"]:
                if k["tag"] == "DW_TAG_subrange_type" and r is not None:
                    ka = k["at"]
                    n = num(ka["DW_AT_count"]) if "DW_AT_count" in ka else (
                        num(ka["DW_AT_upper_bound"]) + 1 if num(ka.get("DW_AT_upper_bound", "")) is not None else 0)
                    r *= n
        elif "DW_AT_type" in a:
            r = size(ref(a["DW_AT_type"]), depth + 1)
        memo[id(d)] = r
        return r

    out = {}
    for d in dies.values():
        a = d["at"]
        if d["tag"] != "DW_TAG_variable" or d["depth"] != 1 or "DW_AT_declaration" in a:
            continue
        spec = ref(a.get("DW_AT_specification"))
        name = a.get("DW_AT_name") or (spec and spec["at"].get("DW_AT_name"))
        typ = a.get("DW_AT_type") or (spec and spec["at"].get("DW_AT_type"))
        if not name:
            continue
        s = size(ref(typ))
        if s is not None:
            out[name] = s if out.get(name, s) == s else None
    return out


def coff_sized(obj, nm_lines):
    """nm lines of a COFF object with the C prefix dropped and the sizes COFF lacks (from the DWARF info)."""
    sizes = dwarf_sizes(obj)
    out = []
    for l in nm_lines:
        p = l.split()
        if len(p) == 2:
            if not p[1].startswith("_"):
                RAW.add(p[1])
            out.append(f"{p[0]} {unprefix(p[1])}")
        elif len(p) == 3:
            n = unprefix(p[2])
            if p[1].upper() in "BDR" and sizes.get(n):
                out.append(f"{p[0]} {sizes[n]:08x} {p[1]} {n}")
            else:
                out.append(f"{p[0]} {p[1]} {n}")
    return out


def gen_symfile(exe, out_path):
    """driver1.sym: `nm -S`-style lines (address size type name) for the Windows exe, which keeps no sizes: a data
    object's from the DWARF info, anything else (functions) up to the next symbol in its section."""
    dsz = dwarf_sizes(exe)
    secs = []
    for l in subprocess.run([os.environ.get("OBJDUMP", "objdump"), "-h", exe], capture_output=True, text=True,
                            check=True).stdout.splitlines():
        m = re.match(r"\s*\d+\s+(\S+)\s+([0-9a-fA-F]+)\s+([0-9a-fA-F]+)\s", l)
        if m:
            secs.append((int(m.group(3), 16), int(m.group(3), 16) + int(m.group(2), 16)))
    syms = []
    for l in subprocess.run([os.environ.get("NM", "nm"), exe], capture_output=True, text=True,
                            check=True).stdout.splitlines():
        p = l.split()
        if len(p) == 3 and p[1] not in "Uu" and not p[2].startswith("."):   # (not section symbols)
            syms.append((int(p[0], 16), p[1], unprefix(p[2])))
    syms.sort()
    addrs = sorted({a for a, t, _ in syms if t.upper() in "TBDR"})   # (A = linker-script aliases, inside objects)
    nxt = dict(zip(addrs, addrs[1:]))
    out = []
    for a, t, n in syms:
        sec = next((s for s in secs if s[0] <= a < s[1]), None)
        size = 0
        if sec and t.upper() in "TBDR":
            size = min(nxt.get(a, sec[1]), sec[1]) - a
            if t.upper() in "BDR" and dsz.get(n):
                size = dsz[n]
        out.append(f"{a:08x} {size:08x} {t} {n}")
    open(out_path, "w").write("\n".join(out) + "\n")


if __name__ == "__main__":
    sys.argv = [a for a in sys.argv if a != "--pe"]
    if sys.argv[1] == "fixups":
        gen_fixups(sys.argv[2], sys.argv[3])
    elif sys.argv[1] == "symfile":
        gen_symfile(sys.argv[2], sys.argv[3])
    else:
        args = sys.argv[4:]
        libs = [a[6:] for a in args if a.startswith("--lib=")]
        gen_syms(sys.argv[2], sys.argv[3], [a for a in args if not a.startswith("--lib=")], libs)
