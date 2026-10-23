extern int D_00096F04, D_00096F10;
extern int D_00096F08;
extern long long D_0009709C, pauseflag;
int func_0006BDAC(void)
{
    int x;
    *(int *)&D_0009709C = 3;
    x = D_00096F08;
    D_00096F10 = 1;
    *(int *)&pauseflag = 0;
    D_00096F04 = 1;
    return x;
}
