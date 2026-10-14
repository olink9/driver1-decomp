/* HANDWRITTEN ASM */
__asm__(
    ".set noreorder\n"
    ".globl func_0002C694\n"
    "func_0002C694:\n"
    "lwc2 $0, 0($3)\n"
    "lwc2 $1, 4($3)\n"
    "lwc2 $2, 8($3)\n"
    "lwc2 $3, 12($3)\n"
    "lwc2 $4, 16($3)\n"
    "jr $31\n"
    "lwc2 $5, 20($3)\n"
    ".set reorder\n"
);
