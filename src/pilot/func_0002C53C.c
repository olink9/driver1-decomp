/* HANDWRITTEN ASM */
__asm__(
    ".set noreorder\n"
    ".globl func_0002C53C\n"
    "func_0002C53C:\n"
    "lui $8, %hi(D_0008A8F0)\n"
    "lw $8, %lo(D_0008A8F0)($8)\n"
    "lui $9, %hi(D_0008A8F4)\n"
    "lw $9, %lo(D_0008A8F4)($9)\n"
    "lui $10, %hi(D_0008A8F8)\n"
    "lw $10, %lo(D_0008A8F8)($10)\n"
    "ctc2 $8, $21\n"
    "ctc2 $9, $22\n"
    "jr $31\n"
    "ctc2 $10, $23\n"
    ".set reorder\n"
);
