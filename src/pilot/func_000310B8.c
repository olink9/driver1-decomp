/* HANDWRITTEN ASM */
/* Handwritten GTE function (matrix load from a3, MVMVA in the jr delay slot). */
__asm__(
    ".text\n"
    ".set noreorder\n"
    ".globl func_000310B8\n"
    "func_000310B8:\n"
    "lw $8, 0($7)\n" "lw $9, 4($7)\n" "lw $10, 8($7)\n" "lw $11, 12($7)\n"
    "lw $12, 16($7)\n" "lw $13, 20($7)\n"
    "ctc2 $8, $8\n" "ctc2 $9, $9\n" "ctc2 $10, $10\n" "ctc2 $11, $11\n"
    "ctc2 $12, $12\n" "sra $12, $12, 16\n" "ctc2 $12, $13\n"
    "sra $12, $13, 16\n" "ctc2 $12, $15\n"
    "sll $13, $13, 16\n" "sra $13, $13, 16\n" "ctc2 $13, $14\n"
    "addi $7, $7, 24\n"
    "jr $31\n"
    "mvmva 1, 1, 0, 1, 0\n"
    ".set reorder\n"
);
