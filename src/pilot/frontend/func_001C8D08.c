extern int D_001CEF28, D_001CEF2C;
extern char D_001CEED8[], D_001CEE68[];
extern void func_001C2644();
extern void func_001C2114();
extern void func_000514E0();
extern int func_001C26E4();
extern void func_001C19C4();
extern void func_001C1BD4();
extern void func_001C11E4();

int func_001C8D08(void)
{
    int r;
    D_001CEF28 = 0;
    func_001C2644(D_001CEED8, 5, 0);
    func_001C2114(0, D_001CEE68, 1);
    for (;;) {
        func_000514E0();
        D_001CEF2C = D_001CEF28;
        r = func_001C26E4(D_001CEED8, &D_001CEF28);
        if (r != 0) break;
        func_001C19C4();
        func_001C1BD4(D_001CEE68);
        func_001C11E4();
    }
    if (r == -1) return 0;
    return r;
}
