/* no Driver 2 counterpart (front-end / menu state loop) */
typedef struct { short x, y, w, h; } RECT;

extern int D_00096818;
extern int D_00097110;
extern unsigned char D_000967DD;
extern int D_00096858;
extern unsigned char D_000967BD;
extern unsigned char D_000967BC;
extern unsigned char D_00096899;
extern unsigned char D_00096898;
extern int D_0009709C;
extern int D_000967E4;
extern int D_000970A8;
extern int D_00096854;
extern int D_000967F4;
extern int pauseflag;
extern int D_00096874;
extern int D_000967C4;
extern int CameraCnt;
extern int D_00096928;
extern int D_00096800;
extern int D_00096884;
extern long long D_0009763C;
extern long long D_00096CC4;
extern long long D_00097534;
extern long long D_00096780;
extern long long D_0009677C;
extern short D_0008AA38[];
extern int D_000C6F04[];
extern char D_0001023C[];

extern void func_00073F88();
extern void func_00079800();
extern void SetDispMask();
extern void func_00025F48();
extern void ShowLoadingScreen();
extern void func_00022818();
extern void func_1E22CC();
extern void func_0002360C();
extern int func_0006C964();
extern void func_0005071C();
extern void func_00025C04();
extern void func_000739B4();
extern void func_00051374();
extern void func_00074374();
extern void DrawSync();

static inline void WaitScreenFade(void)
{
    *(int *)&D_00096CC4 = 1;
    while (*(int *)&D_00096CC4 == 1)
        func_00025C04();
}

