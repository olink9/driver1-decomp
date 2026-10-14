#!/usr/bin/env python3
"""Compile one candidate C file and score one function against the original.

    tools/fnscore.py SYMBOL FILE.c [--cc 2.7.2] [--opt -O2] [-G 8] [--aspsx 2.79] [--asm]

Prints `MATCH <percent>`; 100.0 only when objdiff marks no instruction as
different (never rounded). The target is the function's split asm
(asm_fn/nonmatchings/main/SYMBOL.s) assembled on its own; the candidate is FILE
through cpp -> cc1 (compilers/gcc-<cc>-psx) -> maspsx -> GNU as, like the
original PsyQ ccpsx -> aspsx pipeline. Relocations to the same symbol name count
as equal; a candidate's own rodata (a string, a jump table) against the
original's D_/jtbl_ label counts as equal when the bytes at the original
address match (jump tables: same entry count). --asm prints both listings.
Run inside distrobox `build`.
"""
import argparse, json, os, re, struct, subprocess, sys, tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
B = os.path.join(ROOT, "build", "fnscore")
FN = os.path.join(ROOT, "asm_fn", "nonmatchings", "main")
EXE = os.path.join(ROOT, "orig", "SLUS_008.42")
OBJDIFF = os.environ.get("OBJDIFF", os.path.expanduser("~/.local/opt/decomp/objdiff-cli"))
AS = ["mips-linux-gnu-as", "-EL", "-Iinclude", "-march=r3000", "-mtune=r3000", "-no-pad-sections", "-G0"]
CPP = [os.path.join(ROOT, "compilers", "gcc-2.7.2-psx", "cpp"), "-Iinclude", "-Iinclude/shim", "-Iinclude/psyq", "-Isrc", "-undef", "-Wall", "-lang-c", "-nostdinc",
       "-Dmips", "-D__GNUC__=2", "-D__OPTIMIZE__", "-D__mips__", "-D__mips", "-Dpsx", "-D__psx__", "-D__psx",
       "-D_PSYQ", "-D__EXTENSIONS__", "-D_MIPSEL", "-D_LANGUAGE_C", "-DLANGUAGE_C", "-DDRIVER1"]


def sfile_of(sym):
    p = os.path.join(FN, sym + ".s")
    return p if os.path.exists(p) else None


def target_obj(sym):
    os.makedirs(B, exist_ok=True)
    o = os.path.join(B, "t_" + sym + ".o")
    s = sfile_of(sym)
    if os.path.exists(o) and os.path.getmtime(o) > os.path.getmtime(s):
        return o
    w = o[:-2] + ".s"
    open(w, "w").write('.include "macro.inc"\n.set noat\n.set noreorder\n.section .text\n.include "%s"\n' % s)
    r = subprocess.run(AS + ["-o", o, w], cwd=ROOT, capture_output=True, text=True)
    if r.returncode:
        sys.exit("target asm failed: " + r.stderr[-1500:])
    return o


def compile_c(path, out, a):
    d = os.path.join(ROOT, "compilers")
    cc1 = next(os.path.join(d, n, "cc1") for n in (f"gcc-{a.cc}-psx", f"gcc-{a.cc}", f"gcc-{a.cc}-plain")
               if os.path.exists(os.path.join(d, n, "cc1")))
    flags = [a.opt, f"-G{a.G}", "-mips1", "-mcpu=3000", "-fgnu-linker", "-mgas", "-msoft-float", "-quiet",
             "-funsigned-char", "-w"] + (a.extra.split() if a.extra else [])
    with tempfile.TemporaryDirectory(dir=B) as t:
        i, s, m = (os.path.join(t, n) for n in ("x.i", "x.s", "m.s"))
        for cmd, stdout in ((CPP + [path, i], None), ([cc1] + flags + [i, "-o", s], None)):
            r = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True)
            if r.returncode:
                return (r.stdout + r.stderr)[-3000:]
        mx = [sys.executable, os.path.join(ROOT, "tools/maspsx/maspsx.py"), f"--aspsx-version={a.aspsx}",
              "--expand-div", f"-G{a.G}"] + (["--dont-force-G0"] if False else [])
        # empirical (2026-10-14): externs of <= 4 bytes are $gp-relative, 8-byte ones are not
        env = dict(os.environ, MASPSX_EXTERN_G=os.environ.get("MASPSX_EXTERN_G", "4"))
        r = subprocess.run(mx, stdin=open(s), cwd=ROOT, capture_output=True, text=True, env=env)
        if r.returncode:
            return "maspsx: " + r.stderr[-2000:]
        open(m, "w").write('.include "macro.inc"\n' + r.stdout)
        r = subprocess.run(AS + ["-o", out, m], cwd=ROOT, capture_output=True, text=True)
        if r.returncode:
            return "as: " + r.stderr[-2000:]
        if a.keep_s:
            open(out[:-2] + ".cc1.s", "w").write(open(s).read())
    return None


