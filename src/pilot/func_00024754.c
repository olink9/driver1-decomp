/* func_00024754 - no Driver 2 counterpart (frontend/game-state dispatcher) */

typedef struct {
    unsigned char b0, b1, b2;
    unsigned f24:1, f25:1, f26:2, f28:2, f30:1, f31:1;
    unsigned g0:1, g1:1, g2:1, g3:1;
} S40;

typedef struct { int a; char pad[52]; } S38;

extern int D_000967C8, AttractMode, D_000967F4, D_00096814, D_000967C4, D_00096820, D_0009681C;
extern int D_00096808, D_000967E4, D_00096854, D_00096858, D_00096870, D_00096834, D_00096884;
extern int D_00097110, D_00096888, D_00096874, D_0009709C, D_000967CC, D_000967F0, D_000970A8;
extern int D_00097068, D_000967E0, D_00096800, D_000967F8, D_000967D8, D_00096810;
extern unsigned char D_000967BC, D_00096899, D_00096898, D_00096895;
extern long long D_00096F80, D_00096784, D_00096780, D_0009677C, D_00096778, D_00096DE4;
extern long long D_00096C94, D_00096C90;
extern S40 D_00097640;
extern int D_0009D8DC[];
extern int D_000A083C[];
extern int D_000868DC[];
extern S38 D_0008671C[];
extern short D_0008AA38[];
extern int D_000C7008[];
extern char D_0001028C[];
extern char D_0001023C[];

extern void DrawSync(), VSync(), SsSetSerialVol();
extern void func_0004E170(), func_0004E348(), func_0006F4D8(), func_0006F6E8();
extern void func_00024F1C(), ReInitFrontend(), func_0006EB48(), SetDispMask();
extern void Loadfile(), func_0006EACC(), PlayRender(), func_00079AF0();
extern void ReInitSystem(), func_0006EB74(), func_00079800(), ShowLoadingScreen();
extern void func_00022818(), func_0002360C(), func_000739B4(), SetMasterVolume(), func_0004E454();
extern int func_00079AE0();

