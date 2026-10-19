/* HANDWRITTEN ASM */
/* GTE fragment: load 3 SVECTORs (VXY0..VZ2) from t0, t0 += 24, rtpt in the jr delay slot */
__asm__(
    ".set noreorder\n"
    ".globl func_000268D8\n"
    "func_000268D8:\n"
    "lwc2 $0, 0x0($8)\n"
    "lwc2 $1, 0x4($8)\n"
    "lwc2 $2, 0x8($8)\n"
    "lwc2 $3, 0xC($8)\n"
    "lwc2 $4, 0x10($8)\n"
    "lwc2 $5, 0x14($8)\n"
    "addi $8, $8, 0x18\n"
    "jr $31\n"
    ".word 0x4A280030\n" /* rtpt */
    ".set reorder\n"
);
