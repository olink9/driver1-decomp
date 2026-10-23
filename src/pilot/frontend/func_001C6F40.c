extern int D_000967F0, D_000967F4, D_000A083C, D_000BDFA4[];
extern char D_001CE9E8[], D_001C070C[], D_001C0720[], D_001CEA18[];
extern int D_001CEA38;
extern void func_001C2114(), func_000EC280(), func_000E6D60(), func_0006F6E8(), func_000E7508();
extern void SetDispMask(), func_001C2DAC(), func_001C2F78(), func_001C1B4C(), func_0007457C();
extern void func_001C2644(), DrawSync(), VSync(), func_001C11E4(), func_00024754();

int func_001C6F40(void)
{
    int r;

    func_001C2114(0, D_001CE9E8, 0);
    func_000EC280(0);
    func_000E6D60(D_001C070C, D_001C0720, 1, -2);
    func_0006F6E8();
    func_000E7508(0, &D_000A083C, 0x1E00);
    SetDispMask(0);
    if (D_000967F0 == 0) {
        int *p;
        func_001C2DAC(0x280);
        func_001C2F78();
        p = D_000BDFA4;
        func_001C1B4C();
        func_0007457C(p[1], 0x1080);
        func_0007457C(p[33], 0x1080);
        SetDispMask(1);
    } else {
        SetDispMask(0);
    }
    r = 0;
    func_001C2644(D_001CEA18, 2, D_001CEA38);
    if (D_000967F0 != 0) {
        if (D_000A083C != -1) {
            DrawSync(r);
            VSync(r);
            func_001C11E4();
            D_000967F4 = 0;
            func_001C2114(r, D_001CE9E8, r);
        } else {
            D_000967F0 = 0;
            DrawSync(0);
            VSync(0);
            func_001C11E4();
            D_000967F4 = 0;
            func_001C2114(0, D_001CE9E8, 0);
        }
        r = 1;
        SetDispMask(0);
        func_00024754();
    } else {
        func_001C2114(0, D_001CE9E8, 1);
    }
    return r;
}
