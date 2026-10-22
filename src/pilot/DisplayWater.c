/* Driver 2: DisplayWater (DEBRIS.C) */
#include "gte.h"

typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { int vx, vy, vz, pad; } VECTOR;
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
    unsigned char u0, v0, u1, v1, u2, v2, u3, v3;
    unsigned short tpageid;
    unsigned short clutid;
} TEXINF;

typedef struct {
    VECTOR position;
    char pad[0x38 - 0x10];
    short start_w;
} SMOKE;

extern long long current;
#define current (*(DB **)&current)
extern MATRIX inv_camera_matrix;
extern VECTOR D_0008909C;
extern VECTOR camera_position;
extern long long D_00097128;
extern TEXINF D_000B9200;

#define gte_stsxy3(r0, r1, r2) __asm__ volatile ( \
    "swc2  $12, 0( %0 );" "swc2  $13, 0( %1 );" "swc2  $14, 0( %2 )" : : "r"(r0), "r"(r1), "r"(r2) : "memory")
#define gte_stsz3(r0, r1, r2) __asm__ volatile ( \
    "swc2  $17, 0( %0 );" "swc2  $18, 0( %1 );" "swc2  $19, 0( %2 )" : : "r"(r0), "r"(r1), "r"(r2) : "memory")
#define gte_stsxy(r0) __asm__ volatile ("swc2  $14, 0( %0 )" : : "r"(r0) : "memory")
#define gte_ldv3c(r0) __asm__ volatile ( \
    "lwc2  $0, 0( %0 );" "lwc2  $1, 4( %0 );" "lwc2  $2, 8( %0 );" \
    "lwc2  $3, 12( %0 );" "lwc2  $4, 16( %0 );" "lwc2  $5, 20( %0 )" : : "r"(r0))
#define gte_rtpt() __asm__ volatile ("nop;" "nop;" "rtpt")
#define gte_rtps() __asm__ volatile ("nop;" "nop;" "rtps")

typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
#define setaddr(_p, _addr) (((P_TAG *)(_p))->addr = (unsigned long)(_addr))
#define getaddr(_p) (unsigned long)(((P_TAG *)(_p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)

void DisplayWater(SMOKE *smoke)
{
    char pad[16];
    POLY_FT4 *poly;
    SVECTOR vert[4];
    int size;
    int z;
    int z1;
    int z2;
    int z3;
    int z4;

    gte_SetRotMatrix(&inv_camera_matrix);
    gte_SetTransVec0(&D_0008909C);

    size = smoke->start_w;

    vert[0].vx = (smoke->position.vx - size) - camera_position.vx;
    vert[0].vy = 50 - camera_position.vy;
    vert[0].vz = (smoke->position.vz + size) - camera_position.vz;

    vert[1].vx = (smoke->position.vx + size) - camera_position.vx;
    vert[1].vy = 50 - camera_position.vy;
    vert[1].vz = (smoke->position.vz + size) - camera_position.vz;

    vert[2].vx = (smoke->position.vx - size) - camera_position.vx;
    vert[2].vy = 50 - camera_position.vy;
    vert[2].vz = (smoke->position.vz - size) - camera_position.vz;

    vert[3].vx = (smoke->position.vx + size) - camera_position.vx;
    vert[3].vy = 50 - camera_position.vy;
    vert[3].vz = (smoke->position.vz - size) - camera_position.vz;

    poly = (POLY_FT4 *)current->primptr;

    gte_ldv3c(&vert[0]);
    gte_rtpt();

    ((P_TAG *)poly)->len = 9;
    poly->code = 0x2C;

    poly->r0 = *(unsigned char *)&D_00097128;
    poly->g0 = *(unsigned char *)&D_00097128;
    poly->b0 = *(unsigned char *)&D_00097128;
    poly->tpage = D_000B9200.tpageid;

    gte_stsxy3(&poly->x0, &poly->x1, &poly->x2);
    gte_stsz3(&z1, &z2, &z3);

    gte_ldv0(&vert[3]);
    gte_rtps();

    poly->u0 = D_000B9200.u0;
    poly->v0 = D_000B9200.v0;
    poly->u1 = D_000B9200.u1;
    poly->v1 = D_000B9200.v1;
    poly->u2 = D_000B9200.u2;
    poly->v2 = D_000B9200.v2;
    poly->u3 = D_000B9200.u3;
    poly->v3 = D_000B9200.v3;
    poly->clut = D_000B9200.clutid;

    gte_stsz(&z4);

    z = z1 + z2 + z3 + z4 >> 5;

    addPrim(current->ot + z, poly);
    gte_stsxy(&poly->x3);

    current->primptr += sizeof(POLY_FT4);
}
