/* Driver 2: add_haze */
#include "common.h"
typedef struct { unsigned char *primptr; unsigned long *ot; } DB;
extern long long FastForward;
extern long long current;
#define current (*(DB **)&current)

void add_haze(int top_col, int bot_col, short ot_pos)
{
    TILE *polys;
    POLY_FT3 *null;

    if (top_col > 9 && *(int *)&FastForward == 0) {
        polys = (TILE *)current->primptr;
        polys->code = 0x60;
        setlen(polys, 3);
        polys->r0 = top_col;
        polys->g0 = top_col;
        polys->b0 = top_col;
        setSemiTrans(polys, 1);
        addPrim(current->ot + ot_pos, polys);
        current->primptr += sizeof(TILE);
        polys->x0 = 0;
        polys->y0 = 0;
        polys->w = 512;
        polys->h = 240;

        null = (POLY_FT3 *)current->primptr;
        setPolyFT3(null);
        null->x0 = -1;
        null->y0 = -1;
        null->x1 = -1;
        null->y1 = -1;
        null->x2 = -1;
        null->y2 = -1;
        null->tpage = 0x20;
        addPrim(current->ot + ot_pos, null);
        current->primptr += sizeof(POLY_FT3);
    }
}
