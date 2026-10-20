/* Driver 2: ControlReplay (DIRECTOR.C) */
typedef struct PLAYBACKCAMERA {
    int px, py, pz;                 /* 0x00 position */
    short ax, ay, az, apad;         /* 0x0C angle */
    int FrameCnt;                   /* 0x14 */
    short scr_z;                    /* 0x18 */
    short s1a, s1c, s1e;            /* 0x1A..0x1E */
    unsigned char cameraview;       /* 0x20 */
    unsigned char next;             /* 0x21 */
    unsigned char prev;             /* 0x22 */
    unsigned char idx;              /* 0x23 */
} PLAYBACKCAMERA;

typedef struct { short type; short height; short objectAngle; short pad; } ROUTE_DATA;
typedef struct { short shape_flags; unsigned short flags2; } MODEL;
typedef struct { int RecordingEnd; } REPLAY_PARAMETER_BLOCK;
typedef struct { char pad[0xC8]; int x; int y; int z; } CARP;
typedef struct { int f0; int f4; int f8; char pad[0x74]; } DB80;
typedef struct { short v; short pad[3]; } S8;
typedef struct { int vx, vy, vz, pad; } VEC;

extern int D_0009763C;                  /* quick_replay */
extern long long D_00096844;            /* AttractMode */
extern long long D_0009752C;            /* padd */
extern long long D_000970D8;            /* CameraCnt */
extern REPLAY_PARAMETER_BLOCK *D_00097668;  /* ReplayParameterPtr */
extern volatile long long D_00097540;            /* vblcounter */
extern long long D_0009709C;
extern long long D_0009687C;            /* pauseflag */
extern int D_0009760C;
extern int D_00096E18;                  /* debounce */
extern int D_00096E1C;                  /* first_time */
extern int D_0009761C;                  /* DirectorMenuActive */
extern int D_00096DC4, D_00096DA4;
extern int D_00096DA8;                  /* EditMode */
extern PLAYBACKCAMERA *D_00096DB4;      /* LastChange */
extern PLAYBACKCAMERA *D_00096DB0;      /* ThisChange */
extern PLAYBACKCAMERA *D_00097608;      /* PlaybackCamera */
extern long long D_00096A60, D_00096A64, D_00096A6C;
extern VEC D_0008A1DC;                  /* gCameraOffset */
extern int D_0008A1E0[];
extern int D_0008A8F0[];                /* cameraPos */
extern long long D_00096834;
extern MODEL *D_000A0E20[];             /* modelpointers */
extern int D_0009B170[];
extern int D_0009B16C[];
extern CARP D_0009B0A4;
extern unsigned char D_00097658;        /* tracking_car */
extern S8 D_00096C48, D_00096C4A, D_00096C4C;   /* camera_angle */
extern short D_0008EAA8[];              /* rcossin_tbl */
extern long long D_00096897;            /* cameraview */
extern int D_00097624;                  /* CursorX */
extern int D_00097628;                  /* CursorY */
extern long long D_00096F1C;
extern int D_00097650;                  /* MenuOffset */
extern int D_00097618;                  /* PlayMode */
extern int D_00097678;                  /* ReplayContinue */
extern int D_00097648;                  /* AutoDirect */
extern int D_0009764C;                  /* frame_advance_count */
extern int D_00096DC0;                  /* editing_existing_camera */
extern long long D_0009688C;
extern int D_00097670;
extern long long D_00097530, D_00097524;
extern DB80 D_000BDFA4[];
extern char D_00010E2C[];
extern char D_00096E20[];

extern int func_0001F5B0();             /* MapHeight */
extern void func_000217C8();            /* ROADS_GetRouteData */
extern int SquareRoot0();
extern int ratan2();
extern int func_0005C458();             /* dist */
extern int func_0003C998();             /* CameraCollisionCheck */
extern int func_0003DEE0();             /* OK_To_Zoom */
extern int func_0005C660();             /* FirstCamera */
extern void func_00058B2C();            /* RecordCamera */
extern int func_0005C5B0();             /* NoMoreCamerasErrorMessage */
extern void func_00058A84();            /* setCamera */
extern int func_0005BC04();             /* SelectCameraCar */
extern void func_00074158();            /* SetDispMask */
extern void func_00033AF8();
extern void func_000513D4();
extern void DrawSync();
extern void func_00050E28();            /* Loadfile */
extern void func_EC280();
extern void func_E7508();
extern void func_0003A068();
extern int printf();

