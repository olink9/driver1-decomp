/* Driver 2: DrawStopZone (TARGETS.C) */
#include "gte.h"

typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct { char *primptr; unsigned int *ot; } DB;
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

typedef struct { unsigned addr:24; unsigned len:8; unsigned char r0, g0, b0, code; } P_TAG;
#define setlen(p, _len) (((P_TAG *)(p))->len = (unsigned char)(_len))
#define setcode(p, _code) (((P_TAG *)(p))->code = (unsigned char)(_code))
#define getcode(p) (unsigned char)(((P_TAG *)(p))->code)
#define setPolyFT4(p) setlen(p, 9), setcode(p, 0x2C)
#define setSemiTrans(p, abe) ((abe) ? setcode(p, getcode(p) | 0x02) : setcode(p, getcode(p) & ~0x02))
#define setaddr(_p, _addr) (((P_TAG *)(_p))->addr = (unsigned long)(_addr))
#define getaddr(_p) (unsigned long)(((P_TAG *)(_p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)

extern long long D_00097530;
#define current (*(DB **)&D_00097530)
extern MATRIX D_000B8FE8;
extern VECTOR D_0008909C;
extern VECTOR D_0008A8F0;
extern volatile long long D_000970D8;
#define CameraCnt (*(volatile int *)&D_000970D8)
extern long long D_00096850;
extern unsigned char D_000B9170[];
extern unsigned short D_000B917A[];
extern int func_0001F5B0();
extern void func_0002C564();
extern void func_0007EAF4();

void func_00057734(VECTOR *pPosition)
{
    VECTOR pStopZonePt[4] = {
        {-500, 0, 500},
        {500, 0, 500},
        {-500, 0, -500},
        {500, 0, -500},
    };
    SVECTOR temp;
    long p;
    long flag;
    int sz;
    VECTOR *pVector;
    POLY_FT4 *pPoly;
    int *pOut;

    gte_SetRotMatrix(&D_000B8FE8);
    gte_SetTransVec0(&D_0008909C);

    pVector = pStopZonePt;
    pPoly = (POLY_FT4 *)current->primptr;
    setPolyFT4(pPoly);
    pOut = (int *)&pPoly->x0;

    while (pVector < &pStopZonePt[4]) {
        pVector->vx = pVector->vx * (-CameraCnt % 16) / 16;
        pVector->vz = pVector->vz * (-CameraCnt % 16) / 16;
        pVector->vx += pPosition->vx;
        pVector->vz += pPosition->vz;
        pVector->vy = -func_0001F5B0(pVector);
        pVector->vx -= D_0008A8F0.vx;
        pVector->vy -= D_0008A8F0.vy;
        pVector->vz -= D_0008A8F0.vz;
        func_0002C564(pVector);
        temp.vx = pVector->vx;
        temp.vy = pVector->vy;
        temp.vz = pVector->vz;
        func_0007EAF4(&temp, pOut, &p, &flag);

        if (pOut == (int *)&pPoly->x0)
            pOut = (int *)&pPoly->x1;
        else if (pOut == (int *)&pPoly->x1)
            pOut = (int *)&pPoly->x2;
        else
            pOut = (int *)&pPoly->x3;

        pVector++;
    }

    gte_stsz(&sz);

    pPoly->r0 = 64;
    pPoly->g0 = 64;
    pPoly->b0 = 64;
    setSemiTrans(pPoly, 1);

    pPoly->u0 = D_000B9170[0];
    pPoly->v0 = D_000B9170[1];
    pPoly->u1 = D_000B9170[2];
    pPoly->v1 = D_000B9170[3];
    pPoly->u2 = D_000B9170[4];
    pPoly->v2 = D_000B9170[5];
    pPoly->u3 = D_000B9170[6];
    pPoly->v3 = D_000B9170[7];

    if (*(int *)&D_00096850)
        pPoly->tpage = *(unsigned short *)&D_000B9170[8] | 0x20;
    else
        pPoly->tpage = *(unsigned short *)&D_000B9170[8] | 0x40;

    pPoly->clut = D_000B917A[0];

    addPrim(current->ot + (sz >> 4) + 512, pPoly);

    current->primptr += sizeof(POLY_FT4);
}
