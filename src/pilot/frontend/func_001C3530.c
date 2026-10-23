extern int D_001CDF88;
extern char D_001CDF48[], D_001CDEE8[];
extern void func_001C2644();
extern void func_001C2114();
extern void func_000514E0();
extern int func_001C26E4();
extern void func_001C19C4();
extern void func_001C1BD4();
extern void func_001C11E4();

int func_001C3530(void)
{
    int r;
    D_001CDF88 = 0;
    func_001C2644(D_001CDF48, 2, 0);
    func_001C2114(0, D_001CDEE8, 1);
    for (;;) {
        func_000514E0();
        r = func_001C26E4(D_001CDF48, &D_001CDF88);
        if (r != 0) break;
        func_001C19C4();
        func_001C1BD4(D_001CDEE8);
        func_001C11E4();
    }
    func_001C2114(0, D_001CDEE8, 0);
    return r;
}
