/* Driver 2: XM_Exit */
extern int D_00096FAC;
extern void func_00073738(int);
extern void func_0006F940(int);
void func_0006F8DC(void)
{
    int i;
    for (i = 0; i < D_00096FAC; i++)
        func_00073738(i);
    for (i = 0; i < 8; i++)
        func_0006F940(i);
}