def exe_bytes(addr, n):
    d = open(EXE, "rb").read()
    return d[0x800 + addr - 0x10000: 0x800 + addr - 0x10000 + n]


def obj_data(path, name, n=None):
    """(bytes, section name) of a defined symbol in an ELF object, or of a section+offset."""
    from elftools.elf.elffile import ELFFile
    with open(path, "rb") as f:
        e = ELFFile(f)
        st = e.get_section_by_name(".symtab")
        for sym in st.iter_symbols():
            if sym.name == name and isinstance(sym["st_shndx"], int):
                sec = e.get_section(sym["st_shndx"])
                if sec["sh_type"] == "SHT_NOBITS":
                    return None
                size = n or sym["st_size"] or 4
                return sec.data()[sym["st_value"]:sym["st_value"] + size]
    return None


def diff(target, cand, sym):
    d = subprocess.run([OBJDIFF, "diff", "-1", target, "-2", cand, sym, "-o", "-", "--format", "json",
                        "-c", "functionRelocDiffs=name_address"], capture_output=True, text=True)
    try:
        return json.loads(d.stdout)
    except ValueError:
        sys.exit("OBJDIFF ERROR " + d.stderr[-2000:])


def rows(j, side, sym):
    fn = next((x for x in j.get(side, {}).get("symbols", []) if x.get("name") == sym), None)
    return None if fn is None else (fn.get("match_percent"), fn.get("instructions", []))


def reloc_target(row):
    """(name, addend) of an instruction's relocation target from objdiff's text:
    '%hi(D_000A8014+0xd)' -> ('D_000A8014', 13); '%lo([.rodata]+0x10)' -> ('.rodata', 16)."""
    t = (row.get("instruction") or {}).get("formatted", "")
    m = re.search(r"%(?:hi|lo|gp_rel)\(\[?([^)\]+]+)\]?(?:\+(0x[0-9a-fA-F]+|\d+))?\)", t)
    if not m:
        m = re.search(r"\b(jtbl_\w+|D_[0-9A-F]{8})\b()", t)
    if not m:
        return None, 0
    return m.group(1), int(m.group(2), 0) if m.group(2) else 0


def _operands_sans_reloc(ins):
    """The instruction text with its relocation expression blanked, so rows that
    differ only in what the relocation names still have to agree on registers."""
    t = ins.get("formatted", "")
    t = re.sub(r"%(?:hi|lo|gp_rel)\([^)]*\)", "R", t)
    t = re.sub(r"\b(?:jtbl_\w+|D_[0-9A-F]{8}(?:\+0x[0-9a-fA-F]+)?)\b", "R", t)
    return t


def section_bytes(path, name, off, n):
    from elftools.elf.elffile import ELFFile
    with open(path, "rb") as f:
        sec = ELFFile(f).get_section_by_name(name)
        return None if sec is None or sec["sh_type"] == "SHT_NOBITS" else sec.data()[off:off + n]


WHY = []


