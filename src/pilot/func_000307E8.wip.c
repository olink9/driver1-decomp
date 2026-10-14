/* HANDWRITTEN ASM */
__asm__(
    ".set noreorder\n"
    ".globl func_000307E8\n"
    "func_000307E8:\n"
    "lui $8, 0x40\n"
    "ori $8, $8, 0x8080\n"
    "mtc2 $8, $23\n"
    "j .L00030520\n"
    "nop\n"
    ".set reorder\n"
);
