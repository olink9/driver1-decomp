/* func_0002360C: main game loop (no Driver 2 counterpart found) */

typedef struct { int w[4]; } Q16;
typedef struct { char c[396]; } CAR396;
typedef struct { unsigned char v; char pad[7]; } B8;

extern int D_0009683C, D_00096834, D_00096840, D_0009709C, CameraCnt, D_00096800;
extern int gNight, D_00096874, D_00096888, D_0009688C, D_00096890, D_000970E4;
extern int D_000970DC, D_00096878, gLightsOn, D_00096854, D_00097118, D_00097128;
extern int AttractMode, pauseflag, D_000967F4, D_00096858, D_00096924, D_00097080;
extern short D_00097124;
extern unsigned char cameraview, D_00096899, D_00096894, D_00096895, D_00096898;

extern long long D_00097534, D_0009757C, D_00096F80, D_0009763C, D_00096C94, D_00096C90;
extern long long D_0009752C, D_00096FE4, D_00097148, D_00097144, D_000971B4, D_00097004;
extern long long D_00096A6C, D_00096A60, D_00096A64, D_000977A8, D_000977A4, combointensity;
extern long long FastForward, D_00096980, ReplayParameterPtr, D_00097538, D_00096A08;
extern B8 tracking_car, D_00096AA8, D_00096AA9, D_00097660;

extern CAR396 car_data[];
extern Q16 D_000BDCD8, camera_position;
extern int D_0008A1DC[];
extern unsigned char D_000BDF94[];
extern char D_000C017C[];
extern char D_000C6F04[];

void CloseShutters();
void func_0003336C();
void DentCar();
void func_000513B4();
void func_00051540();
void func_00051374();
void func_00077D7C();
void func_00033AF8();
void func_0004E08C();
void func_0004C438();
void func_0004DE58();
void SetMasterVolume();
void func_0004E454();
void StartXM();
void func_000334B4();
int func_1E19B4();
void func_000430B8();
void func_000513D4();
void func_000514E0();
void SetDispMask();
void func_00022168();
void func_0003CFE8();
void PlaceCameraAtLocation();
void func_0001D590();
void func_0003D810();
void func_0006DBC8();
void func_0003D6C0();
void func_0003DC00();
void DoLightning();
void func_00025748();
void func_0002D79C();
void func_00059B84();
void HandleDebris();
void SetTextColour(unsigned char r, unsigned char g, unsigned char b);
void func_0005D164();
void func_0005B70C();
void func_0004DF28();
int func_0006C964();
void CheckCameraChange();
void func_000230B0();
void func_1E2E70();
void func_00025C68();
void func_000359BC();
void func_0006C8CC();
void func_0005C8F4();
void func_1E1E08();
void func_1E0290();
void func_0006A934();
void func_0006E040();
void ControlReplay();
int VSync(int);
void DrawSync(int);
void func_0004E170();

#define I(x) (*(int *)&(x))
#define ReplayPtr (*(int **)&ReplayParameterPtr)