def relaxed_equal(L, R, cand):
    """True if every differing row differs only by a relocation to the
    candidate's own data whose bytes equal the original's at that address."""
    if len(L) != len(R):
        return False, 0
    n = 0
    for i, (l, r) in enumerate(zip(L, R)):
        if {l.get("diff_kind", "DIFF_NONE"), r.get("diff_kind", "DIFF_NONE")} <= {"DIFF_NONE"}:
            continue
        n += 1
        li, ri = l.get("instruction") or {}, r.get("instruction") or {}
        if li.get("formatted", "").split()[:1] != ri.get("formatted", "").split()[:1]:
            WHY.append(i); return False, n
        if _operands_sans_reloc(li) != _operands_sans_reloc(ri):
            WHY.append(i); return False, n    # registers differ too (linkcheck caught lui $v0 vs $v1)
        (tl, al), (tr, ar) = reloc_target(l), reloc_target(r)
        if not tl or not tr:
            WHY.append(i); return False, n
        if tl == tr and al == ar:
            continue                      # same symbol; differs elsewhere -> objdiff noise
        m = re.fullmatch(r"(?:D|jtbl)_([0-9A-F]{8})", tl)
        if not m:
            WHY.append(i); return False, n
        if tl.startswith("jtbl_"):
            continue                      # entries are code labels; the code itself is compared
        mr = re.fullmatch(r"D_([0-9A-F]{8})", tr)
        if mr and int(m.group(1), 16) + al == int(mr.group(1), 16) + ar:
            continue                      # two splat labels, same resolved address (array end)
        if tr.startswith(".") and "%hi(" in ri.get("formatted", ""):
            continue                      # objdiff drops %hi addends; the paired %lo row is checked
        if tr.startswith("."):            # candidate's own section data (a string literal)
            want = exe_bytes(int(m.group(1), 16) + al, 64)
            cb = section_bytes(cand, tr, ar, 64)
            if cb is None:
                WHY.append(i); return False, n
            k = want.find(b"\0")
            k = 4 if k < 0 else k + 1
            if cb[:k] != want[:k]:
                WHY.append(i); return False, n
            continue
        cb = obj_data(cand, tr)
        if cb is None or exe_bytes(int(m.group(1), 16) + al, len(cb)) != cb:
            WHY.append(i); return False, n
    return True, n


def listing(rs):
    return "\n".join((x.get("instruction") or {}).get("formatted", "") for x in rs)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("symbol")
    ap.add_argument("file")
    ap.add_argument("--cc", default="2.7.2-cdk")
    ap.add_argument("--opt", default="-O2")
    ap.add_argument("-G", default="8")
    ap.add_argument("--aspsx", default="2.79")
    ap.add_argument("--extra", default="")
    ap.add_argument("--asm", action="store_true")
    ap.add_argument("--keep-s", action="store_true")
    ap.add_argument("--diff", action="store_true", help="print only the rows that differ")
    ap.add_argument("--quiet", action="store_true", help="print only the MATCH line")
    a = ap.parse_args()
    if not sfile_of(a.symbol):
        sys.exit(f"{a.symbol}: no asm_fn/nonmatchings/main/{a.symbol}.s")
    t = target_obj(a.symbol)
    cand = os.path.join(B, "c_" + a.symbol + ".o")
    err = compile_c(os.path.abspath(a.file), cand, a)
    if err:
        print("COMPILE ERROR\n" + err)
        sys.exit(2)
    j = diff(t, cand, a.symbol)
    left, right = rows(j, "left", a.symbol), rows(j, "right", a.symbol)
    if right is None:
        print("MATCH 0.0 (candidate does not define the symbol)")
        sys.exit(0)
    pct, L = left
    _, R = right
    ndiff = sum(1 for x in L + R if x.get("diff_kind", "DIFF_NONE") != "DIFF_NONE")
    note = ""
    if ndiff == 0 and len(L) == len(R):
        pct = 100.0
    else:
        ok, _ = relaxed_equal(L, R, cand)
        if ok:
            pct, note = 100.0, " (own rodata, bytes identical)"
        else:
            pct = min(pct or 0.0, 99.99)
    print(f"MATCH {'100.0' if pct >= 100.0 else f'{pct:.2f}'}{note}  ({len(L)} vs {len(R)} instrs)")
    if a.diff and WHY:
        print(f"(relaxed check stopped at row {WHY[0]})")
    if a.diff:
        for i, (l, r) in enumerate(zip(L, R)):
            if l.get("diff_kind", "DIFF_NONE") != "DIFF_NONE" or r.get("diff_kind", "DIFF_NONE") != "DIFF_NONE":
                print(f"{i:4} {(l.get('instruction') or {}).get('formatted', ''):40} | {(r.get('instruction') or {}).get('formatted', '')}")
    if a.asm and not a.quiet:
        print("---- target\n" + listing(L) + "\n---- candidate\n" + listing(R))


if __name__ == "__main__":
    main()
