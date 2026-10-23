#include "common.h"
/* FadeGameScreen (Driver 2) */
extern int D_00096CB4;
extern int D_00096CBC;
extern int D_00096CC4;
extern POLY_GT4 D_000BDEF0;
extern POLY_G4 D_000BDEC0;
extern void DrawPrim();

void FadeGameScreen(int flag, int speed) {
    int do_fade;
    if (flag != 0)
        do_fade = D_00096CB4 > D_00096CBC;
    else
        do_fade = D_00096CB4 < D_00096CBC;
    if (do_fade) {
        unsigned char val;
        setlen(&D_000BDEF0, 12);
        D_000BDEF0.code = 0x3E;
        D_000BDEF0.x0 = -1; D_000BDEF0.y0 = -1;
        D_000BDEF0.x1 = 0; D_000BDEF0.y1 = -1;
        D_000BDEF0.x2 = -1; D_000BDEF0.y2 = 0;
        D_000BDEF0.x3 = 0; D_000BDEF0.y3 = 0;
        D_000BDEF0.tpage = GetTPage(0, 2, 0, 0);
        setPolyG4(&D_000BDEC0);
        val = D_00096CB4;
        setRGB0(&D_000BDEC0, val, val, val);
        setRGB1(&D_000BDEC0, val, val, val);
        setRGB2(&D_000BDEC0, val, val, val);
        setRGB3(&D_000BDEC0, val, val, val);
        setXYWH(&D_000BDEC0, 0, 0, 512, 256);
        setSemiTrans(&D_000BDEC0, 1);
        if (flag != 0)
            D_00096CB4 = D_00096CB4 - speed;
        else
            D_00096CB4 = D_00096CB4 + speed;
        DrawPrim(&D_000BDEF0);
        DrawPrim(&D_000BDEC0);
    } else {
        D_00096CC4 = 0;
    }
}
