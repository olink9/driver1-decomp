extern int D_00096870, D_00096784, D_000967EC, D_001D2E28, D_001D2E2C, D_001D2E30, D_001CE88C;
extern char D_001CE84C[], D_001CE7A8[], D_001CE7BC[];
extern void func_001C66B4();
extern void func_001C2644();
extern void func_001C2114();
extern void func_000514E0();
extern int func_001C26E4();
extern void func_001C19C4();
extern void func_001C1BD4();
extern void func_001C11E4();

int func_001C6438(void)
{
    int r;
    D_001D2E28 = D_00096870;
    D_001D2E2C = D_00096784;
    D_001D2E30 = D_000967EC;
    func_001C66B4();
    D_001CE88C = 0;
    func_001C2644(D_001CE84C, 4, 0);
    func_001C2114(D_001CE7A8, D_001CE7BC, 1);
    for (;;) {
        func_000514E0();
        r = func_001C26E4(D_001CE84C, &D_001CE88C);
        if (r != 0) break;
        func_001C19C4();
        func_001C1BD4(D_001CE7BC);
        func_001C11E4();
    }
    if (r != -1) return 0;
    D_00096870 = D_001D2E28;
    D_00096784 = D_001D2E2C;
    D_000967EC = D_001D2E30;
    return 0;
}
