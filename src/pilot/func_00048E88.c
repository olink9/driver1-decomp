/* DisplaySmoke (Driver 2 name) */
#include "gte.h"

typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { short m[3][3]; short pad; int t[3]; } MATRIX;

typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long D_00097530;
#define current (*(DB **)&D_00097530)

typedef struct {
    VECTOR position;
    char pad10[0x28];
    short start_w;
    short pad3a;
    short transparency;
    char pad3e[0xA];
    int flags;
} SMOKE;

typedef struct {
    unsigned short uv0, uv1, uv2, uv3;
    unsigned short tpageid, clutid;
} TEXINF;

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned int uv0;
    short x1, y1;
    unsigned int uv1;
    short x2, y2;
    unsigned int uv2;
    short x3, y3;
    unsigned int uv3;
} PFT4;

typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;

extern VECTOR D_0008A8F0;
extern MATRIX D_000B8FE8;
extern TEXINF D_000B91B0;
extern long long D_00096834;
extern long long D_00096850;
#define gTimeOfDay (*(int *)&D_00096834)
#define gNight (*(int *)&D_00096850)
extern void func_0002C564(VECTOR *v);

#define gte_SetTransVector(r0) __asm__ volatile ( \
    "lw    $12, 0( %0 );" "lw    $13, 4( %0 );" "lw    $14, 8( %0 );" \
    "ctc2  $12, $5;" "ctc2  $13, $6;" "ctc2  $14, $7" : : "r"(r0) : "$12", "$13", "$14")
#define gte_ldv3c(r0) __asm__ volatile ( \
    "lwc2  $0, 0( %0 );" "lwc2  $1, 4( %0 );" "lwc2  $2, 8( %0 );" \
    "lwc2  $3, 12( %0 );" "lwc2  $4, 16( %0 );" "lwc2  $5, 20( %0 )" : : "r"(r0))
#define gte_ldv0(r0) __asm__ volatile ("lwc2  $0, 0( %0 );" "lwc2  $1, 4( %0 )" : : "r"(r0))
#define gte_rtpt() __asm__ volatile ("nop;" "nop;" "rtpt")
#define gte_rtps() __asm__ volatile ("nop;" "nop;" "rtps")
#define gte_stsz(r0) __asm__ volatile ("swc2  $19, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy(r0) __asm__ volatile ("swc2  $14, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy3(r0, r1, r2) __asm__ volatile ( \
    "swc2  $12, 0( %0 );" "swc2  $13, 0( %1 );" "swc2  $14, 0( %2 )" : : "r"(r0), "r"(r1), "r"(r2) : "memory")

