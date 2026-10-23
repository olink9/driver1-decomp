extern int D_001D2DC8, D_001CE3D4, D_000967D4;
extern char D_001CE408[], D_001CE3D8[];
extern void func_001C2644();
extern void func_001C2114();
extern void func_000514E0();
extern int func_001C26E4();
extern void func_001C19C4();
extern void func_001C1BD4();
extern void func_001C11E4();

int func_001C5968(void)
{
    int r;
    D_001D2DC8 = 0;
    func_001C2644(D_001CE408, 2, 0);
    func_001C2114(0, D_001CE3D8, 1);
    for (;;) {
        func_000514E0();
        D_001CE3D4 = D_001D2DC8;
        r = func_001C26E4(D_001CE408, &D_001D2DC8);
        if (r != 0) break;
        func_001C19C4();
        func_001C1BD4(D_001CE3D8);
        func_001C11E4();
    }
    if (r == -1) {
        D_000967D4 = 0;
        return 0;
    }
    return r;
}
