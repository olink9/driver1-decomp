extern char D_001CE9E0;
extern int D_001CE9D4;
extern int D_00096C90, D_00096C94, D_001D2E38, D_001D2E3C;
extern char D_001CE95C[], D_001CE8A8[], D_001CE90C[];
extern void func_001C1C4C();
extern void func_001C6990();
extern void func_001C2644();
extern void func_001C2114();
extern void func_000514E0();
extern int func_001C26E4();
extern void func_001C19C4();
extern void func_001C1F40();
extern void func_001C1BD4();
extern void func_001C11E4();
extern void func_0006F478();
extern void SetMasterVolume();

int func_001C6748(void)
{
    int r;
    D_001CE9E0 = 1;
    func_001C1C4C();
    func_001C6990();
    D_001CE9E0 = 0;
    D_001CE9D4 = 0;
    func_001C2644(D_001CE95C, 3, 0);
    func_001C2114(D_001CE8A8, D_001CE90C, 1);
    D_001D2E38 = D_00096C90;
    D_001D2E3C = D_00096C94;
    for (;;) {
        func_000514E0();
        r = func_001C26E4(D_001CE95C, &D_001CE9D4);
        if (r != 0) break;
        func_001C19C4();
        func_001C1F40(2);
        func_001C1BD4(D_001CE90C);
        func_001C11E4();
    }
    func_001C2114(D_001CE8A8, D_001CE90C, 0);
    if (r != -1) return 0;
    D_00096C90 = D_001D2E38;
    D_00096C94 = D_001D2E3C;
    func_0006F478(D_00096C90);
    SetMasterVolume(D_00096C94);
    return 0;
}