void func_00048E88(SMOKE *smoke)
{
    VECTOR v;
    SVECTOR smokemesh[4];
    int Z;
    int z;
    int x, negx;
    int nw, nh;
    int smoke_z_offset;
    int tmode;
    PFT4 *poly;
    unsigned char red, green, blue;

    smoke_z_offset = 0;
    x = smoke->start_w;
    negx = (x * 3) >> 1;

    v.vx = smoke->position.vx - D_0008A8F0.vx;
    v.vy = smoke->position.vy - D_0008A8F0.vy;
    v.vz = smoke->position.vz - D_0008A8F0.vz;

    func_0002C564(&v);

    nw = -x;
    nh = -negx;
    gte_SetTransVector(&v);
    gte_SetRotMatrix(&D_000B8FE8);

    smokemesh[0].vy = nw;
    smokemesh[1].vy = nw;
    smokemesh[0].vx = nh;
    smokemesh[0].vz = 0;
    smokemesh[1].vx = negx;
    smokemesh[1].vz = 0;
    smokemesh[2].vx = nh;
    smokemesh[2].vy = x;
    smokemesh[2].vz = 0;
    smokemesh[3].vx = negx;
    smokemesh[3].vy = x;
    smokemesh[3].vz = 0;

    poly = (PFT4 *)current->primptr;

    gte_ldv3c(&smokemesh[0]);
    gte_rtpt();

    tmode = 0x20;

    if (smoke->flags & 0x4000) {
        poly->r0 = smoke->transparency;
        poly->g0 = smoke->transparency;
        poly->b0 = smoke->transparency;
    } else if (smoke->flags & 0x2000) {
        if (gNight) {
            poly->r0 = smoke->transparency >> 1;
            poly->g0 = smoke->transparency >> 1;
            poly->b0 = smoke->transparency >> 1;
        } else {
            poly->r0 = smoke->transparency >> 2;
            poly->g0 = smoke->transparency >> 2;
            poly->b0 = smoke->transparency >> 2;
        }
        tmode = 0x40;
        smoke_z_offset = 17;
    } else if (smoke->flags & 0x1000) {
        red = smoke->transparency;
        blue = smoke->transparency >> 3;
        green = blue + 50;
        if (green < 60)
            green = smoke->transparency >> 2;
        smoke_z_offset = 18;
        poly->r0 = red;
        poly->g0 = green;
        poly->b0 = blue;
    } else if (smoke->flags & 0x10000) {
        if (gTimeOfDay == 6) {
            poly->r0 = smoke->transparency;
            poly->g0 = smoke->transparency + 10;
            poly->b0 = smoke->transparency + 10;
        } else {
            poly->r0 = smoke->transparency >> 1;
            poly->g0 = smoke->transparency;
            poly->b0 = smoke->transparency + 10;
        }
        tmode = 0x40;
    } else if (smoke->flags & 0x20000) {
        if (gNight == 0) {
            poly->r0 = smoke->transparency >> 2;
            poly->g0 = smoke->transparency >> 2;
            poly->b0 = smoke->transparency >> 2;
        } else {
            poly->r0 = smoke->transparency >> 3;
            poly->g0 = smoke->transparency >> 3;
            poly->b0 = (smoke->transparency + 10) >> 3;
        }
    } else {
        if (gNight == 0) {
            poly->r0 = smoke->transparency;
            poly->g0 = smoke->transparency;
            poly->b0 = smoke->transparency + 10;
        } else {
            poly->r0 = smoke->transparency >> 1;
            poly->g0 = smoke->transparency >> 1;
            poly->b0 = (smoke->transparency + 10) >> 1;
        }
        smoke_z_offset = 25;
    }

    gte_stsxy3(&poly->x0, &poly->x1, &poly->x2);
    gte_stsz(&Z);
    gte_ldv0(&smokemesh[3]);
    gte_rtps();

    poly->uv0 = D_000B91B0.uv0 | (D_000B91B0.clutid << 16);
    poly->uv1 = D_000B91B0.uv1 | ((D_000B91B0.tpageid | tmode) << 16);
    poly->uv2 = D_000B91B0.uv2 - 0x400;
    /* poly->uv3 through a plain pointer: not MEM_IN_STRUCT, keeps the store before the Z load */
    *(unsigned int *)((char *)poly + 0x24) = D_000B91B0.uv3 - 0x400;
    ((unsigned char *)poly)[3] = 9;
    poly->code = 0x2E;

    z = (Z >> 3) - smoke_z_offset;
    ((P_TAG *)poly)->addr = ((P_TAG *)&current->ot[z])->addr;
    ((P_TAG *)&current->ot[z])->addr = (unsigned int)poly;

    gte_stsxy(&poly->x3);

    if ((poly->x1 - poly->x0) >> 1 > 100) {
        int cx, cy, l, r, t, b;
        cx = (poly->x0 + poly->x3) >> 1;
        l = cx - 50;
        cy = (poly->y0 + poly->y3) >> 1;
        r = cx + 50;
        t = (cy - 50) << 16;
        b = (cy + 50) << 16;
        *(int *)&poly->x0 = l + t;
        *(int *)&poly->x1 = r + t;
        *(int *)&poly->x2 = l + b;
        *(int *)&poly->x3 = r + b;
    }

    current->primptr += 0x28;
}
