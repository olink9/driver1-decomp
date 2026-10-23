/* Driver 2: ShowFlare (DEBRIS.C) */
#include "gte.h"

typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct { unsigned char r, g, b, cd; } CVECTOR;
typedef struct { char *primptr; unsigned int *ot; } DB;
typedef struct { short m[3][3]; int t[3]; } MATRIX;

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
    unsigned char c[8];
    unsigned short tpageid;
    unsigned short clutid;
} TEXINF;

extern long long current;
#define current (*(DB **)&current)
extern TEXINF D_000B9130;
extern int LightSortCorrect;
extern void func_0004EA98();

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


void ShowFlare(VECTOR *v1, CVECTOR *col, short size, int rotation)
{
    POLY_FT4 *poly;
    SVECTOR vert[4];
    SVECTOR direction;
    MATRIX temp_matrix;
    int z;

    gte_SetTransVec0(v1);

    direction.vy = 0;
    direction.vx = 0;
    direction.vz = rotation;

    func_0004EA98(&temp_matrix, &direction);

    temp_matrix.m[0][0] *= 3;
    temp_matrix.m[0][1] *= 3;
    temp_matrix.m[0][2] *= 3;
    temp_matrix.m[0][0] >>= 1;
    temp_matrix.m[0][1] >>= 1;
    temp_matrix.m[0][2] >>= 1;

    gte_SetRotMatrix(&temp_matrix);

    vert[0].vx = -size;
    vert[0].vy = -size;
    vert[0].vz = 0;

    vert[1].vx = size;
    vert[1].vy = -size;
    vert[1].vz = 0;

    vert[2].vx = -size;
    vert[2].vy = size;
    vert[2].vz = 0;

    vert[3].vx = size;
    vert[3].vy = size;
    vert[3].vz = 0;

    gte_ldv3c(&vert[0]);
    gte_rtpt();

    poly = (POLY_FT4 *)current->primptr;

    poly->u0 = D_000B9130.c[0];
    poly->v0 = D_000B9130.c[1];
    poly->u1 = D_000B9130.c[2];
    poly->v1 = D_000B9130.c[3];
    poly->u2 = D_000B9130.c[4];
    poly->v2 = D_000B9130.c[5] - 1;
    poly->u3 = D_000B9130.c[6];
    poly->v3 = D_000B9130.c[7] - 1;

    ((P_TAG *)poly)->len = 9;
    poly->code = 0x2E;

    poly->r0 = col->r >> 1;
    poly->g0 = col->g >> 1;
    poly->b0 = col->b >> 1;

    gte_stsz(&z);

    z >>= 3;
    if (z > 39) {
        z += LightSortCorrect;
        if (z < 1)
            z = 1;

        gte_stsxy3(&poly->x0, &poly->x1, &poly->x2);
        gte_ldv0(&vert[3]);
        gte_rtps();

        poly->tpage = D_000B9130.tpageid | 0x20;
        poly->clut = D_000B9130.clutid;

        addPrim(current->ot + z, poly);
        current->primptr += sizeof(POLY_FT4);
        gte_stsxy(&poly->x3);
    }
}
