/* HANDWRITTEN ASM */
/* GTE fragment: store SXY0-2 to t1, t1 += 12, t6 -= 3; t6 = 0 once it runs out (sibling of func_000268B8) */
__asm__(
    ".set noreorder\n"
    ".macro XBGTZ r,t; bgtz \\r,\\t; .endm\n"
    ".globl func_00026894\n"
    "func_00026894:\n"
    "addi $14, $14, -3\n"
    "swc2 $12, 0x0($9)\n"
    "swc2 $13, 0x4($9)\n"
    "swc2 $14, 0x8($9)\n"
    "XBGTZ $14, .L000268B0\n"
    "addi $9, $9, 0xC\n"
    "addu $14, $0, $0\n"
    ".L000268B0:\n"
    "jr $31\n"
    "nop\n"
    ".set reorder\n"
);