void func_00024754(void)
{
    int loop;
    int saved784;
    int saved870;
    int h;
    int i;
    char pad[8];

    DrawSync(0);
    VSync(0);
    D_000967C8 = 0;
    AttractMode = 0;
    func_0004E170();
    func_0004E348();
    func_0006F4D8();
    SsSetSerialVol(0, 0, 0);
    func_0006F6E8();
    D_000967BC = 1;
    *(int *)&D_00096F80 = 5;

    switch (D_000967F4) {
    case 23:
        D_00096899 = 0;
        D_00096898 = 0;
        func_00024F1C();
        if (D_00096814 == 3) {
            D_000967C4 = 0;
            return;
        }
        break;
    case 1:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
        D_00096899 = 0;
        D_00096898 = 0;
        if (D_000967F4 == 0x13) {
            D_00096820 = 0;
            D_0009681C = 0;
        }
        if (D_000967F4 == 0x15)
            D_00096820 = 0;
        func_00024F1C();
        D_00096808 = -1;
        D_00096898 = 0;
        break;
    case 16:
        D_00096899 = 1;
        D_000967E4 = 1;
        D_00096834 = D_00097640.b0;
        D_000967F4 = D_00097640.b1;
        D_00096808 = D_00097640.b2;
        D_00096854 = D_00097640.f24;
        D_00096858 = D_00097640.f25;
        D_00096870 = D_00097640.f28;
        *(int *)&D_00096784 = D_00097640.f26;
        D_00096820 = D_00097640.f31;
        D_0009681C = D_00097640.f30;
        D_0009D8DC[0] = D_00097640.g0;
        D_0009D8DC[1] = D_00097640.g1;
        D_0009D8DC[2] = D_00097640.g2;
        D_0009D8DC[4] = D_00097640.g3;
        if (D_000967F4 == 0x13)
            D_00096870 = 2;
        func_00024F1C();
        ReInitFrontend();
        D_000967F0 = 0;
        return;
    case 3:
        D_00096858 = 1;
        D_00096884 = 0x17;
        *(int *)&D_00096780 = 1;
        D_00096898 = 0;
        loop = 1;
        D_00097110 = 0;
        D_00096899 = 0;
        D_00096888 = 0;
        D_00096899 = 0;
        *(int *)&D_0009677C = 0;
        D_00096898 = 8;
        if (D_00096834 == 6) {
            D_00096854 = 0;
            D_00096808 = 0xAA;
            do {
                D_00097110 = 0;
                D_00096874 = 1;
                do {
                    D_00096898 = 8;
                    D_0009709C = 0;
                    func_00024F1C();
                    D_00097110 = 1;
                } while (D_0009709C == 3);
                D_00096874 = 0;
                if (D_0009709C != 2) {
                    do {
                        D_00096898 = 8;
                        D_0009709C = 0;
                        func_00024F1C();
                    } while (D_0009709C == 3);
                }
                if (D_000967CC) {
                    D_00096808++;
                    func_0004E348();
                    D_000967BC = 1;
                } else {
                    D_000967CC = 0;
                }
                if (D_00096808 >= 0xAD)
                    D_00096808 = 0xAA;
                if (D_0009709C == 2)
                    loop = 0;
            } while (loop);
        } else {
            D_00096854 = 1;
            D_00096810 = 0;
            D_00096808 = 0xAD;
            do {
                D_00096874 = 1;
                do {
                    D_00096898 = 8;
                    D_0009709C = 0;
                    func_00024F1C();
                    D_00097110 = 1;
                } while (D_0009709C == 3);
                D_00096874 = 0;
                if (D_0009709C != 2) {
                    do {
                        D_00096898 = 8;
                        D_0009709C = 0;
                        func_00024F1C();
                    } while (D_0009709C == 3);
                }
                if (D_0009709C == 2)
                    loop = 0;
            } while (loop);
        }
        D_00096854 = 0;
        D_00096858 = 0;
        D_00096898 = 0;
        *(int *)&D_00096778 = 0;
        AttractMode = 0;
        D_00096899 = 0;
        break;
    case 2:
        D_00096898 = 0;
        D_00096899 = 0;
        D_00096898 = 0;
        D_000967F4 = 5;
        func_00024F1C();
        ReInitFrontend();
        D_00096898 = 0;
        func_0006EB48();
        SetDispMask(1);
        return;
    case 4:
        D_00096898 = 0;
        D_00096899 = 0;
        D_00096898 = 0;
        func_00024F1C();
        break;
    case 5:
        D_00096898 = 0;
        D_00096899 = 0;
        D_00096898 = 0;
        func_00024F1C();
        Loadfile(D_0001028C, 0x1C0000);
        D_00096898 = 0;
        func_0006EB48();
        SetDispMask(1);
        D_000967BC = 1;
        return;
    case 0:
        D_000970A8 = 0;
        D_000967C4 = 0;
        if (D_000967F0 == 0) {
            func_0006EACC();
            D_00097068 = func_00079AE0();
            PlayRender(2);
            func_00079AF0(D_00097068);
            ReInitSystem();
            D_00096854 = 1;
            D_00096808 = 0xAE;
            D_000967F4 = 5;
            D_00096899 = 0;
            D_00096898 = 0;
            D_00096834 = 2;
            func_00024F1C();
            D_000967F4 = 0;
            Loadfile(D_0001028C, 0x1C0000);
            D_00096854 = 0;
            D_0009709C = 0;
        } else {
            D_000970A8 = 1;
        }
        if (D_000970A8 == 1) {
            D_000967BC = 1;
            func_0006EACC();
            if (D_000967F0) {
                if (D_000967E0 < D_000A083C[0]) {
                    D_000868DC[0] = D_000A083C[0];
                    D_000967E0 = D_000A083C[0];
                }
                D_000C7008[0] = 0;
                D_000868DC[1] = D_000A083C[1];
                D_000868DC[2] = D_000A083C[2];
                D_000868DC[3] = D_000A083C[3];
                D_000967F0 = 0;
            }
            func_0006EB74();
            D_00096814 = 1;
        }
        ReInitFrontend();
        D_000967F0 = 0;
        return;
    case 7:
        saved784 = *(int *)&D_00096784;
        saved870 = D_00096870;
        AttractMode = 1;
        D_00096895 = 1;
        D_00096899 = 1;
        D_00096870 = 1;
        *(int *)&D_00096784 = 1;
        D_00096820 = 0;
        D_0009681C = 0;
        D_0009D8DC[0] = 0;
        D_0009D8DC[1] = 0;
        D_0009D8DC[2] = 0;
        D_0009D8DC[4] = 0;
        *(int *)&D_00096DE4 = D_00096808;
        if (*(int *)&D_00096DE4 == 1)
            D_00096808 = 0xB4;
        if (*(int *)&D_00096DE4 == 2)
            D_00096808 = 0xB5;
        if (*(int *)&D_00096DE4 == 3)
            D_00096808 = 0xB6;
        if (*(int *)&D_00096DE4 == 4)
            D_00096808 = 0xB7;
        if (*(int *)&D_00096DE4 == 5) {
            D_00096808 = 0x7F;
            D_000967F4 = 0x17;
        } else if (*(int *)&D_00096DE4 == 3) {
            D_000967F4 = 0x13;
        } else {
            D_000967F4 = 0x15;
        }
        D_00096854 = 0;
        D_00096858 = 0;
        func_00079800(0);
        SetDispMask(0);
        ShowLoadingScreen(D_0001023C, 1, 0x24);
        func_00022818();
        func_0002360C();
        if (D_00096800) {
            SetDispMask(0);
            func_000739B4(0x100, 0x78);
            D_00096800 = 0;
            D_0008AA38[0] = 0x1999;
            D_00096800 = 0;
        }
        ReInitFrontend();
        AttractMode = 0;
        D_00096870 = saved870;
        *(int *)&D_00096784 = saved784;
        return;
    case 8:
        h = func_00079AE0();
        PlayRender(D_000967F8);
        func_00079AF0(h);
        SetDispMask(0);
        ReInitSystem();
        if ((unsigned)(D_000967F8 - 0x46) < 4) {
            for (i = 0; i < D_000967D8; i++)
                D_0008671C[i].a = -2;
            D_000967C4 = 1;
        }
        SetMasterVolume(*(int *)&D_00096C94);
        func_0004E454(*(int *)&D_00096C90);
        return;
    }
    ReInitFrontend();
}

