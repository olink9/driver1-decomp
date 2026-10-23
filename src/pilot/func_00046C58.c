#include "gte.h"
typedef struct { int vx, vy, vz, pad; } VEC;
typedef struct { short vx, vy, vz, pad; } SVEC;
typedef struct { char pad[0x38]; SVEC sv; char pad2[100 - 0x40]; } E;
typedef struct { char pad[2]; unsigned char idx; char pad2[0xC8 - 3]; VEC pos; char pad3[0x18C - 0xC8 - 16]; } CAR;
extern CAR car_data[];
extern E car_cosmetics[];
extern char D_000AF6A0[];
extern long long CameraCnt, pauseflag;
extern int D_00096ABC;
extern int func_000776E4();
extern void SetRotMatrix(), Setup_Smoke();
void func_00046C58(int a)
{
    VEC pos;
    SVEC sv;
    VEC zero;
    SVEC sv2;
    E *e;
    if ((*(int *)&CameraCnt & 1) != (a & 1))
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
    sv.vx += func_000776E4() & 0x3F;
    sv.vz += func_000776E4() & 0x3F;
    pos = car_data[a].pos;
    pos.vy = -pos.vy;
    SetRotMatrix(D_000AF6A0 + a * 32);
    sv2 = sv;
    gte_SetTransVec0(&pos);
    gte_ldv0(&sv2);
    gte_rtv0tr();
    gte_stlvnl(&pos);
    zero.vx = 0;
    zero.vy = 0;
    zero.vz = 0;
    Setup_Smoke(&pos, 50, 100, 4, 0, &zero, 0);
}
