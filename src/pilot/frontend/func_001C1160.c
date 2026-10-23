extern int D_000967F4;
extern int D_000967D0;
extern int D_00096808;
extern int D_00096834;
extern char D_001CDDDC[];
extern int D_001CDE60;
extern void func_00024754();
extern void func_001C2644();

void func_001C1160(void)
{
    D_000967F4 = 7;
    D_00096834 = 0;
    if (D_000967D0 >= 4)
        D_000967D0 = 0;
    D_00096808 = D_000967D0 + 1;
    func_00024754();
    func_001C2644(D_001CDDDC, 8, D_001CDE60);
    D_000967D0++;
}
