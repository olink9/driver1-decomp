/* HANDWRITTEN ASM */
__asm__(
    ".set noreorder\n"
    ".globl func_00030464\n"
    "func_00030464:\n"
    "ori $8, $0, 0x8080\n"
    "mtc2 $8, $23\n"
    "j .L00030520\n"
    "nop\n"
    ".set reorder\n"
);
