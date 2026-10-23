extern int D_000BDFA4[];
extern char D_001CED34[], D_000868DC[];
extern void func_001C2114();
extern void func_0007457C();
extern void func_0006F6E8();
extern void func_000EC280();
extern void func_000E7508();
extern void func_001C2DAC();
extern void func_001C2F78();
extern void func_001C1B4C();
extern void SetDispMask();

int func_001C7F48(void)
{
    func_001C2114(0, D_001CED34, 0);
    func_0007457C(D_000BDFA4[1], 0x1080);
    func_0007457C(D_000BDFA4[33], 0x1080);
    func_0006F6E8();
    func_000EC280(0);
    func_000E7508(6, D_000868DC, 0x1E00);
    func_001C2DAC(0x280);
    func_001C2F78();
    func_001C1B4C();
    func_0007457C(D_000BDFA4[1], 0x1080);
    func_0007457C(D_000BDFA4[33], 0x1080);
    SetDispMask(1);
    func_001C2114(0, D_001CED34, 1);
    return 0;
}
