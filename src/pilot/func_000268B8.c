/* HANDWRITTEN ASM */
/* GTE fragment: store SXY0-2 to t1, t6 -= 3, loop into func_000268D8 while t6 > 0 */
__asm__(
    ".set noreorder\n"
    ".macro XBGTZ r,t; bgtz \\r,\\t; .endm\n"
    ".globl func_000268B8\n"
    "func_000268B8:\n"
    "addi $14, $14, -3\n"
    "swc2 $12, 0x0($9)\n"
    "swc2 $13, 0x4($9)\n"
    "swc2 $14, 0x8($9)\n"
    "XBGTZ $14, func_000268D8\n"
    "addi $9, $9, 0xC\n"
    "jr $31\n"
    "addiu $14, $0, -1\n"
    ".set reorder\n"
);
