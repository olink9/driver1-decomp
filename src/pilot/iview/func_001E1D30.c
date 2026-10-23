extern int D_001E4018, D_001E4298, D_001E3598, D_001E42B0, D_001E335C;
extern int D_001E4098, D_001E40A8;
extern char D_001E3508[];
extern int D_00096874, D_00096878, D_0008A8F4, D_0008A1E0, D_00096A6C;
extern void func_001E22F4(void);
extern int sprintf();
extern void func_0003E540(void);
extern void func_00033AF8(void);
extern void func_0003A068(void);

void func_001E1D30(void)
{
    func_001E22F4();
    D_001E4018 = 0;
    D_001E4298 = 0;
    D_001E3598 = 0;
    D_001E42B0 = 15;
    D_001E335C = 0;
    if (D_00096874 != 0) {
        sprintf(D_001E3508, "Now Pay Attention...");
        D_00096878 = 0;
    } else {
        sprintf(D_001E3508, "Follow my leader...");
    }
    D_001E4098 = 0x1F;
    D_001E40A8 = 0x78;
    D_0008A8F4 = -200;
    D_0008A1E0 = -200;
    D_00096A6C = 0x800;
    func_0003E540();
    func_00033AF8();
    func_0003A068();
}
