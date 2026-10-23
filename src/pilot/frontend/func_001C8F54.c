/* func_001C8F54: early `if (!x) { ...; return 0; }` + final `return 1;` (if/else with one r reorders v0) */
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
    func_000EC280(0);
    func_000E6D60(D_001C09A8, D_001C09BC, 1, -2);
    func_0007457C(D_000BDFA4[1], 0x1080);
    func_0007457C(D_000BDFA4[33], 0x1080);
    func_0006F6E8();
    func_000E7508(1, 0x1FABBC, 0x3E00);
    SetDispMask(0);
    if (D_000967F0 == 0) {
        func_001C2DAC(0x280);
        func_001C2F78();
        func_0007457C(D_000BDFA4[1], 0x1080);
        func_0007457C(D_000BDFA4[33], 0x1080);
        SetDispMask(1);
        return 0;
    }
    SetDispMask(0);
    D_00096895 = 1;
    D_0009763C = 0;
    return 1;
}
