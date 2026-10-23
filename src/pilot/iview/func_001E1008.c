typedef struct { char pad[9]; unsigned char f9; char pad2; unsigned char fB; } OBJ;

extern int D_001E42C0, D_001E4248, D_001E42A8, D_001E4028, D_000970A8, D_001E4030;
extern int D_001E3570, D_001E4268, D_001E3588, D_001E4080;
extern int D_001E404C[];
extern OBJ *D_001E4250;
extern int D_001E3450;
extern int D_001E335C, D_001E4278, D_00096A6C, D_00096878;
extern unsigned char cameraview, tracking_car;
extern int D_001E3558, D_001E35B8, D_001E42D0;
extern int D_00096874;
extern char D_001E3508[];
extern int sprintf();
extern void func_001E22CC(void);
extern void func_001E22F4(void);
extern int func_00016760(int x, int y, int z, int dir, int flag);

void func_001E1008(void)
{
    int i;

    D_001E42C0 = 0;
    D_001E4248 = 0;
    D_001E42A8 = 0;
    D_001E4028 = 0;
    D_000970A8 = 0;
    D_001E4030 = 0;
    D_001E3570 = 0;
    D_001E4268 = 10;
    D_001E3588 = 0;
    D_001E4080 = 0;
    for (i = 0; i < 13; i++)
        D_001E404C[i] = 255;
    D_001E4250->f9 += 2;
    D_001E4250->fB -= 2;
    D_001E335C = 0;
    D_001E4278 = 0;
    D_00096A6C = 0x800;
    D_00096878 = 1;
    cameraview = 0;
    tracking_car = 1;
    D_001E3558 = 5000;
    D_001E35B8 = 0;
    D_001E42D0 = 4096 / (D_001E3450 * 60);
    func_001E22CC();
    func_00016760(-16000, 200, -10000, 0x400, 0);
    func_00016760(-16000, 200, -14000, 0x400, 0);
    func_00016760(-12800, 200, -10200, 0x400, 0);
    func_00016760(-4700, 200, -9000, 0xC00, 0);
    func_00016760(-4700, 200, -14000, 0xC00, 0);
    func_00016760(-8200, 200, -20000, 0xC00, 0);
    func_001E22F4();
    D_001E335C = 0;
    if (D_00096874)
        sprintf(D_001E3508, "Now Pay Attention...");
    else
        sprintf(D_001E3508, "Follow my leader...");
}
