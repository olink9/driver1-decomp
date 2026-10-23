extern long long D_00096874, pauseflag, D_0009709C;
extern int D_00096F04, D_00096F08;
int func_0006BDD8(void) {
    if (*(int *)&D_00096874 == 0)
        *(int *)&D_0009709C = 3;
    else
        *(int *)&D_0009709C = 6;
    D_00096F04 = 1;
    *(int *)&pauseflag = 0;
    return D_00096F08;
}
