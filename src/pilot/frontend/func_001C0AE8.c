extern int D_000967C8, D_00096C90, D_001CDE60, D_001CDE68, D_001CFDB0;
extern char D_001CDDDC[], D_001CDCD8[], D_001CDC84[];
extern void func_00073F88(), SetDispMask(), func_001C2644(), func_0006F5CC(), func_0006F478();
extern void func_000514E0(), func_001C2114(), func_001C18AC(), func_001C1BD4(), func_001C11E4(), func_001C1160();
extern int func_001C1B00(), func_001C26E4();

void func_001C0AE8(void)
{
    int r;

    D_000967C8 = 1;
    func_00073F88(0);
    D_001CDE68 = 0;
    SetDispMask(0);
    if (func_001C1B00() == 0)
        return;
    func_001C2644(D_001CDDDC, 8, D_001CDE60);
    SetDispMask(0);
    for (;;) {
        if (D_001CFDB0 == 2) {
            func_0006F5CC(8, 4, 7);
            func_0006F478(D_00096C90);
            SetDispMask(1);
        }
        func_000514E0();
        r = func_001C26E4(D_001CDDDC, &D_001CDE60);
        if (r == -1) {
            D_001CDE68 = 0;
            func_001C2644(D_001CDDDC, 8, D_001CDE60);
        } else if (r > 0) {
            D_001CDE60 = r - 1;
            func_001C2644(D_001CDDDC, 8, r - 1);
            func_001C2114(D_001CDCD8, 0, 1);
            func_0006F5CC(8, 4, 7);
            func_0006F478(D_00096C90);
        } else if (r == -2) {
            D_001CDE68 = 0;
            D_001CFDB0 = 0;
        }
        func_001C18AC();
        func_001C1BD4(D_001CDC84);
        func_001C11E4();
        func_001C18AC();
        func_001C1BD4(D_001CDC84);
        func_001C11E4();
        if (D_001CDE68 >= 1500)
            func_001C1160();
        D_001CDE68++;
        D_001CFDB0++;
    }
}
