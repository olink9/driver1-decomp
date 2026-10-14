extern int D_00096F04, D_00096F10;
extern volatile int D_00096F08;
extern long long D_0009709C, D_0009687C;
void func_0006BDAC(void)
{
    int x;
    *(int *)&D_0009709C = 3;
    x = D_00096F08;
    __asm__ volatile("" : : "r"(x));
    D_00096F10 = 1;
    *(int *)&D_0009687C = 0;
    D_00096F04 = 1;
}
