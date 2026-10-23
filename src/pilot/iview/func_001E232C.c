/* FAKEMATCH: unused local 'SVECTOR pad[2]' only reserves sp+0x10..0x1F so c0/c1/z land at 0x20/0x28/0x30 */
/* Probably a fading trail/ribbon renderer (40 entries of 0x38 B at D_001E3758) */
#include "gte.h"

typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct { short m[3][3]; int t[3]; } MATRIX;
typedef struct { char *primptr; unsigned int *ot; } DB;

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    unsigned char r1, g1, b1, p1;
    short x1, y1;
    unsigned char u1, v1;
    unsigned short tpage;
    unsigned char r2, g2, b2, p2;
    short x2, y2;
    unsigned char u2, v2;
    unsigned short pad2;
    unsigned char r3, g3, b3, p3;
    short x3, y3;
    unsigned char u3, v3;
    unsigned short pad3;
} POLY_GT4;

typedef struct {
    unsigned char u0, v0, u1, v1, u2, v2, u3, v3;
    unsigned short tpage, clut;
} TEXDET;

typedef struct {
    VECTOR p0;
    VECTOR p1;
    SVECTOR s0;
    SVECTOR s1;
    int z;
    unsigned char r, g, b, pad;
} TRAIL;

typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;

#define gte_rtps() __asm__ volatile ("nop;" "nop;" "rtps")
#define gte_stsxy(r0) __asm__ volatile ("swc2  $14, 0( %0 )" : : "r"(r0) : "memory")

extern int D_00096874, pauseflag;
extern MATRIX inv_camera_matrix;
extern VECTOR D_0008909C;
extern VECTOR camera_position;
extern DB *current;
extern TEXDET D_000B9190;
extern TRAIL D_001E3758[];
extern int D_001E3750, D_001E35B0;

void func_001E232C(void)
{
    SVECTOR v[2];
    SVECTOR pad[2]; /* unused: sizes the frame (c0 at sp+0x20) */
    unsigned char c0[3];
    unsigned char c1[3];
    int z[2];
    int i, j;
    int wrapped;
    long *xy0, *xy1, *xy2, *xy3;
    int k;
    TRAIL *base, *a;
    POLY_GT4 *poly;

    wrapped = 0;
    if (D_00096874)
        return;

    gte_SetRotMatrix(&inv_camera_matrix);
    gte_SetTransVec0(&D_0008909C);

    for (i = 39; i != -1; i--) {
        v[0].vx = D_001E3758[i].p0.vx - camera_position.vx;
        v[0].vy = D_001E3758[i].p0.vy - camera_position.vy;
        v[0].vz = D_001E3758[i].p0.vz - camera_position.vz;
        gte_ldv0(&v[0]);
        gte_rtps();
        v[1].vx = D_001E3758[i].p1.vx - camera_position.vx;
        v[1].vy = D_001E3758[i].p1.vy - camera_position.vy;
        v[1].vz = D_001E3758[i].p1.vz - camera_position.vz;
        gte_stsz(&z[0]);
        gte_stsxy(&v[0]);
        gte_ldv0(&v[1]);
        gte_rtps();
        if (!pauseflag) {
            if (D_001E3758[i].r)
                D_001E3758[i].r--;
            if (D_001E3758[i].g >= 2)
                D_001E3758[i].g -= 2;
            if (D_001E3758[i].b >= 2)
                D_001E3758[i].b -= 2;
        }
        gte_stsxy(&v[1]);
        gte_stsz(&z[1]);
        D_001E3758[i].s0 = v[0];
        D_001E3758[i].s1 = v[1];
        D_001E3758[i].z = (z[0] + z[1]) >> 1;
    }

    i = D_001E3750 - 2;
    if (i < 0) {
        if (!D_001E35B0)
            return;
        i = D_001E3750 + 38;
        wrapped = 1;
    }

    base = D_001E3758;
    for (;;) {
        j = i + 1;
        if (j >= 40)
            j = 0;
        a = (TRAIL *)(i * sizeof(TRAIL) + (int)base);
        c0[0] = a->r;
        c0[1] = a->g;
        c0[2] = a->b;
        a = (TRAIL *)(j * sizeof(TRAIL) + (int)base);
        c1[0] = a->r;
        c1[1] = a->g;
        c1[2] = a->b;
        xy0 = (long *)&D_001E3758[i].s0;
        xy1 = (long *)&D_001E3758[i].s1;
        xy2 = (long *)&D_001E3758[j].s0;
        xy3 = (long *)&D_001E3758[j].s1;
        z[0] = (D_001E3758[i].z + a->z) >> 1;
        if (z[0] > 50) {
            poly = (POLY_GT4 *)current->primptr;
            ((P_TAG *)poly)->len = 12;
            poly->code = 0x3E;
            poly->r0 = c0[0];
            poly->g0 = c0[2];
            poly->b0 = c0[1];
            poly->r1 = c0[0];
            poly->g1 = c0[2];
            poly->b1 = c0[1];
            poly->r2 = c1[0];
            poly->g2 = c1[2];
            poly->b2 = c1[1];
            poly->r3 = c1[0];
            poly->g3 = c1[2];
            poly->b3 = c1[1];
            *(long *)&poly->x0 = *xy0;
            *(long *)&poly->x1 = *xy1;
            *(long *)&poly->x2 = *xy2;
            *(long *)&poly->x3 = *xy3;
            poly->u0 = D_000B9190.u0;
            poly->v0 = D_000B9190.v0;
            poly->u1 = D_000B9190.u1;
            poly->v1 = D_000B9190.v1;
            poly->u2 = D_000B9190.u2;
            poly->v2 = D_000B9190.v2;
            poly->u3 = D_000B9190.u3;
            poly->v3 = D_000B9190.v3;
            poly->tpage = D_000B9190.tpage | 0x20;
            poly->clut = D_000B9190.clut;
            k = z[0] >> 3;
            current->primptr += sizeof(POLY_GT4);
            ((P_TAG *)poly)->addr = ((P_TAG *)&current->ot[k])->addr;
            ((P_TAG *)&current->ot[k])->addr = (unsigned)poly;
        }
        i--;
        if (i == D_001E3750 - 1)
            return;
        if (i < 0) {
            if (wrapped || !D_001E35B0)
                return;
            i += 40;
            wrapped = 1;
        }
    }
}