#define padd (*(short *)&D_0009752C)
#define CameraCnt (*(int *)&D_000970D8)
#define pauseflag (*(int *)&D_0009687C)
#define CAMVIEW (*(unsigned char *)&D_00096897)
#define current (*(DB80 **)&D_00097530)
#define last (*(DB80 **)&D_00097524)

static inline PLAYBACKCAMERA *FindFreeCamera(void)
{
    int i;
    for (i = 0; i < 60; i++)
        if (D_00097608[i].FrameCnt == 100000)
            return &D_00097608[i];
    return 0;
}

static inline void setCamera(PLAYBACKCAMERA *Change)
{
    int pad = Change->apad;
    int x = Change->px;
    CAMVIEW = Change->cameraview & 7;
    D_00097658 = (Change->cameraview & 8) >> 3;
    D_0008A8F0[0] = x;
    D_0008A8F0[1] = Change->py;
    D_0008A8F0[2] = Change->pz;
    D_00096C48.v = Change->ax;
    D_00096C4A.v = Change->ay;
    D_00096C4C.v = Change->az;
    *(int *)&D_0009688C = pad;
    *(int *)&D_00096A60 = Change->s1a;
    *(int *)&D_00096A64 = Change->s1c;
    *(int *)&D_00096A6C = Change->s1e;
    D_0008A1E0[0] = Change->scr_z;
}

static inline void EditCamera(void)
{
    D_00096DB0 = D_00096DB4;
    D_00096DB4->cameraview = (CAMVIEW & 7) | (D_00097658 << 3);
    D_00096DB0->px = D_0008A8F0[0];
    D_00096DB0->py = D_0008A8F0[1];
    D_00096DB0->pz = D_0008A8F0[2];
    D_00096DB0->ax = D_00096C48.v;
    D_00096DB0->ay = D_00096C4A.v;
    D_00096DB0->az = D_00096C4C.v;
    D_00096DB0->apad = *(int *)&D_0009688C;
    D_00096DB0->s1a = *(int *)&D_00096A60;
    D_00096DB0->s1c = *(int *)&D_00096A64;
    D_00096DB0->s1e = *(int *)&D_00096A6C;
    D_00096DB0->scr_z = D_0008A1E0[0];
}

static inline void DeleteCurrentCamera(void)
{
    int prev;
    int next;

    prev = D_00096DB4->prev;
    next = D_00096DB4->next;
    if (prev != 0xFF) {
        D_00097608[prev].next = next;
        D_00097608[next].prev = prev;
        D_00096DB4->FrameCnt = 100000;
        func_00058A84(D_00097608 + prev);
        D_00096DB4 = D_00097608 + prev;
    }
}

