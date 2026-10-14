#include "common.h"

extern unsigned char D_000967BC[];  /* Driver 2: NewLevel */
extern int D_000967C8[];            /* Driver 2: gInFrontend */
extern int D_00097504;              /* Driver 2: loading_bar_pos */
extern int D_00097508;              /* Driver 2: load_steps */
extern DRAWENV D_000BDF24;          /* Driver 2: load_draw */
extern DISPENV D_000BDF80;          /* Driver 2: load_disp */

void SetPolyG4(POLY_G4 *p);
void DrawPrim(void *p);

/* Driver 2: ShowLoading (loadview.c) */
void func_000505E0(void)
{
    POLY_G4 poly;
    int col;

    if (D_000967BC[0] == 0 && D_000967C8[0] == 0)
        return;

    col = ++D_00097504 * D_00097508;
    if (col > 255)
        col = 255;

    SetPolyG4(&poly);
    setRGB0(&poly, 255, 0, 0);
    setRGB2(&poly, 255, 0, 0);
    setRGB1(&poly, 255, col, 0);
    setRGB3(&poly, 255, col, 0);
    setXYWH(&poly, 32, 431, col, 30);

    DrawPrim(&poly);
    VSync(0);
    DrawSync(0);
    PutDrawEnv(&D_000BDF24);
    PutDispEnv(&D_000BDF80);

    DrawPrim(&poly);
    VSync(0);
    DrawSync(0);
    PutDrawEnv(&D_000BDF24);
    PutDispEnv(&D_000BDF80);
}
