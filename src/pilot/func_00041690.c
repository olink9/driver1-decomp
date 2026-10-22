/* particle sprites (no Driver 2 counterpart) */
#include "gte.h"

typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct { char *primptr; unsigned int *ot; } DB;
typedef struct { int m[5]; } MATRIX;

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0; unsigned short clut;
    short x1, y1;
    unsigned char u1, v1; unsigned short tpage;
    short x2, y2;
    unsigned char u2, v2; unsigned short pad1;
    short x3, y3;
    unsigned char u3, v3; unsigned short pad2;
} POLY_FT4;

typedef struct {
    unsigned char u0, v0, u1, v1, u2, v2, u3, v3;
    unsigned short tpage;
    unsigned short clut;
} SPRTEX;

typedef struct {
    int x, y, z;
    short unk0c, unk0e;
    short vx, vy, vz;
    short pad;
    short life;
    short frame;
} PART;

extern long long current;
#define current (*(DB **)&current)
extern int D_00097420;
extern PART D_000B7E1C[];
extern VECTOR camera_position;
extern MATRIX D_000B8FE8;
extern SPRTEX D_000BBE24;
extern SPRTEX D_000BBE34;
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

void func_00041690(void)
{
    PART *p;
    int i;
    VECTOR pos;
    SVECTOR vert[4];
    int z;
    int w, h;
    POLY_FT4 *poly;

    p = D_000B7E1C;
    i = D_00097420;
    if (p->life == 0)
        return;

    for (i--; i != -1; p++, i--) {
        if (p->life > 0) {
            pos.vx = p->x - camera_position.vx;
            pos.vy = p->y - camera_position.vy;
            pos.vz = p->z - camera_position.vz;
            func_0002C564(&pos);
            gte_SetTransVec0(&pos);
            gte_SetRotMatrix(&D_000B8FE8);

            if ((unsigned)(pos.vz - 0xE1) >= 0x33DB) {
                p->life = 0;
                return;
            }

            if (p->frame >= 4) {
                w = 0x40;
                h = 0x18;
            } else {
                w = 0x28;
                h = 0x13;
            }

            vert[0].vx = -w;
            vert[0].vy = -h;
            vert[0].vz = 0;
            vert[1].vx = w;
            vert[1].vy = -h;
            vert[1].vz = 0;
            vert[2].vx = -w;
            vert[2].vy = h;
            vert[2].vz = 0;
            vert[3].vx = w;
            vert[3].vy = h;
            vert[3].vz = 0;

            gte_ldv3c(&vert[0]);
            gte_rtpt();

            poly = (POLY_FT4 *)current->primptr;

            if (p->frame >= 4) {
                poly->u0 = D_000BBE24.u0;
                poly->v0 = D_000BBE24.v0;
                poly->u1 = D_000BBE24.u1;
                poly->v1 = D_000BBE24.v1;
                poly->u2 = D_000BBE24.u2;
                poly->v2 = D_000BBE24.v2;
                poly->u3 = D_000BBE24.u3;
                poly->v3 = D_000BBE24.v3;
                poly->tpage = D_000BBE24.tpage;
                poly->clut = D_000BBE24.clut;
            } else {
                poly->u0 = D_000BBE34.u0;
                poly->v0 = D_000BBE34.v0;
                poly->u1 = D_000BBE34.u1;
                poly->v1 = D_000BBE34.v1;
                poly->u2 = D_000BBE34.u2;
                poly->v2 = D_000BBE34.v2;
                poly->u3 = D_000BBE34.u3;
                poly->v3 = D_000BBE34.v3;
                poly->tpage = D_000BBE34.tpage;
                poly->clut = D_000BBE34.clut;
            }

            ((P_TAG *)poly)->len = 9;
            poly->code = 0x2C;
            poly->r0 = 0x80;
            poly->g0 = 0x80;
            poly->b0 = 0x80;

            gte_stsxy3(&poly->x0, &poly->x1, &poly->x2);
            gte_stsz(&z);

            if (z < 150)
                return;

            gte_ldv0(&vert[3]);
            gte_rtps();

            addPrim(current->ot + (z >> 3), poly);
            current->primptr += sizeof(POLY_FT4);

            p->x += p->vx;
            p->y += p->vy;
            p->z += p->vz;

            gte_stsxy(&poly->x3);

            if (--p->frame == -1)
                p->frame = 8;
            p->life--;
        }
    }
}
