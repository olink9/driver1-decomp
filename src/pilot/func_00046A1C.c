#include "gte.h"
typedef struct { int vx, vy, vz, pad; } VEC;
typedef struct { short vx, vy, vz, pad; } SVEC;
typedef struct { char pad[0x30]; SVEC sv; char pad2[100 - 0x38]; } E;
typedef struct { char pad[2]; unsigned char idx; char pad2[0xC8 - 3]; VEC pos; char pad3[0x18C - 0xC8 - 16]; } CAR;
extern CAR car_data[];
extern E car_cosmetics[];
extern char D_000AF6A0[];
extern long long CameraCnt, pauseflag;
extern int D_00096ABC;
extern int func_000776E4();
extern void GetSmokeDrift(), Setup_Smoke();
static inline void xform(SVEC *in, VEC *out)
{
    gte_SetTransVec0(out);
    gte_ldv0(in);
    gte_rtv0tr();
    gte_stlvnl(out);
}
void func_00046A1C(int a, int b, int c)
{
    VEC pos;
    VEC v;
    SVEC sv;
    SVEC sv2;
    E *e;
    if ((*(int *)&CameraCnt & 3) != (a & 3))
        return;
    if (a == 0)
        e = car_cosmetics;
    else
        e = &car_cosmetics[car_data[a].idx];
    if (D_00096ABC == 0)
        return;
    if (*(int *)&pauseflag != 0)
        return;
    sv = e->sv;
    sv.vx = (func_000776E4() & 0x7F) - 0x40;
    pos = car_data[a].pos;
    pos.vy = -pos.vy;
    if (b)
        pos.vy -= 50;
    gte_SetRotMatrix(D_000AF6A0 + a * 32);
    sv2 = sv;
    xform(&sv2, &pos);
    GetSmokeDrift(&v);
    if (b)
        Setup_Smoke(&pos, 100, 500, 1, c, &v, 0);
    else
        Setup_Smoke(&pos, 100, 400, 2, c, &v, 0);
}
