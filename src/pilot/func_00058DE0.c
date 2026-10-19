/* Driver 2: CheckCameraChange (DIRECTOR.C / CAMERA.C) */
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

extern PLAYBACKCAMERA *D_00096DB4;      /* LastChange */
extern PLAYBACKCAMERA *D_00096DAC;      /* ThisChange? */
extern PLAYBACKCAMERA *D_00097608;      /* PlaybackCamera */
extern unsigned char D_00097658;        /* tracking_car */
extern long long D_00096897;            /* cameraview */
extern long long D_0009688C;
extern long long D_00096878;
extern long long D_0009687C;
extern long long D_00096A60;
extern long long D_00096A64;
extern long long D_00096A6C;
extern int D_0008A1E0[];
extern int D_0008A8F0[];
typedef struct { short v; short pad[3]; } S8;
extern S8 D_00096C48;
extern S8 D_00096C4A;
extern S8 D_00096C4C;
extern unsigned char D_00097660;
extern int D_00097618;
extern int D_00096DA4;
extern int D_00096DC4;

extern void func_0005B784();
extern int func_0005BCE0();
extern void func_00058B2C(int CameraCnt);

static inline void FindNextChange(int CameraCnt) {
    int nextframe = 100001;
    int found = 0;
    int count = 0;
    PLAYBACKCAMERA *rc = D_00096DAC;
    D_00096DAC = D_00097608;
    do {
        if (D_00097608[count].FrameCnt >= CameraCnt && D_00097608[count].FrameCnt < nextframe) {
            nextframe = D_00097608[count].FrameCnt;
            D_00096DAC = &D_00097608[count];
            found = 1;
        }
        count++;
    } while (count < 60);
    if (!found) {
        D_00096DAC = rc;
        rc->next = 0xFE;
    }
}

int func_00058DE0(int CameraCnt)
{

    if (CameraCnt != -1) {
        func_0005B784();
        if (*(int *)&D_00096878 == 0 && func_0005BCE0(*(int *)&D_0009688C) != 0) {
            func_00058B2C(CameraCnt);
            return 0;
        }
    }

    if (D_00096DAC->FrameCnt != CameraCnt) {
        if (D_00096DAC->next == 0xFE)
            return 0;
        if (CameraCnt < D_00096DAC->FrameCnt)
            return 0;
    }

    *(unsigned char *)&D_00096897 = D_00096DAC->cameraview & 7;
    *(int *)&D_0009688C = D_00096DAC->apad;
    D_00097658 = (D_00096DAC->cameraview & 8) >> 3;
    D_0008A8F0[0] = D_00096DAC->px;
    D_0008A8F0[1] = D_00096DAC->py;
    D_0008A8F0[2] = D_00096DAC->pz;
    D_00096C48.v = D_00096DAC->ax;
    D_00096C4A.v = D_00096DAC->ay;
    D_00096C4C.v = D_00096DAC->az;
    *(int *)&D_00096A60 = D_00096DAC->s1a;
    *(int *)&D_00096A64 = D_00096DAC->s1c;
    *(int *)&D_00096A6C = D_00096DAC->s1e;
    D_0008A1E0[0] = D_00096DAC->scr_z;

    if (func_0005BCE0(*(int *)&D_0009688C) == 0) {
        D_00096DB4 = D_00096DAC;
        FindNextChange(CameraCnt + 1);
    }

    D_00097660 = 1;
    if (D_00097618 == 1) {
        D_00096DA4 = 0;
        D_00096DC4 = 0;
        *(int *)&D_0009687C = D_00097618;
    }
    return 1;
}

