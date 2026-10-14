#include "gte.h"
typedef struct { int vx, vy, vz, pad; } VEC;
typedef struct { short vx, vy, vz, pad; } SVEC;
typedef struct { char pad[0x38]; SVEC sv; char pad2[100 - 0x40]; } E;
typedef struct { char pad[2]; unsigned char idx; char pad2[0xC8 - 3]; VEC pos; char pad3[0x18C - 0xC8 - 16]; } CAR;
extern CAR D_0009B0A4[];
extern E D_0009AF04[];
extern char D_000AF6A0[];
extern long long D_000970D8, D_0009687C;
extern int D_00096ABC;
extern int func_000776E4();
extern void func_00079D20(), func_00047A90();
void func_00046C58(int a)
{
    VEC pos;
    SVEC sv;
    VEC zero;
    SVEC sv2;
    E *e;
    if ((*(int *)&D_000970D8 & 1) != (a & 1))
        return;
    if (a == 0)
        e = D_0009AF04;
    else
        e = &D_0009AF04[D_0009B0A4[a].idx];
    if (D_00096ABC == 0)
        return;
    if (*(int *)&D_0009687C != 0)
        return;
    sv = e->sv;
    sv.vx += func_000776E4() & 0x3F;
    sv.vz += func_000776E4() & 0x3F;
    pos = D_0009B0A4[a].pos;
    pos.vy = -pos.vy;
    func_00079D20(D_000AF6A0 + a * 32);
    sv2 = sv;
    gte_SetTransVec0(&pos);
    gte_ldv0(&sv2);
    gte_rtv0tr();
    gte_stlvnl(&pos);
    zero.vx = 0;
    zero.vy = 0;
    zero.vz = 0;
    func_00047A90(&pos, 50, 100, 4, 0, &zero, 0);
}
