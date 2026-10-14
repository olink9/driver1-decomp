#include "common.h"
typedef struct { char *primptr; unsigned long *ot; } BUF;
extern long long D_00097530;
#define cur (*(BUF **)&D_00097530)

void func_000555F4(int x, int level)
{
    unsigned long *ot;
    POLY_F3 *poly;
    DR_TPAGE *tp;
    int grey;

    ot = cur->ot;
    if (level > 255)
        level = 255;
    grey = (level * (level + 256)) >> 9;

    poly = (POLY_F3 *)cur->primptr;
    setPolyF3(poly);
    poly->r0 = level;
    poly->g0 = level / 4;
    poly->b0 = level / 4;
    poly->x0 = x - 20;
    poly->y0 = 240;
    poly->x1 = x;
    poly->y1 = 210;
    poly->x2 = x + 20;
    poly->y2 = 240;
    setSemiTrans(poly, 1);
    addPrim(ot + 1, poly);
    cur->primptr += sizeof(POLY_F3);

    tp = (DR_TPAGE *)cur->primptr;
    setlen(tp, 1);
    tp->code[0] = 0xE1000620;
    addPrim(ot + 1, tp);
    cur->primptr += sizeof(DR_TPAGE);

    poly = (POLY_F3 *)cur->primptr;
    setPolyF3(poly);
    poly->r0 = grey;
    poly->g0 = grey;
    poly->b0 = grey;
    poly->x0 = x - 20;
    poly->y0 = 240;
    poly->x1 = x;
    poly->y1 = 210;
    poly->x2 = x + 20;
    poly->y2 = 240;
    setSemiTrans(poly, 1);
    addPrim(ot + 1, poly);
    cur->primptr += sizeof(POLY_F3);

    tp = (DR_TPAGE *)cur->primptr;
    setlen(tp, 1);
    tp->code[0] = 0xE1000640;
    addPrim(ot + 1, tp);
    cur->primptr += sizeof(DR_TPAGE);
}
