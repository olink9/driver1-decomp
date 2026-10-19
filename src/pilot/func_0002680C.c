/* HANDWRITTEN ASM */
/* GTE: subtract translation (cfc2 21-23) from a vector, rotate by mvmva, store result as new TR (ctc2 5-7) */
__asm__(
    ".set noreorder\n"
    ".globl func_0002680C\n"
    "func_0002680C:\n"
    "lw $8, 0($4)\n" "lw $9, 4($4)\n" "lw $10, 8($4)\n"
    "cfc2 $11, $21\n" "cfc2 $12, $22\n" "cfc2 $13, $23\n"
    "sub $8, $8, $11\n" "sub $9, $9, $12\n" "sub $10, $10, $13\n"
    "mtc2 $8, $9\n" "mtc2 $9, $10\n" "mtc2 $10, $11\n"
    "nop\n" "nop\n"
    ".word 0x4A4DE012\n"
    "nop\n" "nop\n"
    "mfc2 $8, $9\n" "mfc2 $9, $10\n" "mfc2 $2, $11\n"
    "ctc2 $8, $5\n" "ctc2 $9, $6\n"
    "jr $31\n"
    "ctc2 $2, $7\n"
    ".set reorder\n");
