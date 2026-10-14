#include "gte.h"
typedef struct { int vx, vy, vz, pad; } VEC;
typedef struct { short vx, vy, vz, pad; } SVEC;
typedef struct { char pad[0x28]; SVEC sv; char pad2[100 - 0x30]; } E;
typedef struct { char pad[2]; unsigned char idx; char pad2[0xC8 - 3]; VEC pos; char pad3[0x120 - 0xC8 - 16]; int f120; char pad4[0x18C - 0x124]; } CAR;
extern CAR D_0009B0A4[];
extern E D_0009AF04[];
extern char D_000AF6A0[];
extern long long D_000970D8, D_0009687C;
extern int D_00096ABC;
extern void func_0004EAC0(), func_00047A90();
void func_000467DC(int a, int b, unsigned char c)
{
    VEC pos;
    SVEC sv;
    VEC v;
    SVEC sv2;
    E *e;
    if ((*(int *)&D_000970D8 & 3) != (a & 3))
        return;
    if (a == 0)
        e = D_0009AF04;
    else
        e = &D_0009AF04[D_0009B0A4[a].idx];
    if (D_00096ABC == 0)
        return;
    if (*(int *)&D_0009687C != 0)
        return;
    pos.vx = D_0009B0A4[a].pos.vx;
    pos.vy = -D_0009B0A4[a].pos.vy;
    pos.vz = D_0009B0A4[a].pos.vz;
    gte_SetRotMatrix(D_000AF6A0 + a * 32);
    sv = e->sv;
    sv.vz -= D_0009B0A4[a].f120 >> 6;
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
        func_00047A90(&pos, 20, 200, 1, b, &v, 1);
    else
        func_00047A90(&pos, 20, 100, 2, b, &v, 1);
}
