/* FAKEMATCH: empty __asm__ used as a compiler hint (no instruction emitted) */
extern int D_00096F04;
extern volatile int D_00096F08;
extern long long pauseflag;
void func_0006BE3C(void)
{
    int x;
    D_00096F04 = 1;
    __asm__ volatile("");
    x = D_00096F08;
    *(int *)&pauseflag = 0;
}
