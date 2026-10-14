/* FAKEMATCH: empty __asm__ used as a compiler hint (no instruction emitted) */
extern int D_00096F04, D_00096F10;
extern volatile int D_00096F08;
extern long long D_000967CC, D_0009687C;
void func_0006BE14(void)
{
    int x;
    D_00096F04 = 1;
    *(int *)&D_000967CC = 1;
    D_00096F10 = 1;
    __asm__ volatile("");
    x = D_00096F08;
    *(int *)&D_0009687C = 0;
}
