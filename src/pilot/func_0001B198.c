typedef struct { short x, y; } SV;
typedef struct { int vx, vy, vz, pad; } VEC;
typedef struct { int vx, vy, vz; } V3;
typedef struct { char pad[0xC8]; int px, py, pz; char pad1[0xF8 - 0xD4]; int ang; char pad2[0x104 - 0xFC]; int sgn; char pad3[0x18C - 0x108]; } CAR;
typedef struct { char pad0, on, pad2, pad3, pad4, flag; char pad[0x50 - 6]; } ENT;
extern CAR D_0009B230[];
extern CAR car_data;
extern ENT D_00099030[];
extern int rcossin_tbl[];
extern int func_000397F4();
CAR *func_0001B198(int a, int b, int c)
{
    CAR *car;
    ENT *e;
    VEC delta;
    VEC dir;
    SV sv;
    int ang, ddx, ddz, sx, sy;
    CAR *c0;
    V3 *p;
    ENT *end = D_00099030;
    e = end - 22;
    car = D_0009B230;
    for (; e < end; e++, car++) {
        int *pp = &car->px;
        if (e->on && !e->flag && !func_000397F4(pp, a, b, c)) {
            c0 = &car_data;
            ang = c0->ang;
            if (c0->sgn < 0)
                ang = -ang;
            *(int *)&sv = rcossin_tbl[ang & 0xFFF];
            sy = sv.y;
            dir.vz = sy;
            ddx = car->px - c0->px;
            sx = sv.x;
            delta.vx = ddx;
            ddz = car->pz - c0->pz;
            dir.vx = sx;
            delta.vz = ddz;
            if (ddx * sx + ddz * sy > 0) {
                p = (V3 *)&c0->px;
                ang = car->ang;
                if (car->sgn < 0)
                    ang = -ang;
                *(int *)&sv = rcossin_tbl[ang & 0xFFF];
                sy = sv.y;
                dir.vz = sy;
                ddx = p->vx - car->px;
                sx = sv.x;
                delta.vx = ddx;
                ddz = p->vz - car->pz;
                dir.vx = sx;
                delta.vz = ddz;
                if (ddx * sx + ddz * sy > 0)
                    return car;
            }
        }
    }
    return 0;
}
