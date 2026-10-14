/* HANDWRITTEN ASM */
void func_0002680C(int *v, int b, int c) __attribute__((noreturn));
void func_0002680C(int *v, int b, int c) {
    __asm__ volatile (
        ".set noreorder;"
        "lw $8, 0(%0);" "lw $9, 4(%0);" "lw $10, 8(%0);"
        "cfc2 $11, $21;" "cfc2 $12, $22;" "cfc2 $13, $23;"
        "sub $8, $8, $11;" "sub $9, $9, $12;" "sub $10, $10, $13;"
        "mtc2 $8, $9;" "mtc2 $9, $10;" "mtc2 $10, $11;"
        "nop;" "nop;"
        "mvmva 1, 2, 3, 3, 0;"
        "nop;" "nop;"
        "mfc2 $8, $9;" "mfc2 $9, $10;" "mfc2 $2, $11;"
        "ctc2 $8, $5;" "ctc2 $9, $6;"
        "jr $31;"
        "ctc2 $2, $7;"
        ".set reorder"
        : "+r"(v) : : "$2", "$8", "$9", "$10", "$11", "$12", "$13");
}
