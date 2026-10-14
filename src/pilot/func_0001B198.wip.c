typedef struct { short x, y; } SV;
typedef struct { char pad[0xC8]; int px, py, pz; char pad1[0xF8 - 0xD4]; int ang; char pad2[0x104 - 0xFC]; int sgn; char pad3[0x18C - 0x108]; } CAR;
typedef struct { char pad0, on, pad2, pad3, pad4, flag; char pad[0x50 - 6]; } ENT;
extern CAR D_0009B230[];
extern CAR D_0009B0A4;
extern ENT D_00099030[];
extern int D_0008EAA8[];
extern int func_000397F4();
CAR *func_0001B198(int a, int b, int c)
{
    CAR *car;
    ENT *e;
    volatile int v[8];
    SV sv;
    int pad2;
    int ang, ddx, ddz, sx, sy;
    CAR *c0;
    ENT *end = D_00099030;
    e = end - 22;
    car = D_0009B230;
    for (; e < end; e++, car++) {
        if (e->on && !e->flag && !func_000397F4(&car->px, a, b, c)) {
            c0 = &D_0009B0A4;
            ang = c0->ang;
            if (c0->sgn < 0)
                ang = -ang;
            *(int *)&sv = D_0008EAA8[ang & 0xFFF];
            sy = sv.y;
            v[6] = sy;
            ddx = car->px - c0->px;
            sx = sv.x;
            v[0] = ddx;
            ddz = car->pz - c0->pz;
            v[4] = sx;
            v[2] = ddz;
            if (ddx * sx + ddz * sy > 0) {
                ang = car->ang;
                if (car->sgn < 0)
                    ang = -ang;
                *(int *)&sv = D_0008EAA8[ang & 0xFFF];
                sy = sv.y;
                v[6] = sy;
                ddx = c0->px - car->px;
                sx = sv.x;
                v[0] = ddx;
                ddz = c0->pz - car->pz;
                v[4] = sx;
                v[2] = ddz;
                if (ddx * sx + ddz * sy > 0)
                    return car;
            }
        }
    }
    return 0;
}
