#include "common.h"
/* FadeGameScreen (Driver 2) */
extern int D_00096CB4;
extern int D_00096CBC;
extern int D_00096CC4;
extern POLY_GT4 D_000BDEF0;
extern POLY_G4 D_000BDEC0;
extern void DrawPrim();

void func_0005073C(int flag, int speed) {
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
        setlen(&D_000BDEC0, 8);
        D_000BDEC0.code = 0x38;
        val = D_00096CB4;
        D_000BDEC0.y2 = 0x100; D_000BDEC0.y3 = 0x100;
        D_000BDEC0.x3 = D_000BDEC0.x1 = 0x200;
        D_000BDEC0.x0 = 0; D_000BDEC0.y0 = 0; D_000BDEC0.y1 = 0; D_000BDEC0.x2 = 0;
        D_000BDEC0.code = 0x3A;
        D_000BDEC0.r0 = val; D_000BDEC0.g0 = val; D_000BDEC0.b0 = val;
        D_000BDEC0.r1 = val; D_000BDEC0.g1 = val; D_000BDEC0.b1 = val;
        D_000BDEC0.r2 = val; D_000BDEC0.g2 = val; D_000BDEC0.b2 = val;
        D_000BDEC0.r3 = val; D_000BDEC0.g3 = val; D_000BDEC0.b3 = val;
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
