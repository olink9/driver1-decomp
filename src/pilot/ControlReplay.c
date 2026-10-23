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
extern long long AttractMode;            /* AttractMode */
extern long long D_0009752C;            /* padd */
extern long long CameraCnt;            /* CameraCnt */
extern REPLAY_PARAMETER_BLOCK *ReplayParameterPtr;  /* ReplayParameterPtr */
extern volatile long long vblcounter;            /* vblcounter */
extern long long D_0009709C;
extern long long pauseflag;            /* pauseflag */
extern int D_0009760C;
extern int D_00096E18;                  /* debounce */
extern int D_00096E1C;                  /* first_time */
extern int DirectorMenuActive;                  /* DirectorMenuActive */
extern int FastForward, D_00096DA4;
extern int D_00096DA8;                  /* EditMode */
extern PLAYBACKCAMERA *LastChange;      /* LastChange */
extern PLAYBACKCAMERA *D_00096DB0;      /* ThisChange */
extern PLAYBACKCAMERA *D_00097608;      /* PlaybackCamera */
extern long long D_00096A60, D_00096A64, D_00096A6C;
extern VEC D_0008A1DC;                  /* gCameraOffset */
extern int D_0008A1E0[];
extern int camera_position[];                /* cameraPos */
extern long long D_00096834;
extern MODEL *modelpointers[];             /* modelpointers */
extern int D_0009B170[];
extern int D_0009B16C[];
extern CARP car_data;
extern unsigned char tracking_car;        /* tracking_car */
extern S8 D_00096C48, D_00096C4A, D_00096C4C;   /* camera_angle */
extern short rcossin_tbl[];              /* rcossin_tbl */
extern long long cameraview;            /* cameraview */
extern int CursorX;                  /* CursorX */
extern int CursorY;                  /* CursorY */
extern long long D_00096F1C;
extern int D_00097650;                  /* MenuOffset */
extern int D_00097618;                  /* PlayMode */
extern int D_00097678;                  /* ReplayContinue */
extern int D_00097648;                  /* AutoDirect */
extern int D_0009764C;                  /* frame_advance_count */
extern int D_00096DC0;                  /* editing_existing_camera */
extern long long D_0009688C;
extern int D_00097670;
extern long long current, D_00097524;
extern DB80 D_000BDFA4[];
extern char D_00010E2C[];
extern char D_00096E20[];

extern int MapHeight();
extern void ROADS_GetRouteData();
extern int SquareRoot0();
extern int ratan2();
extern int dist();
extern int CameraCollisionCheck();
extern int OK_To_Zoom();
extern int FirstCamera();
extern void RecordCamera();
extern int NoMoreCamerasErrorMessage();
extern void setCamera();
extern int SelectCameraCar();
extern void SetDispMask();
extern void func_00033AF8();
extern void func_000513D4();
extern void DrawSync();
extern void Loadfile();
extern void func_EC280();
extern void func_E7508();
extern void func_0003A068();
extern int printf();

#define padd (*(short *)&D_0009752C)
#define CameraCnt (*(int *)&CameraCnt)
#define pauseflag (*(int *)&pauseflag)
#define CAMVIEW (*(unsigned char *)&cameraview)
#define current (*(DB80 **)&current)
#define last (*(DB80 **)&D_00097524)

static inline PLAYBACKCAMERA *FindFreeCamera(void)
{
    int i;
    for (i = 0; i < 60; i++)
        if (D_00097608[i].FrameCnt == 100000)
            return &D_00097608[i];
    return 0;
}

static inline void setCameraInline(PLAYBACKCAMERA *Change)
{
    int pad = Change->apad;
    int x = Change->px;
    CAMVIEW = Change->cameraview & 7;
    tracking_car = (Change->cameraview & 8) >> 3;
    camera_position[0] = x;
    camera_position[1] = Change->py;
    camera_position[2] = Change->pz;
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
    D_00096DB0 = LastChange;
    LastChange->cameraview = (CAMVIEW & 7) | (tracking_car << 3);
    D_00096DB0->px = camera_position[0];
    D_00096DB0->py = camera_position[1];
    D_00096DB0->pz = camera_position[2];
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

    prev = LastChange->prev;
    next = LastChange->next;
    if (prev != 0xFF) {
        D_00097608[prev].next = next;
        D_00097608[next].prev = prev;
        LastChange->FrameCnt = 100000;
        setCamera(D_00097608 + prev);
        LastChange = D_00097608 + prev;
    }
}

