/* Driver 2: RoundShadow (DEBRIS.C) */
#include "gte.h"

typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct { unsigned char r, g, b, cd; } CVECTOR;
typedef struct { char *primptr; unsigned int *ot; } DB;
typedef struct { int m[5]; } MATRIX;

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short x1, y1;
    unsigned char u1, v1;
    unsigned short tpage;
    short x2, y2;
    unsigned char u2, v2;
    unsigned short pad1;
    short x3, y3;
    unsigned char u3, v3;
    unsigned short pad2;
} POLY_FT4;

typedef struct {
    char pad[8];
    unsigned short tpageid;
    unsigned short clutid;
} TEXINF;

extern long long D_00097530;
#define current (*(DB **)&D_00097530)
extern MATRIX D_000BBE84;
extern VECTOR D_0008909C;
extern unsigned char D_000B9170[];
extern TEXINF D_000B9110;

#define gte_stsxy3(r0, r1, r2) __asm__ volatile ( \
    "swc2  $12, 0( %0 );" "swc2  $13, 0( %1 );" "swc2  $14, 0( %2 )" : : "r"(r0), "r"(r1), "r"(r2) : "memory")
#define gte_stsxy(r0) __asm__ volatile ("swc2  $14, 0( %0 )" : : "r"(r0) : "memory")
#define gte_ldv3(r0, r1, r2) __asm__ volatile ( \
    "lwc2  $0, 0( %0 );" "lwc2  $1, 4( %0 );" "lwc2  $2, 0( %1 );" \
    "lwc2  $3, 4( %1 );" "lwc2  $4, 0( %2 );" "lwc2  $5, 4( %2 )" : : "r"(r0), "r"(r1), "r"(r2))
#define gte_ldv3c(r0) __asm__ volatile ( \
    "lwc2  $0, 0( %0 );" "lwc2  $1, 4( %0 );" "lwc2  $2, 8( %0 );" \
    "lwc2  $3, 12( %0 );" "lwc2  $4, 16( %0 );" "lwc2  $5, 20( %0 )" : : "r"(r0))
#define gte_rtpt() __asm__ volatile ("nop;" "nop;" "rtpt")
#define gte_rtps() __asm__ volatile ("nop;" "nop;" "rtps")

typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
#define setaddr(_p, _addr) (((P_TAG *)(_p))->addr = (unsigned long)(_addr))
#define getaddr(_p) (unsigned long)(((P_TAG *)(_p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)

void func_000451A8(VECTOR *v1, CVECTOR *col, short size)
{
    POLY_FT4 *poly;
    SVECTOR vert[4];
    int z;

    gte_SetRotMatrix(&D_000BBE84);
    gte_SetTransVec0(&D_0008909C);

    vert[0].vx = v1->vx - size;
    vert[0].vy = v1->vy;
    vert[0].vz = v1->vz + size;

    vert[1].vx = v1->vx + size;
    vert[1].vy = v1->vy;
    vert[1].vz = v1->vz + size;

    vert[2].vx = v1->vx - size;
    vert[2].vy = v1->vy;
    vert[2].vz = v1->vz - size;

    vert[3].vx = v1->vx + size;
    vert[3].vy = v1->vy;
    vert[3].vz = v1->vz - size;

    gte_ldv3c(&vert[0]);
    gte_rtpt();

    poly = (POLY_FT4 *)current->primptr;

    poly->u0 = D_000B9170[0];
    poly->v0 = D_000B9170[1];
    poly->u1 = D_000B9170[2];
    poly->v1 = D_000B9170[3];
    poly->u2 = D_000B9170[4];
    poly->v2 = D_000B9170[5];
    poly->u3 = D_000B9170[6];
    poly->v3 = D_000B9170[7];

    poly->tpage = D_000B9110.tpageid | 0x20;
    poly->clut = D_000B9110.clutid;

    ((P_TAG *)poly)->len = 9;
    poly->code = 0x2E;

    poly->r0 = col->r;
    poly->g0 = col->g;
    poly->b0 = col->b;

    gte_stsz(&z);

    if ((unsigned)(z - 150) < 9851) {
        gte_stsxy3(&poly->x0, &poly->x1, &poly->x2);
        gte_ldv0(&vert[3]);
        gte_rtps();
        addPrim(current->ot + (z >> 3), poly);
        current->primptr += sizeof(POLY_FT4);
        gte_stsxy(&poly->x3);
    }
}
