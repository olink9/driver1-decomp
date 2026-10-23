extern int D_001CEDF4;
extern char D_001CED84[], D_001CECD4[];
extern void func_001C80EC();
extern void func_001C2644();
extern void func_001C2114();
extern void func_000514E0();
extern int func_001C26E4();
extern void func_001C19C4();
extern void func_001C1BD4();
extern void func_001C11E4();

int func_001C8018(void)
{
    int r;
    func_001C80EC();
    D_001CEDF4 = 0;
    func_001C2644(D_001CED84, 3, 0);
    func_001C2114(0, D_001CECD4, 1);
    for (;;) {
        func_000514E0();
        r = func_001C26E4(D_001CED84, &D_001CEDF4);
        if (r != 0) break;
        func_001C19C4();
        func_001C1BD4(D_001CECD4);
        func_001C11E4();
    }
    func_001C2114(0, D_001CECD4, 0);
    if (r == -1) return 0;
    return r;
}
