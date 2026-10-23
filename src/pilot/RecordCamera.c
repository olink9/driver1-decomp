/* Driver 2: RecordCamera (DIRECTOR.C) */
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

extern PLAYBACKCAMERA *LastChange;      /* LastChange */
extern PLAYBACKCAMERA *D_00096DB0;      /* ThisChange */
extern PLAYBACKCAMERA *D_00097608;      /* PlaybackCamera */
extern unsigned char tracking_car;        /* tracking_car */
extern long long cameraview;            /* cameraview */
extern long long D_0009688C;
extern long long D_00096A60;
extern long long D_00096A64;
extern long long D_00096A6C;
extern int D_0008A1E0[];
extern int camera_position[];
typedef struct { short v; short pad[3]; } S8;   /* camera_angle.vx/vy/vz, in-struct memory */
extern S8 D_00096C48;
extern S8 D_00096C4A;
extern S8 D_00096C4C;

extern PLAYBACKCAMERA *FindFreeCamera(void);
extern void FindNextChange(int CameraCnt);

#define CAMVIEW (*(unsigned char *)&cameraview)

void RecordCamera(int CameraCnt)
{
    PLAYBACKCAMERA *TempChange;

    if (LastChange == 0 ||
        LastChange->cameraview != ((CAMVIEW & 7) | (tracking_car << 3)) ||
        *(int *)&D_0009688C != LastChange->apad ||
        CAMVIEW == 1 ||
        (CAMVIEW & 7) == 5 ||
        LastChange->s1a != *(int *)&D_00096A60 ||
        LastChange->s1c != *(int *)&D_00096A64 ||
        LastChange->s1e != *(int *)&D_00096A6C ||
        LastChange->scr_z != D_0008A1E0[0])
    {
        TempChange = FindFreeCamera();
        if (TempChange) {
            if (CameraCnt == 0) {
                D_00096DB0 = D_00097608;
            } else if (D_00096DB0 == 0 || CameraCnt != D_00096DB0->FrameCnt) {
                TempChange->next = LastChange->next;
                TempChange->prev = LastChange->idx;
                LastChange->next = TempChange->idx;
                D_00097608[TempChange->next].prev = TempChange->idx;
                D_00096DB0 = TempChange;
            }

            D_00096DB0->cameraview = (CAMVIEW & 7) | (tracking_car << 3);
            D_00096DB0->px = camera_position[0];
            D_00096DB0->py = camera_position[1];
            D_00096DB0->pz = camera_position[2];
            D_00096DB0->ax = D_00096C48.v;
            D_00096DB0->ay = D_00096C4A.v;
            D_00096DB0->az = D_00096C4C.v;
            D_00096DB0->apad = *(int *)&D_0009688C;
            D_00096DB0->FrameCnt = CameraCnt;
            D_00096DB0->s1a = *(int *)&D_00096A60;
            D_00096DB0->s1c = *(int *)&D_00096A64;
            D_00096DB0->s1e = *(int *)&D_00096A6C;
            D_00096DB0->scr_z = D_0008A1E0[0];
            LastChange = D_00096DB0;
        }
    }
    FindNextChange(CameraCnt);
}
