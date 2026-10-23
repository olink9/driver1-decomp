extern int D_001CDE5C, D_001CE200;
extern int D_001CDE64;
extern int D_000967F4, D_00096808, D_00096834, D_00096870;
extern char D_001CE1A0[], D_001CE120[];
extern void func_001C2114();
extern void func_001C2644();
extern int func_001C41D8();
extern void func_001C4BC8();

int func_001C5068(void)
{
    int old;
    func_001C2114(0, 0, 0);
    D_001CDE64 = 0;
    D_001CDE5C = 2;
    D_001CDE64 = 3;
    D_000967F4 = 0x13;
    if (func_001C41D8()) {
        switch (D_00096834) {
        case 0:
            D_00096808 = 100;
            break;
        case 1:
            D_00096808 = 101;
            break;
        case 3:
            D_00096808 = 102;
            break;
        case 5:
            D_00096808 = 103;
            break;
        }
        D_000967F4 = 0x13;
        old = D_00096870;
        D_00096870 = 2;
        func_001C4BC8(D_00096808 - 100);
        D_00096870 = old;
    }
    D_001CDE5C = 6;
    D_001CE200 = 4;
    func_001C2644(D_001CE1A0, 6, 4);
    func_001C2114(0, D_001CE120, 1);
    return 0;
}
