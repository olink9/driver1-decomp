/* Driver 2: setCamera (DIRECTOR.C) */
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

extern unsigned char tracking_car;        /* tracking_car */
extern long long cameraview;
extern long long D_0009688C;
extern long long D_00096A60;
extern long long D_00096A64;
extern long long D_00096A6C;
extern int D_0008A1E0[];
extern int camera_position[];
typedef struct { short v; short pad[3]; } S8;   /* camera_angle.vx/vy/vz */
extern S8 D_00096C48;
extern S8 D_00096C4A;
extern S8 D_00096C4C;

#define CAMVIEW (*(unsigned char *)&cameraview)

void setCamera(PLAYBACKCAMERA *Change)
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