void func_00059D94(void)
{
    int move;

    move = 0;

    if (D_0009763C || *(int *)&D_00096844) {
        if ((padd != 0 || CameraCnt >= D_00097668->RecordingEnd) && CameraCnt > 2) {
            D_0009760C = *(volatile int *)&D_00097540;
            *(int *)&D_0009709C = 4;
            pauseflag = 1;
        } else
            pauseflag = 0;
        return;
    }

    if ((padd & 0x8000) && D_00096E18 == 0) {
        move = 2;
        D_00096E18 = 1;
    }
    if ((padd & 0x2000) && D_00096E18 == 0) {
        move = 1;
        D_00096E18 = 1;
    }
    if ((padd & 0x1000) && D_00096E18 == 0) {
        move = 3;
        D_00096E18 = 1;
    }
    if ((padd & 0x4000) && D_00096E18 == 0) {
        move = 4;
        D_00096E18 = 1;
    }
    if ((padd & 0x40) && D_00096E18 == 0) {
        if (D_0009761C == 0)
            pauseflag = 1;
        else
            move = 5;
        D_00096DC4 = 0;
        D_00096DA4 = 0;
        D_00096E18 = 1;
    }
    if ((padd & 0x8000) == 0 && (padd & 0x7040) == 0)
        D_00096E18 = 0;

    if (D_00096DA8 != 0 && move != 5) {
        int speed;

        speed = 1;
        if (padd & 8)
            speed = 8;

        switch (D_00096DA8) {
        case 1:
            if ((padd & 0x2000) && D_00096DB4->FrameCnt < CameraCnt)
                D_00096DB4->FrameCnt++;
            if ((padd & 0x8000) && D_00097608[D_00096DB4->prev].FrameCnt < D_00096DB4->FrameCnt)
                D_00096DB4->FrameCnt--;
            break;
        case 2:
            if ((padd & 0x1000) && *(int *)&D_00096A60 > 1000) {
                *(int *)&D_00096A60 -= speed * 16;
                *(int *)&D_00096A64 -= speed * 16;
            }
            if ((padd & 0x4000) && *(int *)&D_00096A60 < 4000) {
                *(int *)&D_00096A60 += speed * 16;
                *(int *)&D_00096A64 += speed * 16;
            }
            if (padd & 0x8000)
                *(int *)&D_00096A6C += speed * 16;
            if (padd & 0x2000)
                *(int *)&D_00096A6C -= speed * 16;
            if ((padd & 4) && D_0008A1DC.vy > -1200)
                D_0008A1DC.vy -= speed * 16;
            if ((padd & 1) && D_0008A1DC.vy < -80)
                D_0008A1DC.vy += speed * 16;
            {
                ROUTE_DATA routeData;
                int road_height;

                road_height = -func_0001F5B0(D_0008A8F0);
                func_000217C8(D_0008A8F0[0], D_0008A8F0[2], &routeData);
                if (*(int *)&D_00096834 == 2 || (D_000A0E20[routeData.type]->flags2 & 0x100)) {
                    int py = D_0009B170[0];
                    D_0008A8F0[1] = D_0008A1DC.vy - py;
                    if (D_0008A8F0[1] < road_height - 450) {
                        D_0008A8F0[1] = road_height - 450;
                        D_0008A1DC.vy = D_0008A8F0[1] + py;

                    }
                }
            }
            break;
        case 3:
        {
            VEC old_camera;
            int x;
            int z;
            int d;
            int angle;
            int dx;
            int dz;
            int height;

            dx = D_0009B0A4.x - D_0008A8F0[0];
            dz = D_0009B0A4.z - D_0008A8F0[2];
            z = x = 0;
            old_camera.vx = D_0008A8F0[0];
            old_camera.vz = D_0008A8F0[2];
            old_camera.vy = D_0008A8F0[1];
            d = SquareRoot0(dx * dx + dz * dz);
            if (D_00097658)
                angle = ratan2(dz, dx);
            else
                angle = D_00096C4A.v;

            if ((padd & 0x1000) && (D_00097658 == 0 || d > 500))
                x = -speed * 16;
            if (padd & 0x4000)
                x = speed * 16;
            if ((padd & 0x8000) && (D_00097658 == 0 || d > 500))
                z = -speed * 16;
            if (padd & 0x2000)
                z = speed * 16;

            if (D_00097658 == 0) {
                int t = x;
                x = -z;
                z = t;
            }


            D_0008A8F0[0] += (z * D_0008EAA8[(angle & 0xfff) * 2]) >> 12;
            D_0008A8F0[2] -= (z * D_0008EAA8[(angle & 0xfff) * 2 + 1]) >> 12;
            D_0008A8F0[0] -= (x * D_0008EAA8[(angle & 0xfff) * 2 + 1]) >> 12;
            D_0008A8F0[2] -= (x * D_0008EAA8[(angle & 0xfff) * 2]) >> 12;

            if (func_0005C458(D_0009B16C, D_0008A8F0) >= 19001) {
                D_0008A8F0[0] = old_camera.vx;
                D_0008A8F0[1] = old_camera.vy;
                D_0008A8F0[2] = old_camera.vz;
            } else {
                if (func_0003C998()) {
                    z = z / speed * 20;
                    x = x / speed * 20;
                    D_0008A8F0[1] = old_camera.vy;
                    D_0008A8F0[0] = old_camera.vx + ((z * D_0008EAA8[(angle & 0xfff) * 2]) >> 12);
                    D_0008A8F0[2] = old_camera.vz - ((z * D_0008EAA8[(angle & 0xfff) * 2 + 1]) >> 12);
                    D_0008A8F0[0] -= (x * D_0008EAA8[(angle & 0xfff) * 2 + 1]) >> 12;
                    D_0008A8F0[2] -= (x * D_0008EAA8[(angle & 0xfff) * 2]) >> 12;
                    if (func_0003C998()) {
                        D_0008A8F0[0] = old_camera.vx;
                        D_0008A8F0[2] = old_camera.vz;
                    }
                }
            }

            height = -func_0001F5B0(D_0008A8F0);
            if (padd & 4)
                D_0008A8F0[1] -= speed * 16;
            if (padd & 1)
                D_0008A8F0[1] += speed * 16;
            if (D_0008A8F0[1] > height - 250)
                D_0008A8F0[1] = height - 250;
            if (D_0008A8F0[1] < height - 1500)
                D_0008A8F0[1] = height - 1500;
            {
                ROUTE_DATA routeData;

                func_000217C8(D_0008A8F0[0], D_0008A8F0[2], &routeData);
                if (*(int *)&D_00096834 == 2 || (D_000A0E20[routeData.type]->flags2 & 0x100)) {
                    if (D_0008A8F0[1] < height - 450)
                        D_0008A8F0[1] = height - 450;
                }
            }
            if (CAMVIEW == 1 && func_0003DEE0() == 0)
                CAMVIEW = 5;
            break;
        }
        case 4:
            if (padd & 0x1000) {
                D_00096C48.v += speed * 4;
                if ((unsigned short)(D_00096C48.v - 300) < 1749)
                    D_00096C48.v = 300;
            }
            if (padd & 0x4000) {
                D_00096C48.v -= speed * 4;
                if ((unsigned short)(D_00096C48.v - 0x800) < 1853)
                    D_00096C48.v = 3900;
            }
            if (padd & 0x8000)
                D_00096C4A.v += speed * 4;
            if (padd & 0x2000)
                D_00096C4A.v -= speed * 4;
            D_00096C48.v &= 0xfff;
            D_00096C4A.v &= 0xfff;
            D_00096C4C.v &= 0xfff;
            break;
        }
        move = 0;
    }

    switch (move) {
    case 1:
        D_00097624++;
        if (CameraCnt >= D_00097668->RecordingEnd && D_0009761C == 1 && D_00097624 == 5)
            D_00097624 = 7;
        else if (FindFreeCamera() == 0 && D_0009761C == 1 && D_00097624 == 5)
            D_00097624 = 6;
        if (D_00097624 == 7 && D_0009761C == 1 && *(unsigned char *)&D_00096F1C)
            D_00097624 = 8;
        break;
    case 2:
        D_00097624--;
        if (D_00097624 == 7 && D_0009761C == 1 && *(unsigned char *)&D_00096F1C)
            D_00097624 = 6;
        if (CameraCnt >= D_00097668->RecordingEnd && D_0009761C == 1 && D_00097624 == 6)
            D_00097624 = 4;
        else if (FindFreeCamera() == 0 && D_0009761C == 1 && D_00097624 == 5)
            D_00097624 = 4;
        break;
    case 3:
        D_00097628--;
        if (func_0005C660() && D_0009761C == 2 && D_00097650 && D_00097628 == 5)
            D_00097628 = 3;
        break;
    case 4:
        D_00097628++;
        if (func_0005C660() && D_0009761C == 2 && D_00097650 && D_00097628 == 4)
            D_00097628 = 6;
        break;
    }

    if (D_00096E18 == 0 && D_00097618 == 3)
        pauseflag = 1;

    if (pauseflag != 0) {
    if (D_00096E1C) {
        if (CameraCnt >= D_00097668->RecordingEnd && (D_00097624 < 4 || D_00097624 == 5))
            D_00097624 = 4;
        D_0009761C = 1;
    }
    D_00096E1C = 0;
    D_00097678 = 0;

    switch (D_0009761C) {
    case 1:
        D_00097628 = 0;
        if (CameraCnt < D_00097668->RecordingEnd) {
            if (D_00097624 < 0)
                D_00097624 = 8;
            if (D_00097624 > 8)
                D_00097624 = 0;
        } else {
            if (D_00097624 < 4)
                D_00097624 = 8;
            if (D_00097624 > 8)
                D_00097624 = 4;
        }
        switch (D_00097624) {
        case 0:
            if (move == 5 && CameraCnt < D_00097668->RecordingEnd) {
                pauseflag = 0;
                D_00097678 = 1;
                D_00097618 = 0;
                D_00097648 = 0;
                *(int *)&D_00097540 = 0;
            }
            break;
        case 1:
            if (move == 5 && CameraCnt < D_00097668->RecordingEnd) {
                pauseflag = 0;
                D_00097678 = 1;
                D_00097618 = 0;
                D_00097648 = 1;
            }
            break;
        case 2:
            if (move == 5 && CameraCnt < D_00097668->RecordingEnd) {
                D_00097678 = 1;
                D_00097618 = 1;
                pauseflag = 0;
                D_00096DA4 = 3;
                D_00097648 = 0;
            }
            break;
        case 3:
            if (CameraCnt < D_00097668->RecordingEnd) {
                if (move == 5) {
                    D_00097618 = 2;
                    pauseflag = 0;
                    D_0009764C = 10;
                    D_00097648 = 0;
                } else if (padd & 0x40) {
                    if (D_0009764C != 0)
                        D_0009764C--;
                    else {
                        D_00097678 = 1;
                        pauseflag = 0;
                        D_00097618 = 3;
                    }
                }
            }
            break;
        case 4:
            if (move == 5) {
                *(int *)&D_0009709C = 5;
                pauseflag = 0;
                D_00097618 = 0;
            }
            break;
        case 5:
            if (CameraCnt < D_00097668->RecordingEnd && move == 5) {
                D_0009761C = 2;
                D_00097650 = 0;
                D_00096DC0 = 0;
                D_00097628++;
            }
            break;
        case 6:
            if (move == 5) {
                D_0009761C = 2;
                D_00097650 = 40;
                D_00096DC0 = 1;
                D_00097628++;
            }
            break;
        case 7:
            if (move == 5) {
                func_00074158(0);
                func_00033AF8();
                func_000513D4();
                func_000513D4();
                DrawSync(0);
                func_00050E28(D_00010E2C, 0xE6600);
                func_EC280(1);
                current = &D_000BDFA4[0];
                last = &D_000BDFA4[1];
                current->f0 = current->f8;
                last->f0 = last->f8;
                func_E7508(5, D_00097670, 0x3E00);
                current = &D_000BDFA4[0];
                last = &D_000BDFA4[1];
                func_0003A068();
                func_00033AF8();
            }
            break;
        case 8:
            if (move == 5) {
                *(int *)&D_0009709C = 4;
                pauseflag = 0;
                D_00097618 = 0;
            }
            break;
        }
        break;
    case 2:
        D_00097624 = 5;
        switch (D_00097628) {
        case 0:
            if (D_00097650)
                D_00097624 = 6;
            D_0009761C = 1;
            if (D_00096DB4)
                setCamera(D_00096DB4);
            break;
        case 1:
            if (move == 5) {
                D_0009761C = 3;
                CAMVIEW = 2;
                D_00097624++;
            }
            break;
        case 2:
            if (move == 5) {
                D_0009761C = 4;
                *(int *)&D_00096A60 = 1600;
                *(int *)&D_00096A64 = 1600;
                *(int *)&D_00096A6C = 2048;
                D_0008A1DC.vx = -45;
                D_0008A1DC.vy = -380;
                CAMVIEW = 0;
                D_0008A1DC.vz = -125;

                D_00097624++;
            }
            break;
        case 3:
            if (move == 5) {
                if (CAMVIEW == 2) {
                    D_0008A8F0[1] = -(D_0009B0A4.y + 500);
                    D_0008A8F0[0] = D_0009B0A4.x;
                    D_0008A8F0[2] = D_0009B0A4.z;
                }
                if (CAMVIEW != 5 && CAMVIEW != 1)
                    CAMVIEW = 1;
                D_0009761C = 5;
                D_00097624++;
            }
            break;
        case 4:
            if (D_00097650) {
                if (D_00096DB4->prev != 0xFF && move == 5) {
                    if (D_00096DA8) {
                        int prev = D_00096DB4->prev;
                        if (D_00096DB4->FrameCnt == D_00097608[prev].FrameCnt) {
                            int pp = D_00097608[prev].prev;
                            printf(D_00096E20);
                            if (pp != 0xFF) {
                                D_00097608[pp].next = D_00097608[D_00096DB4->prev].next;
                                D_00096DB4->prev = pp;
                                D_00097608[prev].FrameCnt = 100000;
                            }
                        }
                        D_00096DA8 = 0;
                    } else
                        D_00096DA8 = 1;
                }
                break;
            }
            if (move == 5) {
                D_0009761C = 1;
                if (D_00096DC0 == 0) {
                    func_00058B2C(CameraCnt);
                    if (func_0005C5B0())
                        D_00097624 = 6;
                } else
                    EditCamera();
            }
            break;
        case 5:
            if (move == 5) {
                D_00097624 = 6;
                DeleteCurrentCamera();
                if (func_0005C660)
                    D_00097628 = 6;
            }
            break;
        case 6:
            if (move == 5) {
                D_00097624 = 6;
                D_0009761C = 1;
                if (D_00096DC0)
                    EditCamera();
                else
                    func_00058B2C(CameraCnt);
            }
            break;
        }
        if (D_00097650) {
            if (D_00097628 > 6)
                D_00097628 = 6;
        } else {
            if (D_00097628 > 4)
                D_00097628 = 4;
        }
        break;
    case 3:
        D_00097628 = 1;
        switch (D_00097624 - 5) {
        case 0:
            D_0009761C = 2;
            break;
        case 1:
            if (move == 5)
                *(int *)&D_0009688C = func_0005BC04(*(int *)&D_0009688C);
            break;
        }
        if (D_00097624 > 6)
            D_00097624 = 6;
        break;
    case 4:
        D_00097628 = 2;
        switch (D_00097624 - 5) {
        case 0:
            D_0009761C = 2;
            break;
        case 1:
            if (move == 5) {
                if (D_00096DA8)
                    D_00096DA8 = 0;
                else
                    D_00096DA8 = 2;
            }
            break;
        }
        if (D_00097624 > 6)
            D_00097624 = 6;
        break;
    case 5:
        D_00097628 = 3;
        switch (D_00097624) {
        case 5:
            D_0009761C = 2;
            break;
        case 6:
            if (move == 5) {
                if (D_00096DA8 == 0)
                    D_00096DA8 = 3;
                else
                    D_00096DA8 = 0;
            }
            break;
        case 7:
            if (move == 5)
                D_00097658 = 1;
            break;
        case 8:
            if (move == 5) {
                D_00097658 = 0;
                if (D_00096DA8)
                    D_00096DA8 = 0;
                else
                    D_00096DA8 = 4;
            }
            break;
        case 9:
            if (D_00097658 && move == 5) {
                if (CAMVIEW == 1)
                    CAMVIEW = 5;
                else
                    CAMVIEW = 1;
            }
            break;
        }
        if (D_00097658 && func_0003DEE0()) {
            if (D_00097624 > 8)
                D_00097624 = 9;
        } else {
            if (D_00097624 > 7)
                D_00097624 = 8;
        }
        break;
    }
    } else {
        D_00096E1C = 1;
        D_0009761C = 0;
        if (D_00097618 == 2)
            pauseflag = 1;
    }
}
