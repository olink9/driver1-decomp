#include "gte.h"
typedef struct { int vx, vy, vz, pad; } VEC;
typedef struct { short vx, vy, vz, pad; } SVEC;
typedef struct { unsigned char r, g, b, cd; } CVEC;
typedef struct { int m[8]; } MAT;
typedef struct { char pad[0xC8]; VEC pos; char pad3[0x18C - 0xC8 - 16]; } CAR;
extern CAR D_0009B0A4[];
extern MAT D_000AF6A0[];
extern VEC D_0008A8F0;
extern int D_00097424, D_00097434, D_00097438, D_00097440, D_00097468;
extern int D_000B9230[];
extern int func_0001F5B0();
extern void func_0004449C(), func_0004A398();
extern void func_00045718(VEC *, CVEC *, short, int);

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
    v.vx = D_0009B0A4[car].pos.vx - D_0008A8F0.vx;
    v.vy = -D_0009B0A4[car].pos.vy - D_0008A8F0.vy;
    v.vz = D_0009B0A4[car].pos.vz - D_0008A8F0.vz;
    sv = *pos;
    gte_SetTransVec0(&v);
    gte_ldv0(&sv);
    gte_rtv0tr();
    gte_stlvnl(&v);
    if (D_00097424 >= 10) {
        v2 = v;
        v2.vy = -func_0001F5B0(&D_0009B0A4[car].pos) - D_0008A8F0.vy;
    }
    col->cd = cd;
    func_0004449C(&v, col, size, arg4);
    func_00045718(&v, col, size * 3, ((pos->vx + pos->vz) >> 2) + ((D_00097434 + D_00097438) & 0xFFF) * 2);
    func_0004A398(&v2, col, size, D_000B9230);
}
