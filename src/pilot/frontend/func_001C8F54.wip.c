/* WIP 97.62: then-branch constant 1 lands in a0/v1 instead of v0 with an a0 copy; else path copies via a0 */
extern int D_000967F0, D_0009763C, D_000BDFA4[];
extern char D_00096895;
extern char D_001C09A8[], D_001C09BC[];
extern void func_000EC280();
extern void func_000E6D60();
extern void func_0007457C();
extern void func_0006F6E8();
extern void func_000E7508();
extern void SetDispMask();
extern void func_001C2DAC();
extern void func_001C2F78();

int func_001C8F54(void)
{
    int r;
    func_000EC280(0);
    func_000E6D60(D_001C09A8, D_001C09BC, 1, -2);
    func_0007457C(D_000BDFA4[1], 0x1080);
    func_0007457C(D_000BDFA4[33], 0x1080);
    func_0006F6E8();
    func_000E7508(1, 0x1FABBC, 0x3E00);
    SetDispMask(0);
    if (D_000967F0 != 0) {
        SetDispMask(0);
        r = D_00096895 = 1;
        D_0009763C = 0;
    } else {
        func_001C2DAC(0x280);
        func_001C2F78();
        func_0007457C(D_000BDFA4[1], 0x1080);
        func_0007457C(D_000BDFA4[33], 0x1080);
        SetDispMask(1);
        r = 0;
    }
    return r;
}
