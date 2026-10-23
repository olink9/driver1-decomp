#!/usr/bin/env python3
"""Export the main executable's symbols for the overlay builds.

    tools/mainsyms.py [build/slus_008.42.elf]

Writes, from main's linked ELF (run `make` first):
  build/main_syms.txt  symbol_addrs format; listed first in frontend.yaml / mcard.yaml
                       so splat spells references into main exactly as main does
                       (func_00022818, not spimdisasm's out-of-segment func_22818)
  build/main_syms.ld   PROVIDE(sym = addr) for every main symbol; the overlay links use it
                       in place of --just-symbols (which would clash with the overlay entry
                       points main itself only knows as undefined_funcs_auto.txt assignments)
Symbols inside an overlay's address range (main's own references to overlay entry points,
D_001E4038 ...), local labels and linker-generated segment boundaries are left out.
Run inside distrobox `build`.
"""
import os, re, sys
from elftools.elf.elffile import ELFFile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OVERLAY_LO, OVERLAY_HI = 0xE6600, 0x200000     # MCARD 0xE6600 .. CRED/IVIEW end, below 2 MB
SEGSYM = re.compile(r"^(main|header)_|_(ROM|VRAM|TEXT|DATA|BSS|RODATA|SDATA|SBSS)_?(START|END|SIZE)$|^__romPos$|^_MACRO_INC_GUARD$")


def main():
    elf = sys.argv[1] if len(sys.argv) > 1 else os.path.join(ROOT, "build", "slus_008.42.elf")
    best = {}                                    # name -> (addr, is_section_symbol, type)
    with open(elf, "rb") as f:
        st = ELFFile(f).get_section_by_name(".symtab")
        for s in st.iter_symbols():
            n = s.name
            if not n or s["st_info"]["bind"] != "STB_GLOBAL" or "." in n or SEGSYM.search(n):
                continue
            if s["st_info"]["type"] in ("STT_FILE", "STT_SECTION"):
                continue
            a = s["st_value"]
            if OVERLAY_LO <= a < OVERLAY_HI:
                continue
            best[n] = (a, s["st_shndx"] != "SHN_ABS", s["st_info"]["type"])
    # splat wants one name per address: keep a symbol defined in a section over an
    # absolute assignment, then a real name over func_/D_, then the first by name
    byaddr = {}
    for n, (a, insec, typ) in sorted(best.items()):
        k = (insec, not re.match(r"(func|D|jtbl)_[0-9A-F]+$", n))
        if a not in byaddr or k > byaddr[a][0]:
            byaddr[a] = (k, n, typ)
    os.makedirs(os.path.join(ROOT, "build"), exist_ok=True)
    with open(os.path.join(ROOT, "build", "main_syms.txt"), "w") as f:
        for a in sorted(byaddr):
            _, n, typ = byaddr[a]
            t = " // type:func" if typ == "STT_FUNC" or n.startswith("func_") else ""
            f.write(f"{n} = 0x{a:08X};{t}\n")
    with open(os.path.join(ROOT, "build", "main_syms.ld"), "w") as f:
        for n, (a, _, _) in sorted(best.items()):
            f.write(f"PROVIDE({n} = 0x{a:X});\n")
    print(f"{len(byaddr)} addresses / {len(best)} names from {os.path.relpath(elf, ROOT)}")


if __name__ == "__main__":
    main()