void func_00024F1C(void)
{
    int ret;
    int saved1, saved2, saved3, saved4, saved5, saved6;
    int mode;
    RECT r;
    char *name = D_0001023C;

    ret = 1;
    func_00073F88(1);
    func_00079800(0);
    SetDispMask(0);
    D_00096818 = 0;
    do {
        D_000967DD = 0;
        if (D_00097110) {
            if (D_00096858) {
                func_00025F48();
                goto skip;
            }
        } else if (D_00096858) {
            D_000967BD = 0xFF;
        }
        if (D_000967BC == 1)
            ShowLoadingScreen(name, 1, 0x24);
        SetDispMask(1);
        func_00022818();
        if (D_00096858 == 1)
            func_1E22CC();
    skip:
        func_0002360C();
        if (D_00096899 == 1) {
            if (D_0009709C == 5) {
                D_0009709C = 4;
            } else if (D_0009709C == 4) {
                if (D_000967E4) {
                    D_000967E4 = 0;
                    D_0009709C = 0;
                    ret = 0;
                } else if (D_00096818 == 1) {
                    if (*(int *)&D_0009763C == 1) {
                        D_000970A8 = 1;
                        D_0009709C = 1;
                        D_000967DD = 1;
                        *(int *)&D_0009763C = 0;
                    } else {
                        D_000970A8 = 1;
                        D_0009709C = 6;
                    }
                } else {
                    if (*(int *)&D_0009763C == 1) {
                        D_000967DD = 1;
                        *(int *)&D_0009763C = 0;
                        D_0009709C = 1;
                    } else {
                        D_0009709C = 3;
                    }
                }
            }
        }
        switch (D_0009709C) {
        case 1:
            if (D_00096854 == 1) {
                if (D_000970A8 == 0) {
                    mode = 2;
                    D_00096818 = 0;
                } else {
                    D_00096818 = 1;
                    mode = 1;
                }
            } else if (D_000967DD) {
                if (D_00096818 == 0)
                    mode = 2;
                else
                    mode = 1;
            } else if (D_000C6F04[65] == 0xFF0) {
                D_00096818 = 0;
                mode = 2;
            } else {
                D_00096818 = 1;
                mode = 1;
            }
            ret = func_0006C964(mode);
            if (ret == 2) {
                D_000970A8 = 0;
                *(int *)&D_0009763C = 0;
                ret = 0;
                if (D_000967F4 == 5)
                    D_000C6F04[65] = 0xFF0;
            } else if (ret == 3) {
                D_00096899 = 1;
                D_00096928 = D_0009709C;
                D_0009709C = 0;
                ret = 1;
            } else if (ret == 1) {
                if (D_000967F4 > 16 || D_000967F4 == 5) {
                    if (D_00096854) {
                        if (D_000970A8) {
                            saved1 = pauseflag;
                            D_00096899 = 0;
                            pauseflag = 1;
                            func_0005071C(0, 0xFF, 8);
                            WaitScreenFade();
                            SetDispMask(0);
                            pauseflag = saved1;
                            *(int *)&D_00097534 = 0;
                            D_0009709C = 1;
                            ret = 0;
                        } else {
                            saved2 = pauseflag;
                            D_00096899 = 0;
                            pauseflag = 1;
                            func_0005071C(0, 0xFF, 8);
                            WaitScreenFade();
                            SetDispMask(0);
                            pauseflag = saved2;
                            *(int *)&D_00097534 = 0;
                            D_0009709C = 0;
                        }
                    }
                    saved3 = pauseflag;
                    D_00096898 = 0;
                    D_00096899 = 0;
                    pauseflag = 1;
                    func_0005071C(0, 0xFF, 8);
                    WaitScreenFade();
                    SetDispMask(0);
                    pauseflag = saved3;
                    *(int *)&D_00097534 = 0;
                    D_0009709C = 0;
                } else {
                    if (D_00096858 && D_00096854 && D_00096874) {
                        D_0009709C = 3;
                        ret = 0;
                    } else if (!D_00096858 || (D_00096854 && !D_00096874)) {
                        D_0009709C = 1;
                        ret = 0;
                    } else if (D_00096858 && D_00096874 && !D_00096854) {
                        D_0009709C = 3;
                        ret = 0;
                    } else {
                        D_0009709C = 1;
                        ret = 0;
                    }
                }
            }
            break;
        case 2:
            *(int *)&D_0009763C = 0;
            ret = 0;
            if (D_000967F4 == 5 || D_000967F4 >= 17) {
                D_000967C4 = -1;
                D_000C6F04[65] = 0xFF0;
            }
            break;
        case 3:
            if (D_000967F4 == 0x16)
                D_00096898 = 0;
            saved4 = pauseflag;
            D_0009709C = 0;
            pauseflag = 1;
            func_0005071C(0, 0xFF, 8);
            WaitScreenFade();
            SetDispMask(0);
            *(volatile int *)&D_00097534 = 0;
            CameraCnt = *(volatile int *)&D_00097534;
            pauseflag = saved4;
            D_00096899 = 0;
            D_0009709C = 0;
            break;
        case 4:
            saved5 = pauseflag;
            pauseflag = 1;
            func_0005071C(0, 0xFF, 8);
            WaitScreenFade();
            SetDispMask(0);
            pauseflag = saved5;
            D_00096899 = 1;
            D_00096928 = D_0009709C;
            D_0009709C = 0;
            ret = 1;
            break;
        case 6:
            *(int *)&D_0009763C = 0;
            ret = 0;
            break;
        case 7:
            func_000739B4(0x100, 0x78);
            ret = 0;
            D_00096800 = 0;
            D_0008AA38[0] = 0x1999;
            break;
        default:
            ret = 0;
            break;
        }
        D_000967BC = 0;
    } while (ret == 1);
    if (D_0009709C != 7) {
        saved6 = pauseflag;
        pauseflag = 1;
        func_0005071C(0, 0xFF, 8);
        WaitScreenFade();
        SetDispMask(0);
        pauseflag = saved6;
    }
    *(int *)&D_00096780 = 0;
    *(int *)&D_0009677C = 0;
    D_00096884 = 0;
    D_00096899 = 0;
    SetDispMask(0);
    func_00051374();
    r.x = 0;
    r.y = 0;
    r.w = 0x200;
    r.h = 0x200;
    func_00074374(&r, 0, 0, 0);
    DrawSync(0);
    SetDispMask(1);
}
