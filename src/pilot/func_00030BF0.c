/* HANDWRITTEN ASM */
/* table lookup helper: index in $t2, table base in $s6 -> unsigned halfwords in $t5/$t4 (non-ABI registers) */
__asm__(
    ".set noreorder\n"
    ".globl func_00030BF0\n"
    "func_00030BF0:\n"
    "srl $12, $10, 2\n"
    "andi $12, $12, 0x3FC0\n"
    "srl $13, $10, 15\n"
    "andi $13, $13, 0xFE\n"
    "add $13, $13, $12\n"
    "add $13, $13, $22\n"
    "srl $12, $12, 5\n"
    "add $12, $12, $22\n"
    "lhu $13, 0x100($13)\n"
    "lhu $12, 0($12)\n"
    "jr $31\n"
    "nop\n"
    ".set reorder\n");
