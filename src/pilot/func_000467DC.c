#include "gte.h"
typedef struct { int vx, vy, vz, pad; } VEC;
typedef struct { short vx, vy, vz, pad; } SVEC;
typedef struct { char pad[0x28]; SVEC sv; char pad2[100 - 0x30]; } E;
typedef struct { char pad[2]; unsigned char idx; char pad2[0xC8 - 3]; VEC pos; char pad3[0x120 - 0xC8 - 16]; int f120; char pad4[0x18C - 0x124]; } CAR;
extern CAR car_data[];
extern E car_cosmetics[];
extern char D_000AF6A0[];
extern long long CameraCnt, pauseflag;
extern int D_00096ABC;
extern void func_0004EAC0(), Setup_Smoke();
void func_000467DC(int a, int b, unsigned char c)
{
    VEC pos;
    SVEC sv;
    VEC v;
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
    pos.vx = car_data[a].pos.vx;
    pos.vy = -car_data[a].pos.vy;
    pos.vz = car_data[a].pos.vz;
    gte_SetRotMatrix(D_000AF6A0 + a * 32);
    sv = e->sv;
    sv.vz -= car_data[a].f120 >> 6;
    sv2 = e->sv;
    gte_SetTransVec0(&pos);
    gte_ldv0(&sv2);
    gte_rtv0tr();
    gte_stlvnl(&pos);
    v.vx = 0;
    v.vy = -1;
    v.vz = 2;
    func_0004EAC0(&v);
    if (c)
        Setup_Smoke(&pos, 20, 200, 1, b, &v, 1);
    else
        Setup_Smoke(&pos, 20, 100, 2, b, &v, 1);
}
