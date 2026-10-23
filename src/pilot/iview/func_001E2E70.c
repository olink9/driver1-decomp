typedef struct { unsigned char id; char pad[3]; int w[98]; } CAR;
typedef struct { unsigned char d[0xBB8]; } RBUF;
typedef struct { int RecordingEnd; } REPLAY_PARAMETER_BLOCK;
typedef struct { char pad0[0xC]; int a; char pad10[0xC]; int b; char pad20[0x1CC - 0x20]; int flags; } PS;
typedef struct { char pad0[0xE4]; int fE4; } PL;

extern PS D_000C6FD4;
extern PL D_000C6F04;
extern int D_000C6FE8;
extern REPLAY_PARAMETER_BLOCK *ReplayParameterPtr;
extern int CameraCnt;
extern unsigned char D_00096898;
extern CAR D_0009B16C[];
extern CAR car_data[];
extern int D_00096878;
extern int D_001E3598;
extern int D_0009745C;
extern int D_001E42C8;
extern int D_000967D4;
extern int D_00096884;
extern unsigned char D_00099031;
extern unsigned char *D_000BEED4[];
extern CAR D_001E35C0;
extern CAR D_001E40B0;
extern int dist();
extern void func_001E333C(void);
extern void func_001E32A0(void);
extern void func_00058814(void);
extern void func_00068020(char *p, int a, unsigned char c);

void func_001E2E70(void)
{
    int a, b, n;
    int *flags;

    a = D_000C6FD4.a;
    b = D_000C6FD4.b;
    flags = &D_000C6FD4.flags;
    if (CameraCnt == ReplayParameterPtr->RecordingEnd - 2
        || (D_00096898 == 8 && dist(&D_0009B16C[0], &D_0009B16C[23]) > 40000))
        func_001E333C();
    if (D_00096878 == 0 && D_001E3598 < D_0009745C)
        func_00068020((char *)&D_000C6F04, D_0009745C - D_001E3598, 0);
    D_001E3598 = D_0009745C;
    if (!(*flags & 8))
        return;
    if (D_000C6F04.fE4 <= D_001E42C8)
        return;
    if (D_000C6F04.fE4 > 0)
        func_001E32A0();
    D_001E42C8 = D_000C6F04.fE4;
    if (D_000967D4 == 0)
        return;
    func_00058814();
    if (D_001E42C8 == 1 || b >= a) {
        D_001E35C0 = D_001E40B0;
        *(RBUF *)D_000BEED4[1] = *(RBUF *)D_000BEED4[0];
        D_00096884 = 23;
        ReplayParameterPtr->RecordingEnd = CameraCnt;
    }
    if (D_000C6FE8) {
        D_00099031 = 7;
        D_00096898 = 8;
    }
    car_data[23] = D_001E35C0;
    car_data[23].id = 23;
    D_001E40B0 = car_data[0];
    CameraCnt = 0;
}
