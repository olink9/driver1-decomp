/* func_000230B0: per-frame game step (Driver 2: StepGame-like) */
typedef struct { int v[4]; } V16;

typedef struct {
    char pad0[0xC];
    short speed;            /* 0x0C */
    char padE[0xC8 - 0xE];
    V16 pos;                /* 0xC8 */
    char padD8[0x17C - 0xD8];
    short f17C;             /* 0x17C */
    char pad17E[396 - 0x17E];
} CAR;

typedef struct { signed char a; unsigned char b; } PADV;
typedef struct { unsigned short v; char pad[6]; } H8;

extern long long D_000977A8;
extern long long D_000977A4;
extern int D_00097118;
extern int D_00096834;
extern unsigned char D_00096899;
extern long long D_00097030;
extern long long ReplayParameterPtr;
extern int CameraCnt;
extern int pauseflag;
extern int D_00097064;
extern unsigned char D_000A8014[];
extern int D_00096874;
extern int D_00096848;
extern int D_00096878;
extern unsigned short D_00097124;
extern int D_0009705C;
extern PADV D_00097060;
extern int D_000970F4;
extern int D_0009B1A8[];
extern int D_00096888;
extern CAR car_data[];
extern unsigned char D_00096898;
extern unsigned char D_000970A0;
extern int D_00097088;
extern char D_0009D438[];
extern int D_0009709C;
extern int D_000970D0;
extern long long D_000976F8;
extern int D_000970EC;
extern int D_00096920;
extern H8 D_00096E7C;
extern int D_000970FC;
extern int D_00097080;
extern long long D_00096FE4;
extern int D_00096840;
extern char D_0009B16C[];
extern int D_00096884;
extern long long D_00096F80;
extern int D_00097084;
extern int D_0009711C;
extern V16 D_000A1AF0;

extern void func_00042BD0();
extern void animate_garage_door();
extern void func_00032088();
extern int func_00079AF0();
extern void cjpRecord();
extern void func_0005BF2C();
extern void func_0001FF24();
extern void func_0001D608();
extern void func_0001A94C();
extern void func_0001A30C();
extern void func_00064A64();
extern void func_0005F3F8();
extern void GlobalTimeStep();
extern void func_00066224();
extern void func_0004C15C();
extern void func_0001A9BC();
extern void func_00021C10();
extern void func_0004D740();
extern void func_000359D0();
extern void func_0004CF50();
extern void func_0002054C();
extern void SwirlLeaves();
extern void func_00020780();

void func_000230B0(void)
{
    int steer;

    if (*(int *)&D_000977A8 != 0 || *(int *)&D_000977A4 != 0)
        D_00097118++;

    func_00042BD0();
    animate_garage_door();
    func_00032088(D_00096834);
    *(int *)&D_00097030 = 0;

    if (D_00096899 != 0 && **(int **)&ReplayParameterPtr - 2 < CameraCnt)
        pauseflag = 1;

    D_00097064 = func_00079AF0(0x1F8003E8);

    steer = D_000A8014[8] - 0x80;

    if (D_00096899 == 0) {
        if (D_00096874 == 0) {
            D_0009705C = D_00097124;
            D_00097060.b = D_000A8014[9] & 2;
            D_00096848 = 1;
            D_00097060.a = steer;
            D_000970F4 = D_00097124;
            if (D_00096878 != 0) {
                if (D_0009B1A8[0] > 0x9000)
                    D_0009705C = 0x80;
                else
                    D_0009705C = 0x10;
                D_00097060.a = 0;
                D_00097060.b = 1;
            }
            cjpRecord(D_00096888 != 0, &D_0009705C, &D_00097060.a, &D_00097060.b);
            func_0001FF24(D_00096888, D_0009705C, D_00097060.a, D_00097060.b);
            func_0001D608(&car_data[D_00096888], 1);
        }
    } else {
        func_0005BF2C(D_00096888, &D_0009705C, &D_00097060.a, &D_00097060.b);
        D_000970F4 = D_0009705C;
        func_0001FF24(D_00096888, D_0009705C, D_00097060.a, D_00097060.b);
        func_0001D608(&car_data[D_00096888], 1);
    }

    if (D_00096898 == 3 || D_00096898 == 8) {
        func_0005BF2C(1, &D_0009705C, &D_00097060.a, &D_00097060.b);
        D_000970F4 = D_0009705C;
        D_000970A0 = D_00097060.a;
        if ((D_00096874 == 0 && D_00096878 != 0 && CameraCnt > 100) || D_00097088 != 0 ||
            **(int **)&ReplayParameterPtr < CameraCnt)
            func_0001FF24(0x17, 0x10, 0, 1);
        else
            func_0001FF24(0x17, D_0009705C, D_00097060.a, D_00097060.b);
        func_0001D608(D_0009D438, 1);
    }

    if (D_00096834 != 6 && D_00096834 != 2) {
        if (D_0009709C == 0) {
            func_0001A30C();
            func_00064A64();
            if (D_000970D0 != 0)
                func_0005F3F8();
        }
    } else {
        func_0001A94C();
        *(int *)&D_000976F8 = 0;
        func_00064A64();
    }

    GlobalTimeStep();
    if (D_000970EC != 0)
        func_00066224();
    func_0004C15C();
    if (D_00096834 != 6 && D_00096834 != 2)
        func_0001A9BC();
    func_00021C10();
    func_00079AF0(D_00097064);

    CameraCnt++;
    if (car_data[0].speed < 100) {
        if (++D_00096920 > 180)
            car_data[0].f17C = D_00096E7C.v + 100;
    } else {
        D_00096920 = 0;
    }

    D_000970FC = (D_000970FC + 60) & 0xFFF;

    if (D_00097080 != 0 && *(int *)&D_00096FE4 == 0) {
        if (D_00096840 == 0)
            func_0004D740(4, 0, 8, D_0009B16C, D_0009B16C + 0x10);
        *(int *)&D_00096FE4 = 11;
    }

    func_000359D0();
    func_0004CF50();
    func_0002054C(0);
    SwirlLeaves(0);

    if (D_00096898 == 6 || D_00096898 == 3 || (unsigned char)(D_00096898 - 7) < 2) {
        func_00020780(0x17);
        SwirlLeaves(0x17);
    }

    if (D_00096878 == 1 && D_00096884 != 0 && D_00096874 == 0 && *(int *)&D_00096F80 == 5 &&
        ++D_00097084 > 20 && D_0009711C == 0) {
        D_0009711C = 1;
        D_000A1AF0 = car_data[D_00096884].pos;
    }
}
