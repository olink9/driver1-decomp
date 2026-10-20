/* HANDWRITTEN ASM */
/* table lookup helper: angle in $t5, table base in $t6 -> $t8 = tbl[0x100/2 + ...], $t9 = tbl[...] (non-ABI registers) */
__asm__(
    ".set noreorder\n"
    ".globl func_0002C6B0\n"
    "func_0002C6B0:\n"
    "srl $25, $13, 2\n"
    "andi $25, $25, 0x3FC0\n"
    "srl $24, $13, 15\n"
    "andi $24, $24, 0xFE\n"
    "add $24, $24, $25\n"
    "add $24, $24, $14\n"
    "lhu $24, 0x100($24)\n"
    "srl $25, $25, 5\n"
    "add $25, $25, $14\n"
    "jr $31\n"
    "lhu $25, 0($25)\n"
    ".set reorder\n");
