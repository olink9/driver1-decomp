/* DoAutoDirect (Driver 2 name) */
typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct { short x, y; } WAYREC;
typedef struct { unsigned char v; char pad[7]; } B8;

extern int D_0009763C;          /* AutoDirect */
extern int D_00097648;
extern long long D_00096878;    /* gStopPadReads */
extern char tracking_car;         /* tracking_car */
extern int D_00097620;          /* way_distance */
extern int D_0009762C;          /* TimeToWay */
extern WAYREC *D_0009765C;      /* PlayerWayRecordPtr */
extern B8 cameraview;           /* cameraview */
extern long long D_00096A6C;    /* gCameraAngle */
extern int D_0008A1E0[];        /* gCameraOffset.vy */
extern long long D_00096A60;    /* gCameraDistance */
extern long long D_00096A64;    /* gCameraMaxDistance */
extern long long D_0009688C;    /* cameraCarId */
extern int *ReplayParameterPtr;         /* ReplayParameterPtr */
extern long long CameraCnt;    /* CameraCnt */
extern VECTOR camera_position;
extern int D_0008A8F4[];       /* MainPlayer.cameraPos */
extern long long D_00096834;    /* playerType */
extern char D_0009B16C[];       /* car_data */
extern int D_00097630;          /* cammapht2 */
extern long long D_000967A0;    /* savemapinfo */
extern int D_0009767C;          /* mapstuff */

extern int func_000776E4();     /* rand */
extern int SelectCameraCar();
extern int MapHeight();
extern int func_0005BDF8();     /* valid_region */
extern int CameraCollisionCheck();
extern int NoMoreCamerasErrorMessage();
extern void RecordCamera();

#define cameraview cameraview.v
#define gCameraAngle (*(int *)&D_00096A6C)
#define gCameraDistance (*(int *)&D_00096A60)
#define gCameraMaxDistance (*(int *)&D_00096A64)
#define cameraCarId (*(int *)&D_0009688C)
#define CameraCnt (*(int *)&CameraCnt)
#define savemapinfo (*(unsigned int **)&D_000967A0)

void DoAutoDirect(void)
{
    VECTOR pos;
    int height;
    int id;

    if ((D_0009763C != 0 || D_00097648 != 0) && *(int *)&D_00096878 == 0) {
        tracking_car = 1;

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
                if ((id = SelectCameraCar(0)) != 0) {
                    cameraCarId = id;
                    cameraview = 2;
                } else {
                    cameraview = 1;
                }
                break;
            case 5:
                cameraCarId = SelectCameraCar(cameraCarId);
                cameraview = 2;
                break;
            case 6:
            case 7:
                cameraview = 5;
                break;
            }

            if (CameraCnt + D_00097620 > *ReplayParameterPtr) {
                cameraview = 0;
                cameraCarId = 0;
            } else {
                pos.vx = camera_position.vx = D_0009765C->x << 10;
                pos.vz = camera_position.vz = D_0009765C->y << 10;
            }

            if (*(int *)&D_00096834 != 2)
            {
                height = MapHeight(&camera_position);
                camera_position.vy = -70 - (func_000776E4() & 0x3ff) - height;
            }
            else
                D_0008A8F4[0] = -200;

            height = MapHeight(D_0009B16C + cameraCarId * 396);
            if (-height < camera_position.vy)
                camera_position.vy = -height - 100;

            D_00097630 = MapHeight(&camera_position);
            D_00097630 = -D_00097630;
            if (D_00097630 < camera_position.vy)
                camera_position.vy = D_00097630 - 100;

            MapHeight(&camera_position);

            if (savemapinfo != 0)
                D_0009767C = *savemapinfo;

            if (func_0005BDF8(D_0009765C->x << 10, D_0009765C->y << 10) == 0 ||
                (cameraview != 2 && cameraview != 0 &&
                 savemapinfo != 0 && (*savemapinfo & 0x3c000000) == 0x3c000000) ||
                CameraCollisionCheck() != 0) {
                cameraview = func_000776E4() & 2;
                if (cameraview == 0) {
                    gCameraAngle = func_000776E4() & 0xfff;
                    D_0008A1E0[0] = -((func_000776E4() & 0xff) + 100);
                    gCameraDistance = (func_000776E4() & 0x7e7) + 1000;
                    gCameraMaxDistance = gCameraDistance;
                }
                cameraCarId = 0;
            }

            if (NoMoreCamerasErrorMessage() == 0) {
                RecordCamera(CameraCnt);
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
