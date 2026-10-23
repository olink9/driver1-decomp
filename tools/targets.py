#!/usr/bin/env python3
"""The binaries this repo rebuilds, one entry each (main executable + code overlays).

Used by tools/fnscore.py, tools/linkcheck.py, tools/permsetup.py, tools/mkqueue.py and
tools/progress.py via `--target NAME`. Paths are relative to the repo root.

  orig      the original file (orig/ is gitignored game data)
  sha1      of that file
  vram      load address of the first byte that is code/data (main: after the 0x800 header)
  fileoff   file offset of `vram`
  ldscript  splat's linker script for the baseline build
  asmfiles  {asm file: object name used in ldscript} - what `make` assembles for it
  text_asm  the asm file holding .text (one nonmatching block per function)
  undef     symbol scripts passed to ld (-T) before the ldscript
  fn        per-function asm (scoring targets), from fn.yaml / fn_<target>.yaml
  src       where matched C lives (src/pilot/<target>/ for overlays)
  queue     work queue (size, addr, symbol, ...)
  linkcheck default TSV for linkcheck results
  G         cc1/maspsx -G (main -G8; the overlays have no $gp access at all: -G0)
  extern_g  MASPSX_EXTERN_G default (externs of <= N bytes are $gp-relative)
  bss_lo    lowest address C .bss may be placed at (symbol name or int); bss_hi upper bound
  lib_start functions at/above this address are PsyQ library code (not queued)
"""
import os

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

_OVL_UNDEF = lambda n: ["build/main_syms.ld", f"undefined_syms_auto.{n}.txt", f"undefined_funcs_auto.{n}.txt"]


def _overlay(name, binname, sha1, vram, end):
    return dict(
        name=name, orig=f"orig/{binname}", sha1=sha1, vram=vram, fileoff=0,
        ldscript=f"{name}.ld",
        asmfiles={f"asm/{name}/{name}.s": f"build/{name}/asm/{name}/{name}.o",
                  f"asm/{name}/data/{name}_rodata.rodata.s": f"build/{name}/asm/{name}/data/{name}_rodata.rodata.o",
                  f"asm/{name}/data/{name}_data.data.s": f"build/{name}/asm/{name}/data/{name}_data.data.o",
                  f"asm/{name}/data/{name}_tail.data.s": f"build/{name}/asm/{name}/data/{name}_tail.data.o"},
        text_asm=f"asm/{name}/{name}.s",
        undef=_OVL_UNDEF(name),
        fn=f"asm_fn/nonmatchings/{name}", src=f"src/pilot/{name}",
        queue=f"pilot/queue-{name}.tsv", linkcheck=f"pilot/linkcheck-{name}.tsv",
        G="0", extern_g="0",
        # the zero tail of the file is the overlay's bss (kept as data bytes for now)
        bss_lo=f"{name}_DATA_START", bss_hi=end, lib_start=None,
        make=f"build/{binname}",
    )


TARGETS = {
    "main": dict(
        name="main", orig="orig/SLUS_008.42", sha1="94b44a84de2f9fa2dcaf120db2e7b3d74bc70d3d",
        vram=0x10000, fileoff=0x800, ldscript="slus_008.42.ld",
        asmfiles={"asm/2C8C.s": "build/asm/2C8C.o", "asm/data/800.rodata.s": "build/asm/data/800.rodata.o",
                  "asm/data/75B14.data.s": "build/asm/data/75B14.data.o",
                  "asm/data/87800.bss.s": "build/asm/data/87800.bss.o",
                  "asm/header.s": "build/asm/header.o"},
        text_asm="asm/2C8C.s",
        undef=["undefined_syms_auto.txt", "undefined_syms_manual.txt", "undefined_funcs_auto.txt"],
        fn="asm_fn/nonmatchings/main", src="src/pilot",
        queue="pilot/queue.tsv", linkcheck="pilot/linkcheck.tsv",
        G="8", extern_g="4",
        bss_lo="main_BSS_START", bss_hi=0x200000, lib_start=0x738F0,
        make="build/SLUS_008.42",
    ),
    "frontend": _overlay("frontend", "FRONTEND.BIN", "62f9e054eccd83f0aa688b6e1b5aad5693f35849", 0x1C0000, 0x1D3308),
    "mcard": _overlay("mcard", "MCARD.BIN", "d25d51e8bf2226168551f46da83b238f7a62754b", 0xE6600, 0xF2A88),
    # CRED and IVIEW both load at 0x1E0000 and are never resident together: own symbol files,
    # neither config includes the other's (their func_ names do not collide)
    "cred": _overlay("cred", "CRED.BIN", "ea5a8e516f3642789131882aa5a2d17edd09ad4d", 0x1E0000, 0x1F3878),
    "iview": _overlay("iview", "IVIEW.BIN", "8ef018b016a0fd9679c87659ef16433fac2d26da", 0x1E0000, 0x1E42E0),
}
NAMES = list(TARGETS)


def get(name):
    if name not in TARGETS:
        raise SystemExit(f"unknown target {name!r} (one of {', '.join(NAMES)})")
    return TARGETS[name]


def path(t, key):
    return os.path.join(ROOT, t[key])


def of_symbol(sym, default="main"):
    """The target whose per-function asm has SYM.s (names are address-based, unique
    across main and the overlays; CRED/IVIEW checked to be disjoint); `default` if none has it."""
    for n, t in TARGETS.items():
        if os.path.exists(os.path.join(ROOT, t["fn"], sym + ".s")):
            return n
    return default


def file_bytes(t, addr, n):
    """n bytes of the original file of target t at address addr."""
    with open(path(t, "orig"), "rb") as f:
        off = t["fileoff"] + addr - t["vram"]
        if off < 0:
            return b""
        f.seek(off)
        return f.read(n)
