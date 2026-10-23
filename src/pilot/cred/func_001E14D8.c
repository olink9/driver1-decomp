#include "common.h"

typedef struct { char *primptr; unsigned long *ot; } DB;
typedef struct {
    unsigned char u0, v0, u1, v1, u2, v2, u3, v3;
    unsigned short tpageid;
    unsigned short clutid;
    short pad;
} TEX14;
extern DB *current;
extern TEX14 D_001F3828[];

int func_001E14D8(int n, int y)
{
    SPRT *p;
    DR_MODE *dr;

    if (y + 64 < 48)
        return 0;
    p = (SPRT *)current->primptr;
    setSprt(p);
    setRGB0(p, 128, 128, 128);
    p->x0 = 360;
    p->y0 = y;
    p->u0 = D_001F3828[n].u0;
    p->v0 = D_001F3828[n].v0;
    p->w = D_001F3828[n].u3 - D_001F3828[n].u0;
    p->h = D_001F3828[n].v3 - D_001F3828[n].v0;
    p->clut = D_001F3828[n].clutid;
    addPrim(current->ot + 5, p);
    dr = (DR_MODE *)(p + 1);
    SetDrawMode(dr, 0, 0, D_001F3828[n].tpageid, 0);
    addPrim(current->ot + 5, dr);
    current->primptr = (char *)(dr + 1);
    return 1;
}
