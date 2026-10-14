/* 95.17 with fnscore defaults; MATCH 100.0 with --aspsx 2.69 (only difference: gp_allow_offset=False,
   so the .comm D_00097174 is $gp-relative at +0 but lui/%lo at +1..+3, as in the target). */
/* no Driver 2 counterpart */
#include "common.h"

typedef struct {
    char pad[4];
    unsigned char x, y, w, h;
} TexInfo;

extern long long D_00096804;
extern long long D_00096834;
extern char D_00096954[];
extern int D_00097184;
extern int D_00097188;
unsigned char D_00097174[4]; /* tentative definition -> .comm */
extern unsigned char D_00097178[8];
extern unsigned short D_00086F30[];
extern unsigned short D_00087030[][32];
extern POLY_FT4 D_000A216C[2][25];
extern TexInfo *func_0005192C();

void func_0002A5CC(void)
{
    unsigned char buf[2];
    TexInfo *t;
    POLY_FT4 *p;
    int i, j;

    if (*(int *)&D_00096804 == 1)
        return;
    t = func_0005192C(D_00096954, buf);
    D_00097184 = buf[0];
    D_00097188 = buf[1];
    D_00097174[0] = t->x;
    D_00097174[1] = t->y;
    D_00097174[2] = t->x + t->w - 1;
    D_00097174[3] = t->y;
    D_00097178[0] = t->x;
    D_00097178[1] = t->y + t->h - 1;
    D_00097178[2] = t->x + t->w - 1;
    D_00097178[3] = t->y + t->h - 1;
    if (*(int *)&D_00096834 == 1) {
        D_00097174[2] = D_00097174[2] - 1;
        D_00097178[2] = D_00097178[2] - 1;
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 25; j++) {
            p = &D_000A216C[i][j];
            setPolyFT4(p);
            setRGB0(p, 0x50, 0x50, 0x50);
            p->tpage = D_00086F30[D_00097184] | 0x40;
            p->clut = D_00087030[D_00097184][D_00097188];
            setSemiTrans(p, 1);
            p->u0 = D_00097174[0];
            p->v0 = D_00097174[1];
            p->u1 = D_00097174[2];
            p->v1 = D_00097174[3];
            p->u2 = D_00097178[0];
            p->v2 = D_00097178[1];
            p->u3 = D_00097178[2];
            p->v3 = D_00097178[3];
        }
    }
}
