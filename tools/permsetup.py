#!/usr/bin/env python3
"""Set up a decomp-permuter directory for one symbol (run 3, 2026-10-22).

    distrobox enter build -- .venv/bin/python tools/permsetup.py [--target T] SYMBOL [src/pilot/SYMBOL.wip.c]

--target (main/frontend/mcard, default: whichever has the symbol's asm) picks the -G flags
and the asm dir; an overlay's default wip is src/pilot/<target>/SYMBOL.wip.c.

Creates perm/SYMBOL/ with base.c (the wip, preprocessed, line markers stripped), target.o (the
function's original asm), compile.sh (cc1 -> maspsx -> as with exactly fnscore's flags) and
settings.toml. Then run the permuter (from the repo root, inside the container):

    distrobox enter build -- .venv/bin/python ~/dev/decomp-permuter/permuter.py perm/SYMBOL -j4 --stop-on-zero

Better candidates land in perm/SYMBOL/output-<score>-<n>/ (source.c + diff.txt). Score 0 = the
permuter's objdump compare is identical; confirm with fnscore before trusting it (relocs, rodata).
"""
import os, re, shutil, stat, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
import fnscore
import targets

COMPILE = r"""#!/bin/bash
# decomp-permuter compile script: $1 = input .c (already preprocessed), -o $3 = output .o
set -e
ROOT=%(root)s
IN="$1"; OUT="$3"
T=$(mktemp -d); trap 'rm -rf "$T"' EXIT
"$ROOT/compilers/gcc-2.7.2-cdk/cc1" -O2 -G%(G)s -mips1 -mcpu=3000 -fgnu-linker -mgas -msoft-float -quiet \
    -funsigned-char -w %(extra)s "$IN" -o "$T/x.s"
MASPSX_EXTERN_G=%(eg)s MASPSX_GP_ALLOW_OFFSET=0 %(py)s "$ROOT/tools/maspsx/maspsx.py" --aspsx-version=2.79 \
    --expand-div -G%(G)s < "$T/x.s" > "$T/m.body"
{ echo '.include "macro.inc"'; cat "$T/m.body"; } > "$T/m.s"
cd "$ROOT" && mips-linux-gnu-as -EL -Iinclude -march=r3000 -mtune=r3000 -no-pad-sections -G0 -o "$OUT" "$T/m.s"
"""


def main():
    args = sys.argv[1:]
    tname = None
    if args[:1] == ["--target"]:
        tname, args = args[1], args[2:]
    sym = args[0]
    tname = tname or targets.of_symbol(sym)
    t = targets.get(tname)
    fnscore.set_target(tname)
    src = args[1] if len(args) > 1 else os.path.join(ROOT, t["src"], sym + ".wip.c")
    extra = os.environ.get("PERM_EXTRA", "")
    d = os.path.join(ROOT, "perm", sym)
    if os.path.exists(d):
        shutil.rmtree(d)
    os.makedirs(d)
    i = os.path.join(d, "pre.i")
    r = subprocess.run(fnscore.CPP + [os.path.abspath(src), i], cwd=ROOT, capture_output=True, text=True)
    if r.returncode:
        sys.exit(r.stderr)
    code = "".join(l for l in open(i) if not l.startswith("#"))
    os.remove(i)
    open(os.path.join(d, "base.c"), "w").write(code)
    shutil.copy(fnscore.target_obj(sym), os.path.join(d, "target.o"))
    cc1 = os.path.join(ROOT, "compilers", "gcc-2.7.2-cdk", "cc1")
    if not os.path.exists(cc1):
        sys.exit("no cc1 at " + cc1)
    p = os.path.join(d, "compile.sh")
    open(p, "w").write(COMPILE % {"root": ROOT, "py": sys.executable, "extra": extra,
                                "G": t["G"], "eg": os.environ.get("MASPSX_EXTERN_G", t["extern_g"])})
    os.chmod(p, os.stat(p).st_mode | stat.S_IEXEC)
    open(os.path.join(d, "settings.toml"), "w").write(
        'func_name = "%s"\ncompiler_type = "gcc"\n' % sym)
    print(d)


if __name__ == "__main__":
    main()
