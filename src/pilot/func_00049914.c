/* Probably DrawRainDrops (Driver 2 name, weather code) */
#include "gte.h"

typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct { int m[5]; } MATRIX;
typedef struct { char *primptr; unsigned int *ot; int pad; int id; } DB;
typedef struct { int vx, vy, vz; short ox, oy, oz; short index; } RAIN_TYPE;
typedef struct {
    unsigned char coords[8];
    unsigned short tpageid;
    unsigned short clutid;
} TEXINF;
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
typedef struct { unsigned char v; char pad[7]; } B8;

typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
#define setaddr(_p, _addr) (((P_TAG *)(_p))->addr = (unsigned long)(_addr))
#define getaddr(_p) (unsigned long)(((P_TAG *)(_p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)

#define gte_rtps() __asm__ volatile ("nop;" "nop;" "rtps")
#define gte_stsxy(r0) __asm__ volatile ("swc2  $14, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsz(r0) __asm__ volatile ("swc2  $19, 0( %0 )" : : "r"(r0) : "memory")

extern int D_00097428;
extern short D_000B8028[];
extern RAIN_TYPE D_000B81C8[];
extern MATRIX D_000B8FE8;
extern TEXINF D_000B9170;
extern VECTOR camera_position;
extern B8 cameraview;
extern int D_0009B1A8[];
extern long long gNight;
extern long long pauseflag;
extern long long current;
#define current (*(DB **)&current)

extern void GetSmokeDrift(VECTOR *v);
extern void func_0002C564(VECTOR *v);

static inline void ReleaseRainDrop(int RainIndex)
{
    D_000B8028[--D_00097428] = RainIndex;
}

void func_00049914(void)
{
    SVECTOR v[4];
    VECTOR p1;
    VECTOR p2;
    VECTOR drift;
    int z;
    RAIN_TYPE *RainPtr;
    POLY_FT4 *poly;
    int Count;
    int col;

    GetSmokeDrift(&drift);

    if (D_00097428 == 0)
        return;

    RainPtr = D_000B81C8;
    Count = D_00097428;

    gte_SetRotMatrix(&D_000B8FE8);

    if (cameraview.v != 1)
        col = 150 - (D_0009B1A8[0] >> 12);
    else
        col = 150;

    if (col < 30)
        col = 30;

    if (*(int *)&gNight != 0)
        col >>= 1;

    while (Count-- != 0) {
        if (*(int *)&pauseflag == 0) {
            RainPtr->vy += 6;
            RainPtr->vx -= drift.vx;
            RainPtr->vz -= drift.vz;
        }

        poly = (POLY_FT4 *)current->primptr;
        ((P_TAG *)poly)->len = 9;
        poly->code = 0x2E;

        p1.vx = RainPtr->ox;
        p1.vy = RainPtr->oy;
        p1.vz = RainPtr->oz;

        RainPtr->ox = p2.vx = RainPtr->vx - camera_position.vx;
        RainPtr->oy = p2.vy = RainPtr->vy - camera_position.vy;
        RainPtr->oz = p2.vz = RainPtr->vz - camera_position.vz;

        if (p1.vx != 0) {
            func_0002C564(&p1);
            func_0002C564(&p2);
            gte_SetTransVec0(&p1);

            v[0].vx = v[0].vy = v[1].vy = v[2].vx = -2;
            v[1].vx = v[2].vy = v[3].vx = v[3].vy = 2;
            v[1].vz = v[0].vz = v[2].vz = v[3].vz = 0;

            gte_ldv0(&v[0]);
            gte_rtps();
            gte_stsz(&z);
            gte_stsxy(&poly->x0);

            if (z < 151 || z > 1000 || poly->x0 < -100 || poly->x0 > 612 ||
                poly->y0 < -50 || poly->y0 > 240) {
                ReleaseRainDrop(RainPtr->index);
                RainPtr++;
                continue;
            }

            gte_ldv0(&v[1]);
            gte_rtps();
            poly->tpage = D_000B9170.tpageid | 0x20;
            poly->clut = D_000B9170.clutid;
            gte_stsxy(&poly->x1);

            gte_SetTransVec0(&p2);
            gte_ldv0(&v[2]);
            gte_rtps();
            poly->u0 = D_000B9170.coords[0];
            poly->v0 = D_000B9170.coords[1];
            poly->u1 = D_000B9170.coords[2];
            poly->v1 = D_000B9170.coords[3];
            poly->u2 = D_000B9170.coords[4];
            poly->v2 = D_000B9170.coords[5];
            poly->u3 = D_000B9170.coords[6];
            poly->v3 = D_000B9170.coords[7];
            gte_stsxy(&poly->x2);

            gte_ldv0(&v[3]);
            gte_rtps();
            addPrim(current->ot + (z >> 3), poly);
            current->primptr += sizeof(POLY_FT4);
            poly->r0 = col;
            poly->g0 = col;
            poly->b0 = col;
            gte_stsxy(&poly->x3);
        }
        RainPtr++;
    }
}
