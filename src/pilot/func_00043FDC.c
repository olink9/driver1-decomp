#include "gte.h"
typedef struct { int vx, vy, vz, pad; } VEC;
typedef struct { short vx, vy, vz, pad; } SVEC;
typedef struct { unsigned char r, g, b, cd; } CVEC;
typedef struct { int m[8]; } MAT;
typedef struct { char pad[0xC8]; VEC pos; char pad3[0x18C - 0xC8 - 16]; } CAR;
extern CAR car_data[];
extern MAT D_000AF6A0[];
extern VEC camera_position;
extern int wetness, D_00097434, D_00097438, D_00097440, D_00097468;
extern int D_000B9230[];
extern int MapHeight();
extern void ShowLight(), DisplayLightReflections();
extern void ShowFlare(VEC *, CVEC *, short, int);

void func_00043FDC(SVEC *pos, int car, CVEC *col, short size, int arg4, int flag)
{
    VEC v2;
    VEC v;
    SVEC sv;
    int fade;
    int tmp;
    unsigned char cd = flag;
    int odd = flag & 1;

    if (cd == 0xFF) {
        fade = 0xFF;
        cd = 0;
    } else {
        if (odd)
            tmp = D_00097468;
        else
            tmp = D_00097440;
        if (tmp < 0)
            return;
        fade = tmp;
    }
    col->r = (col->r * fade) >> 10;
    col->b = (col->b * fade) >> 10;
    col->g = (col->g * fade) >> 10;
    gte_SetRotMatrix(&D_000AF6A0[car]);
    v.vz = 0;
    v.vy = 0;
    v.vx = 0;
    v.vx = car_data[car].pos.vx - camera_position.vx;
    v.vy = -car_data[car].pos.vy - camera_position.vy;
    v.vz = car_data[car].pos.vz - camera_position.vz;
    sv = *pos;
    gte_SetTransVec0(&v);
    gte_ldv0(&sv);
    gte_rtv0tr();
    gte_stlvnl(&v);
    if (wetness >= 10) {
        v2 = v;
        v2.vy = -MapHeight(&car_data[car].pos) - camera_position.vy;
    }
    col->cd = cd;
    ShowLight(&v, col, size, arg4);
    ShowFlare(&v, col, size * 3, ((pos->vx + pos->vz) >> 2) + ((D_00097434 + D_00097438) & 0xFFF) * 2);
    DisplayLightReflections(&v2, col, size, D_000B9230);
}
