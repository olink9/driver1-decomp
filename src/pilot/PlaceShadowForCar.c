/* Driver 2: PlaceShadowForCar (SHADOW.C) */
#include "gte.h"

typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct { char *primptr; unsigned int *ot; int pad; int id; } DB;
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
    char pad0[0x88];
    VECTOR shadowPoints[4];
    char padC8[0x18C - 0xC8];
} CAR;

extern long long D_00096874;
extern long long D_00096834;
extern long long current;
#define current (*(DB **)&current)
extern CAR car_data[];
extern VECTOR camera_position;
extern VECTOR D_0008909C;
extern MATRIX inv_camera_matrix;
extern POLY_FT4 D_000A216C[2][25];

extern void SubdivShadow();

#define gte_stsxy3(r0, r1, r2) __asm__ volatile ( \
    "swc2  $12, 0( %0 );" "swc2  $13, 0( %1 );" "swc2  $14, 0( %2 )" : : "r"(r0), "r"(r1), "r"(r2) : "memory")
#define gte_stsxy(r0) __asm__ volatile ("swc2  $14, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsz3(r0, r1, r2) __asm__ volatile ( \
    "swc2  $17, 0( %0 );" "swc2  $18, 0( %1 );" "swc2  $19, 0( %2 )" : : "r"(r0), "r"(r1), "r"(r2) : "memory")
#define gte_ldv3c(r0) __asm__ volatile ( \
    "lwc2  $0, 0( %0 );" "lwc2  $1, 4( %0 );" "lwc2  $2, 8( %0 );" \
    "lwc2  $3, 12( %0 );" "lwc2  $4, 16( %0 );" "lwc2  $5, 20( %0 )" : : "r"(r0))
#define gte_rtpt() __asm__ volatile ("nop;" "nop;" "rtpt")
#define gte_rtps() __asm__ volatile ("nop;" "nop;" "rtps")

typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
#define setaddr(_p, _addr) (((P_TAG *)(_p))->addr = (unsigned long)(_addr))
#define getaddr(_p) (unsigned long)(((P_TAG *)(_p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)

#define sp (cp->shadowPoints)

void PlaceShadowForCar(int slot)
{
    SVECTOR points[4];
    int z0, z1, z2, z3;
    int z;
    CAR *cp;
    POLY_FT4 *spt;

    if (slot == 0 && *(int *)&D_00096874 == 1)
        return;

    cp = &car_data[slot];

    points[0].vx = sp[0].vx - camera_position.vx;
    points[0].vy = -sp[0].vy - camera_position.vy;
    points[0].vz = sp[0].vz - camera_position.vz;
    points[1].vx = sp[1].vx - camera_position.vx;
    points[1].vy = -sp[1].vy - camera_position.vy;
    points[1].vz = sp[1].vz - camera_position.vz;
    points[2].vx = sp[3].vx - camera_position.vx;
    points[2].vy = -sp[3].vy - camera_position.vy;
    points[2].vz = sp[3].vz - camera_position.vz;

    gte_SetTransVec0(&D_0008909C);
    gte_SetRotMatrix(&inv_camera_matrix);

    spt = &D_000A216C[current->id][slot];

    gte_ldv3c(&points[0]);
    gte_rtpt();

    points[3].vx = sp[2].vx - camera_position.vx;
    points[3].vy = -sp[2].vy - camera_position.vy;
    points[3].vz = sp[2].vz - camera_position.vz;

    gte_stsxy3(&spt->x0, &spt->x1, &spt->x3);
    gte_stsz3(&z0, &z1, &z2);
    z = z0;
    gte_ldv0(&points[3]);
    gte_rtps();
    if (z < z1)
        z = z1;
    if (z < z2)
        z = z2;
    gte_stsz(&z3);
    gte_stsxy(&spt->x2);
    if (z < z3)
        z = z3;

    if ((*(int *)&D_00096834 == 1 || *(int *)&D_00096834 == 5)
        && ((sp[0].vy | sp[1].vy | sp[2].vy | sp[3].vy) & 1)) {
        if (z0 < z1)
            z0 = (z0 + z1) / 2;
        else
            z1 = (z0 + z1) / 2;
        if (z2 < z3)
            z2 = (z2 + z3) / 2;
        else
            z3 = (z2 + z3) / 2;

        if (z0 > 28) z0 -= 20; else z0 = 8;
        if (z1 > 28) z1 -= 20; else z1 = 8;
        if (z2 > 28) z2 -= 20; else z2 = 8;
        if (z3 > 28) z3 -= 20; else z3 = 8;

        SubdivShadow(z0, z1, z2, z3, spt);
        return;
    }

    addPrim(current->ot + (z >> 3), spt);
}
