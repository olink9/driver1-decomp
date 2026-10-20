/* HANDWRITTEN ASM */
/* stores t3,t4,s1,s2,s4,s3 to a1 and advances a1 by 0x18 */
__asm__(
    ".set noreorder\n"
    ".globl func_00026C18\n"
    "func_00026C18:\n"
    "sw $11, 0x0($5)\n"
    "sw $12, 0x4($5)\n"
    "sw $17, 0x8($5)\n"
    "sw $18, 0xC($5)\n"
    "sw $20, 0x10($5)\n"
    "sw $19, 0x14($5)\n"
    "jr $31\n"
    "addi $5, $5, 0x18\n"
    ".set reorder\n"
);
