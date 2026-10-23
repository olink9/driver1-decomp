extern int D_00096A08, D_00096874, D_00096854, D_00096834, D_00096890, D_00096884, D_00096878;
extern int D_00096A6C, D_0009709C, D_00096810;
extern unsigned char D_00096898;
extern int CameraCnt, FastForward, pauseflag;
extern char cameraview;
extern unsigned char tracking_car;
extern int *ReplayParameterPtr;
typedef struct { int vx, vy, vz, pad; } VECTOR;
extern VECTOR camera_position;
extern int D_0009B16C[];
extern char D_001E0000[], D_001E00DC[], D_001E0148[], D_001E0160[], D_001E0178[], D_001E0194[];
extern char D_001E3508[], D_001E35A0[];
extern int D_001E335C, D_001E3588, D_001E4098, D_001E40A8;
extern void SetTextColour(unsigned char r, unsigned char g, unsigned char b);
extern void PrintString();
extern int sprintf();
extern int dist();
extern void func_0005C4C8(void);
extern void func_0005C8F4();
extern void func_001E14D4(), func_001E16D8(), func_001E282C(), func_001E232C(), func_001E2AB4(), func_001E2CC8();

void func_001E1E08(void)
{
    int t;
    int c;

    if (D_00096A08 != 1)
        return;

    if (D_00096874) {
        t = CameraCnt & 0x1F;
        if (t < 16)
            c = t;
        else
            c = 32 - t;
        SetTextColour(c << 3, 0, 0);
        PrintString(D_001E0000, 0xD2, 0x1E);
        D_00096890 = 0x17;
        if (D_00096854)
            cameraview = 0;
        else
            cameraview = 1;
        tracking_car = 1;
        if (D_00096834 == 6) {
            if (CameraCnt < *ReplayParameterPtr)
                func_0005C4C8();
        } else {
            camera_position.vx = -0x2904;
            camera_position.vz = -15000;
            camera_position.vy = -500;
        }
        func_001E16D8(360);
    } else if (D_00096834 == 2) {
        func_001E14D4();
    }

    if (D_001E335C != 2 && !D_00096874 && D_00096898 == 8 &&
        dist(D_0009B16C, D_0009B16C + 2277) > 40000) {
        D_001E3588 = 0;
        D_00096884 = 0;
        D_001E335C = 2;
        D_00096878 = 1;
    }

    if (D_001E335C == 0) {
        D_00096A6C = (D_00096A6C - 0x40) & 0xFFF;
        func_001E282C();
        func_001E232C();
        if (D_001E3588 > 60) {
            D_001E335C = 1;
            D_00096A6C = 0x800;
            D_00096878 = 0;
            D_001E3588 = 0;
            return;
        }
    } else if (D_001E335C == 2) {
        D_00096A6C = (D_00096A6C - 0x16) & 0xFFF;
        SetTextColour(0x7F, 0x7F, 0x7F);
        if (D_00096874)
            sprintf(D_001E3508, D_001E0148);
        else if (D_00096884 == 0)
            sprintf(D_001E3508, D_001E0160);
        else {
            sprintf(D_001E3508, D_001E0178);
            func_001E2CC8();
        }
        D_001E4098 = 0x1F;
        if (D_001E3588 > 120)
            D_0009709C = 1;
    } else if (D_001E335C == 3) {
        D_00096A6C = (D_00096A6C - 0x16) & 0xFFF;
        sprintf(D_001E3508, D_001E0194);
        D_001E4098 = 0x1F;
        cameraview = 0;
        tracking_car = 1;
        if (D_001E3588 > 120)
            D_0009709C = 1;
    } else {
        func_001E282C();
        func_001E232C();
        func_001E2AB4();
        if (CameraCnt > *ReplayParameterPtr && D_001E335C != 2) {
            D_001E335C = 2;
            if (!D_00096874)
                cameraview = 0;
            tracking_car = 1;
            D_001E3588 = 0;
            D_00096878 = 1;
            if (!D_00096874)
                D_001E35A0[D_00096810] = 1;
        }
    }

    if (D_001E40A8 > 0)
        D_001E40A8--;

    if (D_001E4098 > 0) {
        if (D_001E4098 < 10)
            SetTextColour(D_001E4098 << 3, D_001E4098 << 3, D_001E4098 << 3);
        else
            SetTextColour(0x7F, 0x7F, 0x7F);
        if (!FastForward)
            func_0005C8F4(D_001E3508, 0xB4);
        if (--D_001E4098 == 0)
            sprintf(D_001E3508, D_001E00DC);
    }

    if (!pauseflag)
        D_001E3588++;
}
