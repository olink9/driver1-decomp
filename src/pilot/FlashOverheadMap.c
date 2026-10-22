/* FlashOverheadMap */
#include "common.h"
typedef struct { char *primptr; unsigned long *ot; } BUF;
extern long long current;
#define cur (*(BUF **)&current)

void FlashOverheadMap(int r, int g, int b)
{
    TILE *prim;
    POLY_FT3 *null;

    prim = (TILE *)cur->primptr;
    setTile(prim);
    prim->x0 = 400;
    prim->y0 = 160;
    prim->w = 96;
    prim->r0 = r;
    prim->g0 = g;
    prim->b0 = b;
    setSemiTrans(prim, 1);
    prim->h = 60;
    addPrim(cur->ot, prim);
    cur->primptr += sizeof(TILE);

    null = (POLY_FT3 *)cur->primptr;
    setPolyFT3(null);
    null->x0 = -1;
    null->y0 = -1;
    null->x1 = -1;
    null->y1 = -1;
    null->x2 = -1;
    null->y2 = -1;
    null->tpage = 0x20;
    addPrim(cur->ot, null);
    cur->primptr += sizeof(POLY_FT3);
}
