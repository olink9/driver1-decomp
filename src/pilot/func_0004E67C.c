/* sound volume by distance to camera for channel n (GTE sqr0 distance) */
#include "gte.h"
typedef struct { int vx, vy, vz, pad; } VEC;
typedef struct { char pad0[0x48]; int f48; char pad4c[0x10]; VEC *pos; char pad60[132 - 0x60]; } ENT;
extern ENT D_000BD498[];
extern VEC D_000BDCD8;
extern long long D_00096834;
extern int SquareRoot0(int);

int func_0004E67C(int n)
{
    VEC *p;
    int vol = D_000BD498[n].f48 + 10000;
    int dist, att;
    VEC d;

    p = D_000BD498[n].pos;
    if (p) {
        d.vx = p->vx - D_000BDCD8.vx;
        d.vy = p->vy + D_000BDCD8.vy;
        d.vz = p->vz - D_000BDCD8.vz;
        gte_ldlvl(&d);
        gte_sqr0();
        gte_stlvnl(&d);
        dist = SquareRoot0(d.vx + d.vy + d.vz);
        if (dist < 0x400)
            dist = 0x400;
        att = dist - 9000;
        if (*(int *)&D_00096834 == 2) {
            dist = 0x578000 / (dist + 1300);
        } else {
            if (att < 0)
                att = 0;
            else if (att > 0x1000)
                att = 0x1000;
            dist = 0x914000 / (dist + 1300);
            if (att)
                dist = dist * (0x1000 - att) >> 12;
        }
        vol = dist * vol / 4096;
    }
    return vol - 10000;
}
