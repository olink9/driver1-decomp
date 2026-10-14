/* DoAutoDirect (Driver 2 name) */
typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct { short x, y; } WAYREC;
typedef struct { unsigned char v; char pad[7]; } B8;

extern int D_0009763C;          /* AutoDirect */
extern int D_00097648;
extern long long D_00096878;    /* gStopPadReads */
extern char D_00097658;         /* tracking_car */
extern int D_00097620;          /* way_distance */
extern int D_0009762C;          /* TimeToWay */
extern WAYREC *D_0009765C;      /* PlayerWayRecordPtr */
extern B8 D_00096897;           /* cameraview */
extern long long D_00096A6C;    /* gCameraAngle */
extern int D_0008A1E0[];        /* gCameraOffset.vy */
extern long long D_00096A60;    /* gCameraDistance */
extern long long D_00096A64;    /* gCameraMaxDistance */
extern long long D_0009688C;    /* cameraCarId */
extern int *D_00097668;         /* ReplayParameterPtr */
extern long long D_000970D8;    /* CameraCnt */
extern VECTOR D_0008A8F0;
extern int D_0008A8F4[];       /* MainPlayer.cameraPos */
extern long long D_00096834;    /* playerType */
extern char D_0009B16C[];       /* car_data */
extern int D_00097630;          /* cammapht2 */
extern long long D_000967A0;    /* savemapinfo */
extern int D_0009767C;          /* mapstuff */

extern int func_000776E4();     /* rand */
extern int func_0005BC04();
extern int func_0001F5B0();     /* MapHeight */
extern int func_0005BDF8();     /* valid_region */
extern int func_0003C998();     /* CameraCollisionCheck */
extern int func_0005C5B0();     /* NoMoreCamerasErrorMessage */
extern void func_00058B2C();    /* RecordCamera */

#define cameraview D_00096897.v
#define gCameraAngle (*(int *)&D_00096A6C)
#define gCameraDistance (*(int *)&D_00096A60)
#define gCameraMaxDistance (*(int *)&D_00096A64)
#define cameraCarId (*(int *)&D_0009688C)
#define CameraCnt (*(int *)&D_000970D8)
#define savemapinfo (*(unsigned int **)&D_000967A0)

void func_0005B784(void)
{
    VECTOR pos;
    int height;
    int id;

    if ((D_0009763C != 0 || D_00097648 != 0) && *(int *)&D_00096878 == 0) {
        D_00097658 = 1;

        if (D_0009762C == D_00097620 / 2 + 15 &&
            D_0009765C->x != 0x7fff && D_0009765C->y != 0x7fff) {
            switch (func_000776E4() & 7) {
            case 0:
                cameraview = 0;
                gCameraAngle = func_000776E4() & 0xfff;
                D_0008A1E0[0] = -((func_000776E4() & 0xff) + 100);
                gCameraDistance = (func_000776E4() & 0x7e7) + 1000;
                gCameraMaxDistance = gCameraDistance;
                break;
            case 1:
            case 2:
                cameraview = 1;
                break;
            case 3:
            case 4:
                if ((id = func_0005BC04(0)) != 0) {
                    cameraCarId = id;
                    cameraview = 2;
                } else {
                    cameraview = 1;
                }
                break;
            case 5:
                cameraCarId = func_0005BC04(cameraCarId);
                cameraview = 2;
                break;
            case 6:
            case 7:
                cameraview = 5;
                break;
            }

            if (CameraCnt + D_00097620 > *D_00097668) {
                cameraview = 0;
                cameraCarId = 0;
            } else {
                pos.vx = D_0008A8F0.vx = D_0009765C->x << 10;
                pos.vz = D_0008A8F0.vz = D_0009765C->y << 10;
            }

            if (*(int *)&D_00096834 != 2)
            {
                height = func_0001F5B0(&D_0008A8F0);
                D_0008A8F0.vy = -70 - (func_000776E4() & 0x3ff) - height;
            }
            else
                D_0008A8F4[0] = -200;

            height = func_0001F5B0(D_0009B16C + cameraCarId * 396);
            if (-height < D_0008A8F0.vy)
                D_0008A8F0.vy = -height - 100;

            D_00097630 = func_0001F5B0(&D_0008A8F0);
            D_00097630 = -D_00097630;
            if (D_00097630 < D_0008A8F0.vy)
                D_0008A8F0.vy = D_00097630 - 100;

            func_0001F5B0(&D_0008A8F0);

            if (savemapinfo != 0)
                D_0009767C = *savemapinfo;

            if (func_0005BDF8(D_0009765C->x << 10, D_0009765C->y << 10) == 0 ||
                (cameraview != 2 && cameraview != 0 &&
                 savemapinfo != 0 && (*savemapinfo & 0x3c000000) == 0x3c000000) ||
                func_0003C998() != 0) {
                cameraview = func_000776E4() & 2;
                if (cameraview == 0) {
                    gCameraAngle = func_000776E4() & 0xfff;
                    D_0008A1E0[0] = -((func_000776E4() & 0xff) + 100);
                    gCameraDistance = (func_000776E4() & 0x7e7) + 1000;
                    gCameraMaxDistance = gCameraDistance;
                }
                cameraCarId = 0;
            }

            if (func_0005C5B0() == 0) {
                func_00058B2C(CameraCnt);
            } else {
                cameraview = 0;
                gCameraAngle = func_000776E4() & 0xfff;
                D_0008A1E0[0] = -((func_000776E4() & 0xff) + 100);
                gCameraDistance = (func_000776E4() & 0x7e7) + 1000;
                gCameraMaxDistance = gCameraDistance;
            }

            D_0009765C->x = 0x7fff;
            D_0009765C->y = 0x7fff;
        }
    }

    if (D_0009762C == 0) {
        D_0009762C = D_00097620;
        D_0009765C++;
    } else
        D_0009762C--;
}