void func_0002360C(void)
{
    int x;
    int r;
    char pad[8];

    x = 0;
    if (D_0009683C == 2 && (D_00096834 == 5 || D_00096834 == 3))
        x = 1;
    D_00096840 = x;

    CloseShutters(2, 0x140, 0x200);
    func_0003336C();
    DentCar(0x17);
    func_000513B4();
    func_00051540();
    func_00051374();
    func_00077D7C(0x1234);
    cameraview = 0;
    I(D_00097534) = 0;
    I(D_0009757C) = 0;
    func_00033AF8();
    func_0004E08C();
    func_0004C438();
    if (I(D_00096F80) != 5 && I(D_0009763C) == 0)
        func_0004DE58();
    SetMasterVolume(I(D_00096C94));
    func_0004E454(I(D_00096C90));

    while (D_0009709C == 0) {
        if (CameraCnt == 3 && D_00096834 != 2)
            StartXM(1);
        func_000334B4();
        if (D_00096800 == 1 && func_1E19B4())
            D_0009709C = 7;
        if (gNight || D_00096834 == 3)
            func_000430B8();
        func_000513D4();
        func_000514E0();
        if (I(D_00097534) == 3)
            SetDispMask(1);
        {
            unsigned short pad = *(unsigned short *)&D_0009752C;
            if (pad & 0x2000) {
                if (pad & 0x8000)
                    *(unsigned short *)&D_0009752C = pad & 0x5FFF;
            }
        }
        D_00097124 = *(unsigned short *)&D_0009752C;
        if (I(D_00096FE4) > 0 && D_00097080)
            I(D_00096FE4)--;
        else
            I(D_00096FE4) = 0;
        if (D_00096874 == 0 && I(D_00096F80) == 5)
            func_00022168();

        switch (cameraview) {
        case 0:
            if (D_00096899 == 0)
                func_0003CFE8(D_00096888);
            else
                func_0003CFE8(0);
            break;
        case 1:
            if (D_00096899 == 0 && I(D_00096F80) == 5)
                tracking_car.v = 1;
            PlaceCameraAtLocation(D_00096890, 1);
            break;
        case 3:
            func_0001D590(&car_data[D_0009688C]);
            func_0003D810(D_0009688C, 1);
            break;
        case 2:
            func_0001D590(&car_data[D_0009688C]);
            func_0003D810(D_0009688C, 0);
            break;
        case 4:
            func_0001D590(&car_data[D_0009688C]);
            func_0003D810(D_0009688C, 0);
            break;
        case 5:
            if (I(D_00096F80) == 3)
                func_0006DBC8();
            else if (D_00096899 == 0 && I(D_00096F80) == 5)
                tracking_car.v = 1;
            if (D_00096899 == 0)
                D_00096890 = 0;
            PlaceCameraAtLocation(D_00096890, 0);
            break;
        case 6:
            func_0003D6C0(D_000C017C);
            break;
        case 7:
            func_0003CFE8(0);
            break;
        case 8:
            func_0003DC00();
            break;
        }

        D_000BDCD8 = camera_position;
        I(D_00097148) = 7000;
        I(D_00097144) = 9000;
        I(D_000971B4) = 500 - (((D_000970E4 - 256) * 5) >> 4);

        if (I(D_00097004) == 0 || D_000970DC < 100) {
            D_000970DC--;
            D_00096878 = 1;
            D_00096890 = 0;
            cameraview = 0;
            I(D_00096A6C) = (I(D_00096A6C) - 22) & 0xFFF;
            if (I(D_00096A60) < 1000) {
                I(D_00096A60) += 8;
                I(D_00096A64) += 8;
            }
            if (D_0008A1DC[1] >= -999)
                D_0008A1DC[1] -= 8;
            if (D_000970DC < 0)
                D_0009709C = 1;
        }

        gLightsOn = 0;
        if (D_00096854) {
            D_00097128 = 60;
        } else if (I(D_000977A8) || I(D_000977A4)) {
            if (I(D_000977A8)) {
                int v = (D_00097118 >> 7) + 26;
                D_00097128 = v;
                if (v > 96)
                    D_00097128 = 96;
            } else {
                if (D_00096AA8.v)
                    D_00097128 = 78 - (D_00097118 >> 7);
                else
                    D_00097128 = 128 - (D_00097118 >> 5);
                if (D_00097128 < 26)
                    D_00097128 = 26;
            }
            if (I(D_000977A8) && D_00097128 < 50)
                gLightsOn = 1;
            if (I(D_000977A4) && D_00097128 < 80)
                gLightsOn = 1;
            gNight = 1;
        } else if (gNight == 0) {
            if (D_00096AA8.v) {
                D_00097128 = 78;
                if (D_00096AA9.v == 0)
                    DoLightning();
            } else {
                D_00097128 = 128;
            }
        } else {
            gLightsOn = 1;
            D_00097128 = 26;
            if (D_00096AA8.v && D_00096AA9.v == 0)
                DoLightning();
        }

        if (D_00096834 == 3)
            gLightsOn = 1;
        D_000BDF94[4] = D_00097128;
        D_000BDF94[5] = D_00097128;
        D_000BDF94[6] = D_00097128;
        I(combointensity) = D_00097128 | (D_00097128 << 8) | (D_00097128 << 16);

        if (I(FastForward) == 0) {
            func_00025748();
            I(D_00096980) = 0;
        } else {
            func_0002D79C();
        }
        if (D_00096899 && AttractMode == 0)
            func_00059B84();
        if (I(FastForward) == 0) {
            HandleDebris();
        } else if (CameraCnt < *ReplayPtr - 1) {
            int f = CameraCnt & 0x1F;
            int c;
            if (f < 16)
                c = f;
            else
                c = 32 - f;
            SetTextColour(c * 8, 0, 0);
            func_0005D164("Fast Forward", 100, 30, 0x1000, 0x1000, 0);
        }
        if (I(FastForward))
            func_0005B70C();

        if (AttractMode == 0 && *(short *)&D_00097538 == 0x800 && pauseflag == 0 && D_00096899 == 0) {
            if (D_00096800 == 1) {
                D_0009709C = 7;
            } else if (I(D_00096F80) == 5 && I(D_00097534) >= 3) {
                pauseflag = 1;
                func_0004DF28();
                r = func_0006C964(0);
                if (r == 1)
                    D_0009709C = 3;
                else if (r == 3)
                    D_0009709C = 4;
                else if (r == 4)
                    D_0009709C = 5;
                else if (D_0009709C == 0)
                    func_0004E08C();
            }
        }
        if (D_00096899 == 0) {
            if (I(D_00096A08) == 0) {
                pauseflag = 1;
                func_0004DF28();
            }
            if (D_00096899 == 0 && I(D_0009757C))
                I(D_0009757C)--;
        }

        D_00097660.v = 0;
        if (pauseflag == 0 && D_00096899)
            CheckCameraChange(CameraCnt);
        if (pauseflag == 0) {
            if (I(D_00096F80) == 5 || I(D_00096F80) == 1)
                func_000230B0();
            if (D_000967F4 == 0x16 && D_00096834 == 6)
                func_1E2E70();
            if (D_00096894) {
                D_00096895 = 1;
                if (D_00096899 == 0 && AttractMode == 0)
                    func_00025C68();
                D_00096894 = 0;
            }
        } else {
            func_000359BC();
            func_0006C8CC();
            if (D_00096899 == 0 && AttractMode == 0) {
                if (I(D_00096A08) == 0) {
                    SetTextColour(255, 255, 255);
                    func_0005C8F4("No Controller Connected", 120);
                } else if (I(D_00096A08) == -1) {
                    SetTextColour(255, 255, 255);
                    func_0005C8F4("Unsupported Controller Connected", 120);
                } else if (D_0009709C == 0) {
                    pauseflag = 1;
                    func_0004DF28();
                    r = func_0006C964(0);
                    if (r == 1)
                        D_0009709C = 3;
                    else if (r == 3)
                        D_0009709C = 4;
                    else if (r == 4)
                        D_0009709C = 5;
                    else if (D_0009709C == 0)
                        func_0004E08C();
                }
            }
            D_00096894 = 1;
        }

        if (D_00096834 != 2 && D_00096858 && (D_00096898 == 3 || D_00096898 == 8))
            func_1E1E08();
        if (D_00096854)
            func_1E0290();
        if (D_000967F4 == 5 || D_000967F4 > 16 || D_000967F4 == 3)
            func_0006A934(D_000C6F04);
        if (I(D_0009763C) == 1)
            *(volatile int *)&D_00097534;
        if (I(D_00096F80) != 5 && D_00096899 == 0)
            func_0006E040();
        if (D_00096899 && AttractMode == 0)
            ControlReplay();
        if (I(FastForward) == 0) {
            while ((unsigned)(VSync(-1) - D_00096924) < 2)
                ;
            D_00096924 = VSync(-1);
        }
        (*(volatile int *)&D_00097534)++;
        DrawSync(0);
        if (AttractMode && (*(short *)&D_00097538 != 0 || CameraCnt >= *ReplayPtr)) {
            pauseflag = 1;
            D_0009709C = 1;
        }
    }

    if ((D_00096898 == 3 || D_000967F4 == 0x16) && D_00096899 == 0)
        *ReplayPtr = CameraCnt;
    func_000359BC();
    func_0004E170();
}
