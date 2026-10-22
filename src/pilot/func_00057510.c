#include "gte.h"

#define b048_gte_stsz(r0) __asm__ volatile ( \
    "swc2  $19, 0( %0 )" : : "r"(r0) : "memory")

typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long current;
#define current (*(DB **)&current)
typedef struct { short vx, vy, vz, pad; } SVEC;
typedef struct { int vx, vy, vz, pad; } VEC;
typedef struct { short m[3][3]; int t[3]; } MAT;
typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char r1, g1, b1, p1;
    short x1, y1;
    unsigned char r2, g2, b2, p2;
    short x2, y2;
} PG3;

extern VEC camera_position;
extern MAT D_000B8FE8;
extern int func_000398B0();
extern int func_0002C5F0();
extern int MapHeight();
extern void func_0002C564();
extern int RotTransPers();

void func_00057510(VEC *pos)
{
    SVEC v[3];
    VEC t;
    int z;
    int flag;
    int oz;
    PG3 *poly;
    short *xy;
    short *first;
    unsigned char i;
    SVEC *vp;

    if (func_000398B0(pos) == 0)
        return;
    if (func_0002C5F0(pos, 800) == -1)
        return;
    v[0].vx = 0;
    v[0].vy = 0;
    v[0].vz = 0;
    v[1].vx = -150;
    v[1].vy = -300;
    v[1].vz = 0;
    v[2].vx = 150;
    v[2].vy = -300;
    v[2].vz = 0;
    t.vy = -MapHeight(pos) - camera_position.vy;
    t.vx = pos->vx - camera_position.vx;
    t.vz = pos->vz - camera_position.vz;
    func_0002C564(&t);
    vp = v;
    poly = (PG3 *)current->primptr;
    xy = &poly->x0;
    ((unsigned char *)poly)[3] = 6;
    poly->code = 0x30;
    gte_SetRotMatrix(&D_000B8FE8);
    poly->r0 = 0;
    poly->g0 = 0;
    poly->b0 = 0;
    gte_SetTransVec0(&t);
    poly->r1 = 255;
    poly->g1 = 0;
    poly->b1 = 0;
    poly->r2 = 255;
    poly->g2 = 0;
    poly->b2 = 0;
    for (i = 0, first = xy; i < 3; i++) {
        RotTransPers(vp, xy, &z, &flag);
        if (xy == first)
            xy = &poly->x1;
        else
            xy = &poly->x2;
        vp++;
    }
    b048_gte_stsz(&z);
    oz = z >> 3;
    poly->tag = (poly->tag & 0xFF000000) | (current->ot[oz] & 0xFFFFFF);
    current->ot[oz] = (current->ot[oz] & 0xFF000000) | ((unsigned int)poly & 0xFFFFFF);
    current->primptr += 0x1C;
}
