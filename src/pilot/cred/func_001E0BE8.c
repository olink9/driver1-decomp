/* func_001E0BE8: 5 POLY_FT4 strips + an unlinked DR_STP. The 0x2C|2 at runtime needs setSemiTrans right after setRGB0 with setUV4 last;
   the tpage constant lives in a local set before the loop (getTPage(2,1,896,256) = 0x13E). */
#include "common.h"

typedef struct { char *primptr; unsigned long *ot; } DB;
typedef struct { unsigned long tag; unsigned long code[2]; } DR_STP12;
extern DB *current;
extern void func_00074D2C(DR_STP12 *p, int pbw); /* SetDrawStp */

void func_001E0BE8(int x, int y, int tile, int c)
{
    POLY_FT4 *p;
    int i;
    int u, v;
    int tpage;

    tile *= 5;
    tpage = getTPage(2, 1, 896, 256);
    p = (POLY_FT4 *)current->primptr;
    for (i = 0; i < 5; i++) {
        p->tpage = tpage;
        setXY4(p, x, y, x + 16, y, x, y + 64, x + 16, y + 64);
        x += 16;
        u = (tile & 3) << 4;
        v = (tile >> 2) << 6;
        tile++;
        setPolyFT4(p);
        setRGB0(p, c, c, c);
        setSemiTrans(p, 1);
        setUV4(p, u, v, u + 16, v, u, v + 64, u + 16, v + 64);
        addPrim(current->ot + 3, p);
        p++;
    }
    func_00074D2C((DR_STP12 *)p, 1);
    current->primptr = (char *)p + 12;
}
