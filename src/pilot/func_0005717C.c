#include "common.h"

typedef struct { char *primptr; unsigned long *ot; } DB;
extern long long D_00097530;
#define current (*(DB **)&D_00097530)

typedef struct {
    unsigned char u0, v0, u1, v1, u2, v2, u3, v3;
    unsigned short tpage, clut;
} SPRT_UV;

typedef struct { int x, pad, y, pad2; } PT;

extern unsigned char D_000975E0;
extern SPRT_UV D_000B9170;

void func_0005717C(PT *p, int n, unsigned char *col, unsigned char size, unsigned char scale, unsigned short tp)
{
    POLY_FT4 *poly;
    int s;
    int w;
    int x, y;

    while (n--) {
        if ((unsigned)(p->x - 388) < 121 && p->y >= 152 && p->y < 229) {
            poly = (POLY_FT4 *)current->primptr;
            if (scale)
                s = (D_000975E0 * size) / 8;
            else
                s = size;
            w = (s << 9) / 320;
            x = p->x;
            y = p->y;
            setPolyFT4(poly);
            poly->y0 = y - s;
            poly->y1 = y - s;
            poly->y2 = y + s;
            poly->y3 = y + s;
            poly->x0 = x - w;
            poly->x1 = x + w;
            poly->x2 = x - w;
            poly->x3 = x + w;
            poly->r0 = col[0];
            poly->g0 = col[1];
            poly->b0 = col[2];
            setSemiTrans(poly, 1);
            poly->u0 = D_000B9170.u0;
            poly->v0 = D_000B9170.v0;
            poly->u1 = D_000B9170.u1;
            poly->v1 = D_000B9170.v1;
            poly->u2 = D_000B9170.u2;
            poly->v2 = D_000B9170.v2;
            poly->u3 = D_000B9170.u3;
            poly->v3 = D_000B9170.v3;
            poly->tpage = D_000B9170.tpage | tp;
            poly->clut = D_000B9170.clut;
            addPrim(current->ot, poly);
            current->primptr += sizeof(POLY_FT4);
        }
        p++;
    }
}
