/* HANDWRITTEN ASM */
/* GTE fragment: load 3 SVECTORs (VXY0..VZ2) from a0, a0 += 24, rtpt in the jr delay slot */
__asm__(
    ".set noreorder\n"
    ".globl func_00031094\n"
    "func_00031094:\n"
    "lwc2 $0, 0x0($4)\n"
    "lwc2 $1, 0x4($4)\n"
    "lwc2 $2, 0x8($4)\n"
    "lwc2 $3, 0xC($4)\n"
    "lwc2 $4, 0x10($4)\n"
    "lwc2 $5, 0x14($4)\n"
    "addi $4, $4, 0x18\n"
    "jr $31\n"
    ".word 0x4A280030\n" /* rtpt */
    ".set reorder\n"
);
