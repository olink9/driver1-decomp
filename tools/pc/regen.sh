#!/bin/sh
# regenerate port/pc/asm_*.c from src/*_asm.s (run from anywhere)
cd "$(dirname "$0")"
R=$(cd ../.. && pwd)
P=$R/port/pc
S=$R/src
python3 - "$S" <<'PY'
import sys; s=open(sys.argv[1]+"/unpack_asm.s").read()
old="""    lui $t3, 0x2
    ori $t3, $t3, 0x6790
"""
assert old in s
s=s.replace(old,"""    lui $t3, %hi(D_00026790)
    addiu $t3, $t3, %lo(D_00026790)
""")
open('unpack_pc.s','w').write(s)
PY
gen() { python3 mips2c.py "$1" $2 --head tmpl/$2.head --foot tmpl/$2.foot --abi "$3" $4 > $P/asm_$2.c || exit 1; }
gen $S/gte_asm.s gte func_0002C53C,func_0002C564,func_0002C598,func_0002C5F0,func_0002C6DC
gen $S/tile_asm.s tile func_00026794,func_0002680C,func_000268FC,func_000269D4,func_00026FF0,func_00027098 "--entries func_00026F28:0x10,func_00026F28:0x20"
gen $S/mdraw_asm.s mdraw func_00030ED0 "--alias L00030520=func_00030474:0xAC"
gen $S/carpoly_asm.s carpoly func_0003DF94,func_0003E244
gen $S/rle_asm.s rle func_00035B5C
gen unpack_pc.s unpack func_00026140
