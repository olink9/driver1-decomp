/* func_00056754: draw a model as flat red POLY_F3s (GTE rtpt per triangle) */
#include "gte.h"

typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long D_00097530;
#define current (*(DB **)&D_00097530)

typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { long vx, vy, vz, pad; } VECTOR;
typedef struct { short m[3][3]; long t[3]; } MATRIX;

typedef struct {
    SVECTOR *verts;
    unsigned char *tris;
    unsigned char ntris;
} MDL_b062;

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    int xy0;
    int xy1;
    int xy2;
} PF3_b062;

typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;

#define b062_ldv0(r0) __asm__ volatile ("lwc2  $0, 0( %0 );" "lwc2  $1, 4( %0 )" : : "r"(r0))
#define b062_ldv1(r0) __asm__ volatile ("lwc2  $2, 0( %0 );" "lwc2  $3, 4( %0 )" : : "r"(r0))
#define b062_ldv2(r0) __asm__ volatile ("lwc2  $4, 0( %0 );" "lwc2  $5, 4( %0 )" : : "r"(r0))
#define b062_rtpt() __asm__ volatile ("nop;" "nop;" "rtpt")
#define b062_stsxy3(r0, r1, r2) __asm__ volatile ( \
    "swc2  $12, 0( %0 );" "swc2  $13, 0( %1 );" "swc2  $14, 0( %2 )" : : "r"(r0), "r"(r1), "r"(r2) : "memory")
#define b062_stsz(r0) __asm__ volatile ("swc2  $19, 0( %0 )" : : "r"(r0) : "memory")

extern VECTOR D_0008A8F0;   /* camera_position */
extern MATRIX D_000B8FE8;
extern int func_000398B0();
extern int func_0002C5F0();
extern void func_0002C564();
extern void func_00057734();

void func_00056754(MDL_b062 *m, VECTOR *pos, int flag)
{
    VECTOR v[2];
    long z;
    int n;
    VECTOR *vp;
    VECTOR *pp;
    SVECTOR *verts;
    unsigned char *p;
    PF3_b062 *prim;

    if (!func_000398B0(pos))
        return;
    if (func_0002C5F0(pos, 800) == -1)
        return;

    vp = v;
    pp = pos;
    n = 1;
    verts = m->verts;
    while (n--) {
        vp->vx = pp->vx - D_0008A8F0.vx;
        vp->vy = pp->vy - D_0008A8F0.vy;
        vp->vz = pp->vz - D_0008A8F0.vz;
        func_0002C564(vp);
        pp++;
        vp++;
    }
    gte_SetRotMatrix(&D_000B8FE8);
    gte_SetTransVec0(&v[0]);

    for (p = m->tris; p < m->tris + m->ntris * 3;) {
        prim = (PF3_b062 *)current->primptr;
        b062_ldv0(&verts[*p]);
        p++;
        b062_ldv1(&verts[*p]);
        p++;
        b062_ldv2(&verts[*p]);
        p++;
        b062_rtpt();
        ((unsigned char *)prim)[3] = 4;
        prim->code = 0x20;
        prim->r0 = 0xFF;
        prim->g0 = 0;
        prim->b0 = 0;
        b062_stsxy3(&prim->xy0, &prim->xy1, &prim->xy2);
        b062_stsz(&z);
        ((P_TAG *)prim)->addr = ((P_TAG *)&current->ot[z >> 3])->addr;
        ((P_TAG *)&current->ot[z >> 3])->addr = (unsigned int)prim;
        current->primptr += 20;
    }
    if (!flag)
        func_00057734(pos);
}
