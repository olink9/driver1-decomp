/* Driver 2: DisplaySplashes */
#include "gte.h"
typedef struct { int vx, vy, vz, pad; } VEC;
typedef struct { unsigned char r, g, b, cd; } CVEC;
typedef struct { short m[3][3]; int t[3]; } MAT;

extern long long pauseflag;           /* pauseflag */
extern unsigned char D_00096AA8, D_00096AA9;
extern long long D_000971B4;           /* FrAng */
extern long long D_00096C4A;           /* camera_angle.vy */
extern MAT D_000B8FE8;                 /* identity */
extern VEC camera_position;                 /* camera_position */
extern short rcossin_tbl[];             /* rcossin_tbl */
extern unsigned int D_00096C44;        /* rand */
extern char D_000B90F0[];              /* drop_texture */
extern int MapHeight();
#define RSIN(a) rcossin_tbl[((a) & 0xfff) * 2]
#define RCOS(a) rcossin_tbl[((a) & 0xfff) * 2 + 1]            /* MapHeight */
extern void ShowLight();

void DisplaySplashes(void)
{
    VEC CamGnd;
    VEC Gnd1;
    VEC Gnd2;
    VEC Position;
    char pad[48];
    CVEC col = { 25, 25, 25 };
    int SplashNo;
    int d1, d2;
    unsigned int r;
    int SplashFrac;

    if (*(int *)&pauseflag != 0)
        return;

    SplashNo = (D_00096AA8 + D_00096AA9) >> 2;
    SplashFrac = *(int *)&D_000971B4 * 3;
    if (SplashNo > 30)
        SplashNo = 30;
    SplashNo = (SplashNo * SplashFrac) >> 12;

    gte_SetRotMatrix(&D_000B8FE8);

    CamGnd.vx = camera_position.vx;
    CamGnd.vz = camera_position.vz;
    CamGnd.vy = -MapHeight(&CamGnd) - camera_position.vy;

    Gnd1.vx = RSIN(*(int *)&D_000971B4 - *(short *)&D_00096C4A) + camera_position.vx;
    Gnd1.vz = RCOS(*(int *)&D_000971B4 - *(short *)&D_00096C4A) + camera_position.vz;
    Gnd1.vy = -MapHeight(&Gnd1) - camera_position.vy;
    Gnd1.vx -= CamGnd.vx;
    Gnd1.vy -= CamGnd.vy;
    Gnd1.vz -= CamGnd.vz;

    Gnd2.vx = RSIN(-*(short *)&D_00096C4A - *(int *)&D_000971B4) + camera_position.vx;
    Gnd2.vz = RCOS(-*(short *)&D_00096C4A - *(int *)&D_000971B4) + camera_position.vz;
    Gnd2.vy = -MapHeight(&Gnd2) - camera_position.vy;
    Gnd2.vx -= CamGnd.vx;
    Gnd2.vy -= CamGnd.vy;
    Gnd2.vz -= CamGnd.vz;

    while (SplashNo-- != 0) {
        r = D_00096C44 * 0x19660D + 0x3C6EF35F;
        D_00096C44 = r * 0x19660D + 0x3C6EF35F;
        d1 = r >> 4 & 0xfff;
        d2 = D_00096C44 >> 14 & 0xfff;

        Position.vx = (Gnd1.vx * d1 + Gnd2.vx * d2 + 0x800) >> 12;
        Position.vy = ((Gnd1.vy * d1 + Gnd2.vy * d2 + 0x800) >> 12) + CamGnd.vy;
        Position.vz = (Gnd1.vz * d1 + Gnd2.vz * d2 + 0x800) >> 12;

        ShowLight(&Position, &col, 12, D_000B90F0);
    }
}
