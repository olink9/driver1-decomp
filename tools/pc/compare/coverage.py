#!/usr/bin/env python3
"""Function coverage of the PC build over the lockstep sessions (port/README.md "Lockstep comparison").

  coverage.py BUILD_DIR FUNCS.txt [FUNCS.txt ...] [--list]

BUILD_DIR is a build made with -finstrument-functions (cmake -DCMAKE_C_FLAGS=-finstrument-functions); each run with
D1_FUNCS=<file> (session.sh: D1_FUNCS=1) writes the entry addresses of every function it entered (port/pc/pc_trace.c).
Reports, per group, how many functions ran: all game code (src/**), the hand-written asm rewritten in C
(port/pc/asm_*.c: the global routines), the INCLUDE_ASM wips (port/pc/wips_pc.c) and the FAKEMATCH functions (a
FAKEMATCH comment on the function in src/, or a FAKEMATCH match in pilot/results.tsv) - and lists those that never
ran (--list: every game function that never ran, per file).
"""
import glob
import os
import re
import subprocess
import sys
from collections import defaultdict

REPO = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "../../.."))


def nm_defined(path, globals_only=False):
    out = subprocess.run(["nm", "--defined-only", path], capture_output=True, text=True).stdout
    names = []
    for line in out.splitlines():
        p = line.split()
        if len(p) == 3 and p[1] in ("T", "t") and (p[1] == "T" or not globals_only):
            names.append(p[2])
    return names


def fakematch_functions():
    names = set()
    for f in glob.glob(os.path.join(REPO, "src/**/*.c"), recursive=True):
        lines = open(f, errors="replace").read().split("\n")
        for i, l in enumerate(lines):
            if "FAKEMATCH" in l and not l.lstrip().startswith("*"):
                for j in range(i, min(i + 40, len(lines))):
                    m = re.match(r"^[A-Za-z_][\w \*]*?\b(\w+)\s*\([^;]*$", lines[j])
                    if m and not lines[j].startswith(("extern", "#", "static inline")):
                        names.add(m.group(1))
                        break
    try:
        for line in open(os.path.join(REPO, "pilot/results.tsv")):
            p = line.rstrip("\n").split("\t")
            if len(p) > 6 and "FAKEMATCH" in p[6]:
                names.add(p[1])
    except OSError:
        pass
    return names


def main():
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    if len(args) < 2:
        sys.exit(__doc__)
    build, funcs = args[0], args[1:]
    exe = os.path.join(build, "driver1")
    objdir = os.path.join(build, "CMakeFiles/d1obj.dir")
    addr2name = {}
    out = subprocess.run(["nm", exe], capture_output=True, text=True).stdout
    for line in out.splitlines():
        p = line.split()
        if len(p) == 3 and p[1] in "Tt":
            addr2name.setdefault(int(p[0], 16), p[2])
    ran = set()
    for f in funcs:
        for line in open(f):
            a = int(line, 16)
            if a in addr2name:
                ran.add(addr2name[a])

    # game functions per source file (objects of src/** = <objdir>/<rel path from port>/../src/..)
    per_file = defaultdict(list)
    for o in glob.glob(os.path.join(objdir, "**/*.o"), recursive=True):
        rel = os.path.relpath(o, objdir)
        if "/src/" not in "/" + rel and not rel.startswith("pc/"):
            continue
        per_file[rel[:-2]] = nm_defined(o)

    def group(title, names):
        names = sorted(set(names))
        miss = [n for n in names if n not in ran]
        print(f"{title}: {len(names) - len(miss)}/{len(names)} ran")
        return miss

    game = [n for f, ns in per_file.items() if not f.startswith("pc/") for n in ns]
    group("game code (src/**)", game)
    # the 58 routines: each asm_*.c translation is a static F_<addr> (the global func_<addr> wrappers, the
    # dispatchers and second entry points all end up in it)
    asm_f = {}
    for f in per_file:
        if re.search(r"pc/asm_\w+\.c$", f):
            asm_f[f] = [n for n in nm_defined(os.path.join(objdir, f + ".o")) if re.match(r"F_[0-9A-F]+$", n)]
    allasm = [n for ns in asm_f.values() for n in ns]
    miss = group("port/pc/asm_*.c routines (the F_<addr> translations)", allasm)
    for f in sorted(asm_f):
        m = ["func_%08X" % int(n[2:], 16) for n in sorted(asm_f[f]) if n in miss]
        if m:
            print(f"    {os.path.basename(f)}: not run: {' '.join(m)}")
    wips = [n for f, ns in per_file.items() if f.endswith("pc/wips_pc.c")
            for n in nm_defined(os.path.join(objdir, f + ".o"), globals_only=True)]
    miss = group("port/pc/wips_pc.c (global)", wips)
    if miss:
        print("    not run: " + " ".join(miss))
    fm = fakematch_functions()
    defined = set(game) | set(allasm) | set(wips)
    fm_def = sorted(n for n in fm if n in defined)
    miss = group(f"FAKEMATCH functions ({len(fm)} tagged, {len(fm_def)} defined on PC)", fm_def)
    if miss:
        print("    not run: " + " ".join(miss))
    if "--list" in sys.argv:
        print("\nnever ran, per file:")
        for f in sorted(per_file):
            if f.startswith("pc/"):
                continue
            m = [n for n in sorted(per_file[f]) if n not in ran]
            if m:
                print(f"  {f}: {len(m)}/{len(per_file[f])}: " + " ".join(m))


if __name__ == "__main__":
    main()
