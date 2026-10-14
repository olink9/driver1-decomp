/* Driver 2: DisplayLightReflections (DEBRIS.C) */
#include "gte.h"

typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct { unsigned char r, g, b, cd; } CVECTOR;
typedef struct { char *primptr; unsigned int *ot; } DB;
typedef struct { int m[5]; } MATRIX;

typedef struct {
    unsigned short coords[4];
    unsigned short tpageid;
    unsigned short clutid;
} TEXTURE_DETAILS;

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned int uv0clut;
    short x1, y1;
    unsigned int uv1tpage;
    short x2, y2;
    unsigned int uv2;
    short x3, y3;
    unsigned int uv3;
} POLY_FT4;

extern long long D_00097530;
#define current (*(DB **)&D_00097530)
extern int D_00097424;
extern MATRIX D_000AF9C0;
extern void func_0002C564(VECTOR *v);

#define gte_stsxy3(r0, r1, r2) __asm__ volatile ( \
    "swc2  $12, 0( %0 );" "swc2  $13, 0( %1 );" "swc2  $14, 0( %2 )" : : "r"(r0), "r"(r1), "r"(r2) : "memory")
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

void func_0004A398(VECTOR *v1, CVECTOR *col, int size, TEXTURE_DETAILS *texture)
{
    SVECTOR vert[4];
    VECTOR pos;
    CVECTOR thiscol;
    int z;
    POLY_FT4 *poly;
    int tpage;
    int clut;

    tpage = (texture->tpageid | 0x20) << 16;
    clut = texture->clutid << 16;

    if (D_00097424 > 9) {
        pos.vx = v1->vx;
        pos.vz = v1->vz;
        pos.vy = v1->vy;

        gte_SetTransVec0(v1);
        func_0002C564(v1);
        gte_SetTransVec0(v1);
        gte_SetRotMatrix(&D_000AF9C0);

        vert[0].vx = -size;
        vert[2].vx = -size;
        vert[1].vx = size;
        vert[3].vx = size;
        vert[0].vy = 0;
        vert[0].vz = 0;

        gte_ldv0(&vert[0]);
        gte_rtps();

        vert[1].vy = 0;
        vert[1].vz = 0;
        vert[2].vz = 0;
        vert[3].vz = 0;

        gte_stsz(&z);

        if (z >= 150) {
            poly = (POLY_FT4 *)current->primptr;

            gte_stsxy(&poly->x0);

            vert[2].vy = (z >> 3) + 250;
            vert[3].vy = (z >> 3) + 250;

            gte_ldv3c(&vert[1]);
            gte_rtpt();

            poly->uv0clut = texture->coords[0] | clut;
            poly->uv1tpage = texture->coords[1] | tpage;
            poly->uv2 = texture->coords[2];
            poly->uv3 = texture->coords[3];

            thiscol.r = col->r >> 3;
            thiscol.g = col->g >> 3;
            thiscol.b = col->b >> 3;

            *(int *)&poly->r0 = *(int *)&thiscol;
            ((P_TAG *)poly)->len = 9;
            poly->code = 0x2E;

            addPrim(current->ot + (z >> 4), poly);
            current->primptr += sizeof(POLY_FT4);

            gte_stsxy3(&poly->x1, &poly->x2, &poly->x3);
        }
    }
}
