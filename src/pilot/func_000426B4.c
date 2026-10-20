#include "common.h"
typedef struct { int x, y, z; } V3s;
typedef struct { int x, y, z; short p; unsigned short type; } COP;
typedef struct { short a; unsigned short flags2; } MOD;
extern long long D_0009B1A8;
extern long long D_0009697C;
#define G_COPP (*(COP ***)&D_0009697C)
extern V3s D_0008A8F0;
extern int D_00097430;
extern MOD *D_000A0E20[];
extern COP *D_000A5DCC[];
extern int func_000776E4();
extern void func_000217C8(), func_00041AC4();

/* Driver 2 name: AddGroundDebris */
static inline int rnd(int s) { return (s & 0x3ff) - 512; }
void func_000426B4(void)
{
    int t, count, seed, xb, zb, type, number;
    COP *cop;
    MOD *model;
    V3s Position;
    short out[4];

    if (*(unsigned *)&D_0009B1A8 + 99999U <= 0x30d3e)
        return;
    count = 0;
    G_COPP = D_000A5DCC;
    do {
        cop = *G_COPP;
        if (cop == 0) return;
        model = D_000A0E20[cop->type];
        xb = cop->x - D_0008A8F0.x;
        zb = cop->z - D_0008A8F0.z;
        if (xb < 0) xb = -xb;
        if (zb < 0) zb = -zb;
        if (xb > 9000 || zb > 9000) return;
        if (xb < 7000 && zb < 7000) return;
        if (D_00097430 >= 40) return;
        seed = func_000776E4();
        Position.x = cop->x + rnd(seed);
        t = rnd(func_000776E4());
        Position.z = cop->z + t;
        func_000217C8(Position.x, Position.z, out);
        Position.y = -out[1];
        if (model->flags2 & 0x2000) { number = seed & 7; type = 1; }
        else { number = seed & 3; type = 2; }
        func_00041AC4(&Position, number + 1, type);
        G_COPP++;
        count++;
    } while (count < 16);
}
