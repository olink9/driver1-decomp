extern int D_001CEA38;
extern char D_001CEA18[], D_001CE9E8[];
extern void func_001C2644();
extern void func_001C2114();
extern void func_000514E0();
extern int func_001C26E4();
extern void func_001C19C4();
extern void func_001C1BD4();
extern void func_001C11E4();

int func_001C6DE0(void)
{
    int r;
    D_001CEA38 = 0;
    func_001C2644(D_001CEA18, 2, 0);
    func_001C2114(0, D_001CE9E8, 1);
    for (;;) {
        func_000514E0();
        r = func_001C26E4(D_001CEA18, &D_001CEA38);
        if (r != 0) break;
        func_001C19C4();
        func_001C1BD4(D_001CE9E8);
        func_001C11E4();
    }
    func_001C2114(0, D_001CE9E8, 0);
    return r;
}
