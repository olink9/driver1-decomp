extern int D_001D2DC8, D_000967D4, D_001CDE5C, D_001CDE64, D_001CE200;
extern char D_001CE408[], D_001CE3D8[];
extern void func_001C2114();
extern void func_001C5268();
extern void func_001C2644();

int func_001C584C(void)
{
    int m = D_001D2DC8;
    if (m == 0) {
        D_000967D4 = 1;
        D_001CDE5C = 6;
        func_001C2114(0, 0, 0);
        D_001CDE64 = 0;
        D_001CDE5C = 6;
        func_001C5268();
        D_001CDE5C = 6;
        D_001D2DC8 = 0;
        D_001CE200 = 5;
        func_001C2644(D_001CE408, 2, 0);
    } else if (m == 1) {
        D_000967D4 = 0;
        func_001C2114(0, 0, 0);
        D_001CDE64 = 0;
        D_001CDE5C = 6;
        func_001C5268();
        D_001CDE5C = 6;
        D_001D2DC8 = m;
        D_001CE200 = 5;
        func_001C2644(D_001CE408, 2, 1);
    } else {
        return 0;
    }
    func_001C2114(0, D_001CE3D8, 1);
    return 0;
}
