/* HANDWRITTEN ASM */
/* table lookup helper: index in $t2, table base in $s6 -> signed halfwords in $t8/$t9 (non-ABI registers) */
__asm__(
    ".set noreorder\n"
    ".globl func_00026BB8\n"
    "func_00026BB8:\n"
    "srl $25, $10, 2\n"
    "andi $25, $25, 0x3FC0\n"
    "srl $24, $10, 15\n"
    "andi $24, $24, 0xFE\n"
    "add $24, $24, $25\n"
    "add $24, $24, $22\n"
    "srl $25, $25, 5\n"
    "add $25, $25, $22\n"
    "lh $24, 0x100($24)\n"
    "lh $25, 0($25)\n"
    "jr $31\n"
    "nop\n"
    ".set reorder\n");
