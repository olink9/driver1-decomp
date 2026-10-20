/* HANDWRITTEN ASM */
/* GTE fragment: store SXY0-2 to s0, t7 -= 3, loop into func_00031094 while t7 > 0 */
__asm__(
    ".set noreorder\n"
    ".macro XBGTZ r,t; bgtz \\r,\\t; .endm\n"
    ".globl func_00031074\n"
    "func_00031074:\n"
    "addi $15, $15, -3\n"
    "swc2 $12, 0x0($16)\n"
    "swc2 $13, 0x4($16)\n"
    "swc2 $14, 0x8($16)\n"
    "XBGTZ $15, func_00031094\n"
    "addi $16, $16, 0xC\n"
    "jr $31\n"
    "addiu $15, $0, -1\n"
    ".set reorder\n"
);
