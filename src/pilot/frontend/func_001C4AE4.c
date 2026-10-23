extern int D_001CDE5C, D_001D2DB8, D_001D2DC0;
extern char D_001CE0CC[], D_001CE0FC[];
extern void func_001C2644();
extern void func_001C2114();
extern void func_000514E0();
extern int func_001C26E4();
extern void func_001C19C4();
extern void func_001C1BD4();
extern void func_001C11E4();

int func_001C4AE4(void)
{
    int r;
    D_001D2DC0 = 0;
    func_001C2644(D_001CE0FC, 2, 0);
    D_001CDE5C = 6;
    func_001C2114(0, D_001CE0CC, 1);
    D_001D2DB8 = -1;
    for (;;) {
        func_000514E0();
        D_001D2DB8 = D_001D2DC0;
        r = func_001C26E4(D_001CE0FC, &D_001D2DC0);
        if (r != 0) break;
        func_001C19C4();
        func_001C1BD4(D_001CE0CC);
        func_001C11E4();
    }
    D_001CDE5C = 2;
    if (r == -1) return 0;
    return r;
}
