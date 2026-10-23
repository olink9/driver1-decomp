/* DrawCarWheels - DrawCarWheels (Driver 2) */
#include "gte.h"

typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct { short m[3][3]; int t[3]; } MATRIX;

typedef struct {
    unsigned char susCompression;
    unsigned char locked;
    char pad[2];
} WHEEL;

typedef struct {
    char pad0[0x78];
    WHEEL wheel[4];         /* 0x78 */
    char pad88[0xF4 - 0x88];
    int direction;          /* 0xF4 */
    char padF8[0x100 - 0xF8];
    int wheelspin;          /* 0x100 */
    int wheel_speed;        /* 0x104 */
    char pad108[0x18C - 0x108];
} CAR_DATA;

typedef struct {
    char pad0[0x40];
    SVECTOR wheelDisp[4];   /* 0x40 */
    char pad60[100 - 0x60];
} CAR_COSMETICS;

typedef struct {
    char pad[0x10];
    SVECTOR *vertices;      /* 0x10 */
} MODEL;

extern CAR_DATA car_data[];
extern CAR_COSMETICS car_cosmetics[];
extern long long pauseflag;
extern int D_000AB654[];
extern int D_000AB6C4[];
extern long long D_000970AC;
extern long long D_000970C0;
extern long long D_000970C4;
extern long long D_00096978;
extern short D_0008EAAA[];
extern int D_000B700C[];
extern void MulMatrix0();
extern void SetRotMatrix();
extern void DrawWheelObject();

#define pauseflag (*(int *)&pauseflag)
#define gCleanWheelModelPtr (*(MODEL **)&D_000970AC)
#define gFastWheelModelPtr (*(MODEL **)&D_000970C0)
#define gDamWheelModelPtr (*(MODEL **)&D_000970C4)

#define gte_rtv0() __asm__ volatile ( \
    "nop;" "nop;" "mvmva 1, 0, 0, 3, 0")

#define gte_stsv(r0) __asm__ volatile ( \
    "mfc2  $12, $9;" "mfc2  $13, $10;" "mfc2  $14, $11;" \
    "sh    $12, 0( %0 );" "sh    $13, 2( %0 );" "sh    $14, 4( %0 )" : : "r"(r0) : "$12", "$13", "$14", "memory")

void DrawCarWheels(int car, MATRIX *RearMatrix, VECTOR *pos, int unused, int cosIdx)
{
    MATRIX FrontMatrix;
    MATRIX SteerMatrix;
    VECTOR WheelPos;
    SVECTOR sWheelPos;
    CAR_DATA *cp;
    CAR_COSMETICS *car_cos;
    MODEL *WheelModelFront;
    MODEL *WheelModelBack;
    MODEL *model;
    SVECTOR *VertPtr;
    SVECTOR *v;
    SVECTOR *p;
    int FrontWheelIncrement, BackWheelIncrement;
    int sn, cs;
    int a, b; short c, d;
    int loop;

    cp = &car_data[car];
    FrontWheelIncrement = cp->wheel_speed / 256;
    BackWheelIncrement = cp->wheel_speed / 256;

    if (car == 0)
        car_cos = &car_cosmetics[0];
    else
        car_cos = &car_cosmetics[cosIdx];

    if (cp->wheelspin != 0)
        BackWheelIncrement = -700;
    if (cp->wheel[0].locked != 0)
        FrontWheelIncrement = 0;
    if (cp->wheel[3].locked != 0)
        BackWheelIncrement = 0;

    if (pauseflag == 0)
    {
        D_000AB654[car] = (D_000AB654[car] + FrontWheelIncrement) & 0xFFF;
        D_000AB6C4[car] = (D_000AB6C4[car] + BackWheelIncrement) & 0xFFF;
    }

    if (FrontWheelIncrement + 500U > 1000)
        WheelModelFront = gCleanWheelModelPtr;
    else
        WheelModelFront = gFastWheelModelPtr;

    if (BackWheelIncrement + 500U > 1000)
        WheelModelBack = gCleanWheelModelPtr;
    else
        WheelModelBack = gFastWheelModelPtr;

    v = WheelModelFront->vertices;
    a = D_0008EAAA[(D_000AB654[car] & 0xFFF) * 2] * 101 >> 12;
    b = D_0008EAAA[(D_000AB654[car] & 0xFFF) * 2 - 1] * 101 >> 12;
    c = -b; d = -a;
    v[15].vy = v[8].vy = a;
    v[15].vz = v[8].vz = b;
    v[14].vy = v[9].vy = c;
    v[14].vz = v[9].vz = a;
    v[13].vy = v[10].vy = d;
    v[13].vz = v[10].vz = c;
    v[12].vy = v[11].vy = b;
    v[12].vz = v[11].vz = d;
    v = WheelModelBack->vertices;
    a = D_0008EAAA[(D_000AB6C4[car] & 0xFFF) * 2] * 101 >> 12;
    b = D_0008EAAA[(D_000AB6C4[car] & 0xFFF) * 2 - 1] * 101 >> 12;
    c = -b; d = -a;
    v[15].vy = v[8].vy = a;
    v[15].vz = v[8].vz = b;
    v[14].vy = v[9].vy = c;
    v[14].vz = v[9].vz = a;
    v[13].vy = v[10].vy = d;
    v[13].vz = v[10].vz = c;
    v[12].vy = v[11].vy = b;
    v[12].vz = v[11].vz = d;
    SteerMatrix.m[2][2] = D_0008EAAA[(cp->direction & 0xFFF) * 2];
    SteerMatrix.m[0][0] = SteerMatrix.m[2][2];
    SteerMatrix.m[0][2] = D_0008EAAA[(cp->direction & 0xFFF) * 2 - 1];
    SteerMatrix.m[1][1] = 0x1000;
    SteerMatrix.m[2][1] = 0;
    SteerMatrix.m[1][2] = 0;
    SteerMatrix.m[1][0] = 0;
    SteerMatrix.m[0][1] = 0;
    SteerMatrix.m[2][0] = -SteerMatrix.m[0][2];
    MulMatrix0(RearMatrix, &SteerMatrix, &FrontMatrix);

    for (loop = 0; loop < 4; loop++)
    {
        if (loop & 1)
            model = WheelModelBack;
        else
            model = WheelModelFront;
        VertPtr = model->vertices;

        if (car == 0 && D_000B700C[3 - loop] == 0)
            model = gDamWheelModelPtr;

        if (loop & 2)
            sWheelPos.vx = -car_cos->wheelDisp[loop].vx - 25;
        else
            sWheelPos.vx = -car_cos->wheelDisp[loop].vx + 25;
        sWheelPos.vy = -car_cos->wheelDisp[loop].vy - (cp->wheel[loop].susCompression + 50);
        sWheelPos.vz = -car_cos->wheelDisp[loop].vz;

        gte_SetRotMatrix(RearMatrix);
        gte_ldv0(&sWheelPos);
        gte_rtv0();
        gte_stsv(&sWheelPos);

        WheelPos.vx = sWheelPos.vx + pos->vx;
        WheelPos.vy = sWheelPos.vy + pos->vy;
        WheelPos.vz = sWheelPos.vz + pos->vz;
        gte_SetTransVec0(&WheelPos);

        if ((loop & 1) == 0)
            SetRotMatrix(&FrontMatrix);

        DrawWheelObject(model, VertPtr, *(int *)&D_00096978);
    }
}
