/* HANDWRITTEN ASM */
/* GTE helper fragment: loads four words from a3 into t2-t5, advances a3 by 0x14, falls through (no jr) */
__asm__(
    ".set noreorder\n"
    ".globl func_00026BA4\n"
    "func_00026BA4:\n"
    "lw $10, 0x0($7)\n"
    "lw $11, 0x4($7)\n"
    "lw $12, 0x8($7)\n"
    "lw $13, 0xC($7)\n"
    "addi $7, $7, 0x14\n"
    ".set reorder\n"
);
