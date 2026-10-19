/* doppler: distance to camera now and next frame (pos + velocity), pitch 0x1000 - 2*delta (GTE sqr0) */
#include "gte.h"
typedef struct { int vx, vy, vz, pad; } VEC;
typedef struct { char pad0[0x52]; short f52; int f54; char pad58[4]; VEC *pos; char pad60[0x10]; VEC *vel; } OBJ;
extern VEC D_000BDCD8;
extern VEC D_000BD474;
extern int SquareRoot0(int);

void func_0004DBF8(OBJ *o)
{
    VEC d;
    int dist, dist2;

    if (o->pos) {
        d.vx = o->pos->vx - D_000BDCD8.vx;
        d.vy = o->pos->vy + D_000BDCD8.vy;
        d.vz = o->pos->vz - D_000BDCD8.vz;
        gte_ldlvl(&d);
        gte_sqr0();
        gte_stlvnl(&d);
        dist = SquareRoot0(d.vx + d.vy + d.vz);
        d.vx = (o->pos->vx - D_000BDCD8.vx) + (o->vel->vx - D_000BD474.vx);
        d.vy = (o->pos->vy + D_000BDCD8.vy) + (o->vel->vy - D_000BD474.vy);
        d.vz = (o->pos->vz - D_000BDCD8.vz) + (o->vel->vz - D_000BD474.vz);
        gte_ldlvl(&d);
        gte_sqr0();
        gte_stlvnl(&d);
        dist2 = SquareRoot0(d.vx + d.vy + d.vz);
        o->f54 = dist;
        o->f52 = 0x1000 - (dist2 - dist) * 2;
    } else {
        o->f52 = 0x1000;
    }
}
