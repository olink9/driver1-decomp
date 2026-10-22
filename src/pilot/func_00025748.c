/* func_00025748 - per-frame game loop step (no Driver 2 match) */
typedef struct { unsigned char v; char pad[7]; } B8;

typedef struct CarS {
    char pad0[0x114];
    int f114[6];
    char pad1[0x17C - 0x12C];
    short f17C;
    char pad2[396 - 0x17E];
} CarS;

typedef struct {
    char pad0[0xC];
    short c;
    char pad1[2];
} SndS;

typedef struct {
    char pad0[0xF8];
    CarS *car;
    char pad1[0x2D8 - 0xFC];
    int idx;
} PlayerS;

extern int camera_position[];
extern unsigned char D_000967DD;
extern int D_000967E8;
extern int D_000967F4;
extern int D_00096800;
extern int D_0009681C;
extern int D_00096834;
extern int D_00096840;
extern int AttractMode;
extern int gNight;
extern int D_00096854;
extern int D_00096858;
extern int D_00096874;
extern int D_00096888;
extern unsigned char D_00096896;
extern unsigned char D_00096898;
extern unsigned char D_00096899;
extern char D_0009692C[];
extern B8 D_00096AA8;
extern B8 D_00096AA9;
extern long long FastForward;
extern long long D_00096F80;
extern int D_00097074;
extern int D_00097080;
extern int D_000970A4;
extern int D_000970D0;
extern int CameraCnt;
extern int D_000970EC;
extern CarS car_data[];
extern SndS D_000BE9C4;
extern SndS D_000BE9E0;
extern PlayerS D_000C6F04;

extern void func_00014A8C();
extern void DrawTyreTracks();
extern void func_0002C53C();
extern void DrawMapPSX();
extern void InitFrustrumMatrix();
extern void func_0002DBB4();
extern void func_0002DBEC();
extern void func_0002F444();
extern void func_00031AE0();
extern void func_0003A698();
extern void AddGroundDebris();
extern void func_0004287C();
extern void func_00046000();
extern void DrawLensFlare();
extern void DrawRainDrops();
extern void func_00049914();
extern void AddRainDrops();
extern void ScaleCamera();
extern void func_00050024();
extern void func_0005347C();
extern void func_00053644();
extern void func_000543D8();
extern void DrawPercentageBar();
extern void SetTextColour();
extern void PrintString();
extern void func_0005DB30();
extern void func_000669B8();
extern void func_0006B4AC();

void func_00025748(void)
{
    int a;
    int i;
    short snd;
    CarS *cp;

    InitFrustrumMatrix();
    func_0002DBB4();
    func_0002DBEC();
    func_0002C53C();
    DrawMapPSX();

    if (D_00096834 == 5 && camera_position[2] < -0x6CA48 && camera_position[0] > -0x1E078)
        func_0002F444();

    func_0004287C();
    func_0005347C();

    if (AttractMode != 0) {
        if ((CameraCnt & 0x1F) < 16)
            a = CameraCnt & 0x1F;
        else
            a = 32 - (CameraCnt & 0x1F);
        SetTextColour((a << 3) & 0xF8, 0, 0);
        if (D_00096800 == 0)
            PrintString(D_0009692C, 0x23, 0xF);
    }

    if (D_00096834 == 0)
        func_00053644();

    if (D_00096888 == 0 || (D_00096898 != 6 && D_00096898 != 7)) {
        if (D_00096840 != 0 && D_00097080 != 0)
            func_00046000(0);
        func_0003A698(0, 0);
    }

    if (D_000970EC != 0)
        func_000669B8();

    AddGroundDebris();
    func_00031AE0(D_00096834);

    if (D_00096AA8.v != 0) {
        AddRainDrops();
        DrawRainDrops();
    }
    if (D_00096AA9.v != 0) {
        AddRainDrops();
        func_00049914();
    }

    ScaleCamera();

    if (D_00096854 == 0 && *(int *)&FastForward == 0)
        func_00050024();

    if (D_000970D0 != 0)
        func_0005DB30();

    DrawTyreTracks();

    if (gNight == 0 && *(int *)&FastForward == 0 && D_00096834 != 2)
        DrawLensFlare();

    if (D_00096800 == 0) {
        if (D_000970A4 != 0 && D_00096834 != 6 && *(int *)&D_00096F80 == 5 && D_000967DD == 0)
            func_000543D8();

        if (D_00096800 == 0 && D_00096854 == 0 && D_00096896 == 0 && D_00096899 == 0 &&
            AttractMode == 0 && D_000967DD == 0 && D_00097074 != 0 && *(int *)&D_00096F80 == 5) {
            cp = &car_data[0];
            if (D_000967E8 == 0 && D_00096874 == 0) {
                if (D_0009681C == 1) {
                    for (i = 5; i >= 0; i--)
                        cp->f114[i] = 0;
                    cp->f17C = 0;
                } else {
                    D_000BE9E0.c = cp->f17C;
                    DrawPercentageBar(&D_000BE9E0);
                }
            }

            snd = -1;
            if (D_000C6F04.idx >= 0) {
                snd = car_data[D_000C6F04.idx].f17C;
            } else if (D_000C6F04.idx == -1) {
                if (D_000C6F04.car != 0)
                    snd = D_000C6F04.car->f17C;
            }

            if (snd != -1 && D_000967E8 == 0 && D_00096858 == 0) {
                D_000BE9C4.c = snd;
                DrawPercentageBar(&D_000BE9C4);
            }
        }
    }

    if ((D_000967F4 == 5 || D_000967F4 > 16 || D_000967F4 == 3) &&
        D_00096899 == 0 && D_000967E8 == 0 && D_000967DD == 0)
        func_0006B4AC(&D_000C6F04);

    func_00014A8C();
}