void ControlReplay(void)
{
    int move;

    move = 0;

    if (D_0009763C || *(int *)&AttractMode) {
        if ((padd != 0 || CameraCnt >= ReplayParameterPtr->RecordingEnd) && CameraCnt > 2) {
            D_0009760C = *(volatile int *)&vblcounter;
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
        if (DirectorMenuActive == 0)
            pauseflag = 1;
        else
            move = 5;
        FastForward = 0;
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
            if ((padd & 0x2000) && LastChange->FrameCnt < CameraCnt)
                LastChange->FrameCnt++;
            if ((padd & 0x8000) && D_00097608[LastChange->prev].FrameCnt < LastChange->FrameCnt)
                LastChange->FrameCnt--;
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

                road_height = -MapHeight(camera_position);
                ROADS_GetRouteData(camera_position[0], camera_position[2], &routeData);
                if (*(int *)&D_00096834 == 2 || (modelpointers[routeData.type]->flags2 & 0x100)) {
                    int py = D_0009B170[0];
                    camera_position[1] = D_0008A1DC.vy - py;
                    if (camera_position[1] < road_height - 450) {
                        camera_position[1] = road_height - 450;
                        D_0008A1DC.vy = camera_position[1] + py;

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

            dx = car_data.x - camera_position[0];
            dz = car_data.z - camera_position[2];
            z = x = 0;
            old_camera.vx = camera_position[0];
            old_camera.vz = camera_position[2];
            old_camera.vy = camera_position[1];
            d = SquareRoot0(dx * dx + dz * dz);
            if (tracking_car)
                angle = ratan2(dz, dx);
            else
                angle = D_00096C4A.v;

            if ((padd & 0x1000) && (tracking_car == 0 || d > 500))
                x = -speed * 16;
            if (padd & 0x4000)
                x = speed * 16;
            if ((padd & 0x8000) && (tracking_car == 0 || d > 500))
                z = -speed * 16;
            if (padd & 0x2000)
                z = speed * 16;

            if (tracking_car == 0) {
                int t = x;
                x = -z;
                z = t;
            }


            camera_position[0] += (z * rcossin_tbl[(angle & 0xfff) * 2]) >> 12;
            camera_position[2] -= (z * rcossin_tbl[(angle & 0xfff) * 2 + 1]) >> 12;
            camera_position[0] -= (x * rcossin_tbl[(angle & 0xfff) * 2 + 1]) >> 12;
            camera_position[2] -= (x * rcossin_tbl[(angle & 0xfff) * 2]) >> 12;

            if (dist(D_0009B16C, camera_position) >= 19001) {
                camera_position[0] = old_camera.vx;
                camera_position[1] = old_camera.vy;
                camera_position[2] = old_camera.vz;
            } else {
                if (CameraCollisionCheck()) {
                    z = z / speed * 20;
                    x = x / speed * 20;
                    camera_position[1] = old_camera.vy;
                    camera_position[0] = old_camera.vx + ((z * rcossin_tbl[(angle & 0xfff) * 2]) >> 12);
                    camera_position[2] = old_camera.vz - ((z * rcossin_tbl[(angle & 0xfff) * 2 + 1]) >> 12);
                    camera_position[0] -= (x * rcossin_tbl[(angle & 0xfff) * 2 + 1]) >> 12;
                    camera_position[2] -= (x * rcossin_tbl[(angle & 0xfff) * 2]) >> 12;
                    if (CameraCollisionCheck()) {
                        camera_position[0] = old_camera.vx;
                        camera_position[2] = old_camera.vz;
                    }
                }
            }

            height = -MapHeight(camera_position);
            if (padd & 4)
                camera_position[1] -= speed * 16;
            if (padd & 1)
                camera_position[1] += speed * 16;
            if (camera_position[1] > height - 250)
                camera_position[1] = height - 250;
            if (camera_position[1] < height - 1500)
                camera_position[1] = height - 1500;
            {
                ROUTE_DATA routeData;

                ROADS_GetRouteData(camera_position[0], camera_position[2], &routeData);
                if (*(int *)&D_00096834 == 2 || (modelpointers[routeData.type]->flags2 & 0x100)) {
                    if (camera_position[1] < height - 450)
                        camera_position[1] = height - 450;
                }
            }
            if (CAMVIEW == 1 && OK_To_Zoom() == 0)
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
        CursorX++;
        if (CameraCnt >= ReplayParameterPtr->RecordingEnd && DirectorMenuActive == 1 && CursorX == 5)
            CursorX = 7;
        else if (FindFreeCamera() == 0 && DirectorMenuActive == 1 && CursorX == 5)
            CursorX = 6;
        if (CursorX == 7 && DirectorMenuActive == 1 && *(unsigned char *)&D_00096F1C)
            CursorX = 8;
        break;
    case 2:
        CursorX--;
        if (CursorX == 7 && DirectorMenuActive == 1 && *(unsigned char *)&D_00096F1C)
            CursorX = 6;
        if (CameraCnt >= ReplayParameterPtr->RecordingEnd && DirectorMenuActive == 1 && CursorX == 6)
            CursorX = 4;
        else if (FindFreeCamera() == 0 && DirectorMenuActive == 1 && CursorX == 5)
            CursorX = 4;
        break;
    case 3:
        CursorY--;
        if (FirstCamera() && DirectorMenuActive == 2 && D_00097650 && CursorY == 5)
            CursorY = 3;
        break;
    case 4:
        CursorY++;
        if (FirstCamera() && DirectorMenuActive == 2 && D_00097650 && CursorY == 4)
            CursorY = 6;
        break;
    }

    if (D_00096E18 == 0 && D_00097618 == 3)
        pauseflag = 1;

    if (pauseflag != 0) {
    if (D_00096E1C) {
        if (CameraCnt >= ReplayParameterPtr->RecordingEnd && (CursorX < 4 || CursorX == 5))
            CursorX = 4;
        DirectorMenuActive = 1;
    }
    D_00096E1C = 0;
    D_00097678 = 0;

    switch (DirectorMenuActive) {
    case 1:
        CursorY = 0;
        if (CameraCnt < ReplayParameterPtr->RecordingEnd) {
            if (CursorX < 0)
                CursorX = 8;
            if (CursorX > 8)
                CursorX = 0;
        } else {
            if (CursorX < 4)
                CursorX = 8;
            if (CursorX > 8)
                CursorX = 4;
        }
        switch (CursorX) {
        case 0:
            if (move == 5 && CameraCnt < ReplayParameterPtr->RecordingEnd) {
                pauseflag = 0;
                D_00097678 = 1;
                D_00097618 = 0;
                D_00097648 = 0;
                *(int *)&vblcounter = 0;
            }
            break;
        case 1:
            if (move == 5 && CameraCnt < ReplayParameterPtr->RecordingEnd) {
                pauseflag = 0;
                D_00097678 = 1;
                D_00097618 = 0;
                D_00097648 = 1;
            }
            break;
        case 2:
            if (move == 5 && CameraCnt < ReplayParameterPtr->RecordingEnd) {
                D_00097678 = 1;
                D_00097618 = 1;
                pauseflag = 0;
                D_00096DA4 = 3;
                D_00097648 = 0;
            }
            break;
        case 3:
            if (CameraCnt < ReplayParameterPtr->RecordingEnd) {
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
            if (CameraCnt < ReplayParameterPtr->RecordingEnd && move == 5) {
                DirectorMenuActive = 2;
                D_00097650 = 0;
                D_00096DC0 = 0;
                CursorY++;
            }
            break;
        case 6:
            if (move == 5) {
                DirectorMenuActive = 2;
                D_00097650 = 40;
                D_00096DC0 = 1;
                CursorY++;
            }
            break;
        case 7:
            if (move == 5) {
                SetDispMask(0);
                func_00033AF8();
                func_000513D4();
                func_000513D4();
                DrawSync(0);
                Loadfile(D_00010E2C, 0xE6600);
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
        CursorX = 5;
        switch (CursorY) {
        case 0:
            if (D_00097650)
                CursorX = 6;
            DirectorMenuActive = 1;
            if (LastChange)
                setCameraInline(LastChange);
            break;
        case 1:
            if (move == 5) {
                DirectorMenuActive = 3;
                CAMVIEW = 2;
                CursorX++;
            }
            break;
        case 2:
            if (move == 5) {
                DirectorMenuActive = 4;
                *(int *)&D_00096A60 = 1600;
                *(int *)&D_00096A64 = 1600;
                *(int *)&D_00096A6C = 2048;
                D_0008A1DC.vx = -45;
                D_0008A1DC.vy = -380;
                CAMVIEW = 0;
                D_0008A1DC.vz = -125;

                CursorX++;
            }
            break;
        case 3:
            if (move == 5) {
                if (CAMVIEW == 2) {
                    camera_position[1] = -(car_data.y + 500);
                    camera_position[0] = car_data.x;
                    camera_position[2] = car_data.z;
                }
                if (CAMVIEW != 5 && CAMVIEW != 1)
                    CAMVIEW = 1;
                DirectorMenuActive = 5;
                CursorX++;
            }
            break;
        case 4:
            if (D_00097650) {
                if (LastChange->prev != 0xFF && move == 5) {
                    if (D_00096DA8) {
                        int prev = LastChange->prev;
                        if (LastChange->FrameCnt == D_00097608[prev].FrameCnt) {
                            int pp = D_00097608[prev].prev;
                            printf(D_00096E20);
                            if (pp != 0xFF) {
                                D_00097608[pp].next = D_00097608[LastChange->prev].next;
                                LastChange->prev = pp;
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
                DirectorMenuActive = 1;
                if (D_00096DC0 == 0) {
                    RecordCamera(CameraCnt);
                    if (NoMoreCamerasErrorMessage())
                        CursorX = 6;
                } else
                    EditCamera();
            }
            break;
        case 5:
            if (move == 5) {
                CursorX = 6;
                DeleteCurrentCamera();
                if (FirstCamera)
                    CursorY = 6;
            }
            break;
        case 6:
            if (move == 5) {
                CursorX = 6;
                DirectorMenuActive = 1;
                if (D_00096DC0)
                    EditCamera();
                else
                    RecordCamera(CameraCnt);
            }
            break;
        }
        if (D_00097650) {
            if (CursorY > 6)
                CursorY = 6;
        } else {
            if (CursorY > 4)
                CursorY = 4;
        }
        break;
    case 3:
        CursorY = 1;
        switch (CursorX - 5) {
        case 0:
            DirectorMenuActive = 2;
            break;
        case 1:
            if (move == 5)
                *(int *)&D_0009688C = SelectCameraCar(*(int *)&D_0009688C);
            break;
        }
        if (CursorX > 6)
            CursorX = 6;
        break;
    case 4:
        CursorY = 2;
        switch (CursorX - 5) {
        case 0:
            DirectorMenuActive = 2;
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
        if (CursorX > 6)
            CursorX = 6;
        break;
    case 5:
        CursorY = 3;
        switch (CursorX) {
        case 5:
            DirectorMenuActive = 2;
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
                tracking_car = 1;
            break;
        case 8:
            if (move == 5) {
                tracking_car = 0;
                if (D_00096DA8)
                    D_00096DA8 = 0;
                else
                    D_00096DA8 = 4;
            }
            break;
        case 9:
            if (tracking_car && move == 5) {
                if (CAMVIEW == 1)
                    CAMVIEW = 5;
                else
                    CAMVIEW = 1;
            }
            break;
        }
        if (tracking_car && OK_To_Zoom()) {
            if (CursorX > 8)
                CursorX = 9;
        } else {
            if (CursorX > 7)
                CursorX = 8;
        }
        break;
    }
    } else {
        D_00096E1C = 1;
        DirectorMenuActive = 0;
        if (D_00097618 == 2)
            pauseflag = 1;
    }
}
