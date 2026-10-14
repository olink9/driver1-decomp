#include "gte.h"
typedef struct { int vx, vy, vz, pad; } VEC;
typedef struct { short vx, vy, vz, pad; } SVEC;
typedef struct { char pad[0x30]; SVEC sv; char pad2[100 - 0x38]; } E;
typedef struct { char pad[2]; unsigned char idx; char pad2[0xC8 - 3]; VEC pos; char pad3[0x18C - 0xC8 - 16]; } CAR;
extern CAR D_0009B0A4[];
extern E D_0009AF04[];
extern char D_000AF6A0[];
extern long long D_000970D8, D_0009687C;
extern int D_00096ABC;
extern int func_000776E4();
extern void func_000466D4(), func_00047A90();
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
    sv = e->sv;
    sv.vx = (func_000776E4() & 0x7F) - 0x40;
    pos = D_0009B0A4[a].pos;
    pos.vy = -pos.vy;
    if (b)
        pos.vy -= 50;
    gte_SetRotMatrix(D_000AF6A0 + a * 32);
    sv2 = sv;
    xform(&sv2, &pos);
    func_000466D4(&v);
    if (b)
        func_00047A90(&pos, 100, 500, 1, c, &v, 0);
    else
        func_00047A90(&pos, 100, 400, 2, c, &v, 0);
}
