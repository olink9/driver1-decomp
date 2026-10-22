/* Driver 2: DisplaySpark (DEBRIS.C) */
#include "gte.h"

typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct { char *primptr; unsigned int *ot; } DB;
typedef struct { short m[3][3]; int t[3]; } MATRIX;

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char r1, g1, b1, pad1;
    short x1, y1;
    unsigned char r2, g2, b2, pad2;
    short x2, y2;
} POLY_G3;

typedef struct {
    VECTOR final_tail_pos;
    char pad10[0x28 - 0x10];
    VECTOR position;
    short start_w;
    short pad3A;
    short transparency;
} SMOKE;

extern long long current;
#define current (*(DB **)&current)
extern MATRIX inv_camera_matrix;
extern VECTOR D_0008909C;
extern VECTOR camera_position;
extern long long D_00096834;
extern long long gNight;

#define gte_stsxy3(r0, r1, r2) __asm__ volatile ( \
    "swc2  $12, 0( %0 );" "swc2  $13, 0( %1 );" "swc2  $14, 0( %2 )" : : "r"(r0), "r"(r1), "r"(r2) : "memory")
#define gte_ldv3c(r0) __asm__ volatile ( \
    "lwc2  $0, 0( %0 );" "lwc2  $1, 4( %0 );" "lwc2  $2, 8( %0 );" \
    "lwc2  $3, 12( %0 );" "lwc2  $4, 16( %0 );" "lwc2  $5, 20( %0 )" : : "r"(r0))
#define gte_rtpt() __asm__ volatile ("nop;" "nop;" "rtpt")

typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
#define setaddr(_p, _addr) (((P_TAG *)(_p))->addr = (unsigned long)(_addr))
#define getaddr(_p) (unsigned long)(((P_TAG *)(_p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)

void DisplaySpark(SMOKE *spark)
{
    SVECTOR v[3];
    SVECTOR TrailPos;
    int z;
    POLY_G3 *poly;

    gte_SetRotMatrix(&inv_camera_matrix);
    gte_SetTransVec0(&D_0008909C);

    v[2].vx = spark->position.vx - camera_position.vx;
    v[2].vy = spark->position.vy - camera_position.vy;
    v[2].vz = spark->position.vz - camera_position.vz;

    v[0].vx = spark->final_tail_pos.vx - camera_position.vx;
    v[0].vy = spark->final_tail_pos.vy - camera_position.vy;
    v[0].vz = spark->final_tail_pos.vz - camera_position.vz;

    v[1].vx = v[0].vx + 5;
    v[1].vy = v[0].vy;
    v[1].vz = v[0].vz + 5;

    poly = (POLY_G3 *)current->primptr;

    gte_ldv3c(&v[0]);
    gte_rtpt();

    ((P_TAG *)poly)->len = 6;
    poly->code = 0x30;

    if (spark->start_w == 2) {
        if (*(int *)&D_00096834 == 6) {
            poly->r0 = 0xA0;
            poly->g0 = 0x3C;
            poly->b0 = 0x32;
            poly->r1 = 0xA0;
            poly->g1 = 0x3C;
            poly->b1 = 0x32;
            poly->r2 = 0xA0;
            poly->g2 = 0x3C;
            poly->b2 = 0x32;
        } else if (*(int *)&gNight == 0) {
            poly->r0 = 0;
            poly->g0 = spark->transparency >> 2;
            poly->b0 = 0;
            poly->r1 = 0;
            poly->g1 = spark->transparency >> 2;
            poly->b1 = 0;
            poly->r2 = (0x80 - spark->transparency) >> 2;
            poly->g2 = 0x20;
            poly->b2 = 0;
        } else {
            poly->r0 = 0;
            poly->g0 = spark->transparency >> 2;
            poly->b0 = 0;
            poly->r1 = 0;
            poly->g1 = spark->transparency >> 2;
            poly->b1 = 0;
            poly->r2 = 0x14;
            poly->g2 = 0x0A;
            poly->b2 = 0;
        }
    } else {
        poly->r0 = spark->transparency;
        poly->g0 = spark->transparency;
        poly->b0 = spark->transparency >> 1;
        poly->r1 = spark->transparency;
        poly->g1 = spark->transparency;
        poly->b1 = spark->transparency >> 1;
        poly->r2 = spark->transparency;
        poly->g2 = 0;
        poly->b2 = 0;
    }

    gte_stsz(&z);

    if (z > 50) {
        gte_stsxy3(&poly->x0, &poly->x1, &poly->x2);
        addPrim(current->ot + (z >> 3), poly);
        current->primptr += sizeof(POLY_G3);
    }
}
