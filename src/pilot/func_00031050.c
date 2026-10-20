/* HANDWRITTEN ASM */
/* GTE fragment: store SXY0-2 to s0, s0 += 12, t7 -= 3; t7 = -1 once it runs out */
__asm__(
    ".set noreorder\n"
    ".macro XBGTZ r,t; bgtz \\r,\\t; .endm\n"
    ".globl func_00031050\n"
    "func_00031050:\n"
    "addi $15, $15, -3\n"
    "swc2 $12, 0x0($16)\n"
    "swc2 $13, 0x4($16)\n"
    "swc2 $14, 0x8($16)\n"
    "XBGTZ $15, .L0003106C\n"
    "addi $16, $16, 0xC\n"
    "addiu $15, $0, -1\n"
    ".L0003106C:\n"
    "jr $31\n"
    "nop\n"
    ".set reorder\n"
);
