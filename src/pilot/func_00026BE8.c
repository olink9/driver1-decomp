/* HANDWRITTEN ASM */
/* store GTE SXY0-2 / SXYP plus caller s-/t-registers as a 0x28-byte record at $a1, advance $a1 (non-ABI) */
__asm__(
    ".set noreorder\n"
    ".globl func_00026BE8\n"
    "func_00026BE8:\n"
    "swc2 $20, 0($5)\n"
    "swc2 $21, 4($5)\n"
    "sw $17, 8($5)\n"
    "swc2 $22, 0xC($5)\n"
    "sw $20, 0x10($5)\n"
    "swc2 $23, 0x14($5)\n"
    "sw $18, 0x18($5)\n"
    "sw $12, 0x1C($5)\n"
    "sw $19, 0x20($5)\n"
    "sw $13, 0x24($5)\n"
    "jr $31\n"
    "addi $5, $5, 0x28\n"
    ".set reorder\n");
