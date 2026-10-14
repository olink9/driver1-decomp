#include "common.h"
typedef struct { unsigned addr:24; unsigned len:8; unsigned char r0, g0, b0, code; } PT;
#undef setlen
#undef setcode
#undef getcode
#define setlen(p, _len) (((PT *)(p))->len = (unsigned char)(_len))
#define setcode(p, _code) (((PT *)(p))->code = (unsigned char)(_code))
#define getcode(p) (unsigned char)(((PT *)(p))->code)
/* ShowLoadingScreen (Driver 2 name) */
extern DRAWENV D_000BDF24; /* load_draw */
extern DISPENV D_000BDF80; /* load_disp */
extern int D_00097504;     /* loading_bar_pos */
extern int D_00097508;     /* load_steps */
extern long long D_000967C8;
extern long long D_00096804;
extern long long D_00096834;

extern void func_00074158(int mask);   /* SetDispMask */
extern void func_0007D66C();           /* SetDefDrawEnv */
extern void func_0007D720();           /* SetDefDispEnv */
extern void func_00050E28();           /* Loadfile */
extern void func_0007D5A4();           /* LoadClut */
extern void func_00074404();           /* LoadImage */
extern int func_0007A7F4();            /* GetClut */

void func_000508B0(char *screen_name, int effect)
{
    int done;
    SPRT *sprt;
    POLY_FT3 *null;
    unsigned int tp;
    int i;
    int j;
    int fade;
    int steps;
    RECT16 dest;
    SPRT prims[4];
    POLY_FT3 nulls[4];
    int fade_step;

    fade = 255;
    fade_step = -4;
    if (effect == 1)
        func_00074158(0);

    func_0007D66C(&D_000BDF24, 0, 0, 320, 480);
    func_0007D720(&D_000BDF80, 0, 0, 320, 480);
    D_000BDF80.screen.y = 0;
    D_000BDF80.isrgb24 = 0;
    D_000BDF80.isinter = 1;

    PutDispEnv(&D_000BDF80);
    PutDrawEnv(&D_000BDF24);

    func_00050E28(screen_name, (char *)0xD0200);
    func_0007D5A4((char *)0xD0214, 320, 511);
    DrawSync(0);

    setRECT(&dest, 320, 0, 160, 511);
    func_00074404(&dest, (char *)0xD0420);
    DrawSync(0);

    done = 0;
    null = nulls;
    sprt = prims;
    i = 0;
    tp = 320;
    while (i < 2) {
        setSprt(sprt);
        sprt->x0 = (i << 8);
        sprt->y0 = 0;
        sprt->u0 = 0;
        sprt->v0 = 0;
        sprt->w = 256;
        sprt->h = 256;
        sprt->clut = func_0007A7F4(320, 511);

        setPolyFT3(null);
        null->x0 = -1;
        null->y0 = -1;
        null->x1 = -1;
        null->y1 = -1;
        null->x2 = -1;
        null->y2 = -1;
        setSemiTrans(null, 0);
        setShadeTex(null, 0);
        null->tpage = GetTPage(1, 0, tp, 0);

        null++;
        sprt++;
        tp += 128;
        i++;
    }

    sprt = &prims[2];
    i = 0;
    tp = 320;
    while (i < 2) {
        null = &nulls[i + 2];
        setSprt(sprt);
        sprt->x0 = (i << 8);
        sprt->y0 = 256;
        sprt->u0 = 0;
        sprt->v0 = 0;
        sprt->w = 256;
        sprt->h = 256;
        sprt->clut = func_0007A7F4(320, 511);

        setPolyFT3(null);
        null->x0 = -1;
        null->y0 = -1;
        null->x1 = -1;
        null->y1 = -1;
        null->x2 = -1;
        null->y2 = -1;
        setSemiTrans(null, 0);
        setShadeTex(null, 0);
        null->tpage = GetTPage(1, 0, tp, 256);

        sprt++;
        tp += 128;
        i++;
    }

    if (effect == 1) {
        fade = 0;
        fade_step = 2;
    } else if (effect == 2) {
        fade = 128;
        fade_step = -2;
    }

    i = 0;
    while (!done) {
        if (i == 2)
            func_00074158(1);

        for (j = 0; j < 4; j++) {
            prims[j].r0 = fade;
            prims[j].g0 = fade;
            prims[j].b0 = fade;
        }
        for (j = 0; j < 4; j++) {
            DrawPrim(&nulls[j]);
            DrawPrim(&prims[j]);
        }
        DrawSync(0);
        VSync(0);
        PutDispEnv(&D_000BDF80);
        PutDrawEnv(&D_000BDF24);

        fade += fade_step;
        i++;
        if (effect == 1) {
            if (fade > 127)
                done = 1;
        } else if (effect == 2) {
            if (fade < 0)
                done = 1;
        }
    }

    D_00097504 = 0;
    if (*(int *)&D_000967C8 != 0)
        steps = 32;
    else if (*(int *)&D_00096804 != 0)
        steps = 42;
    else {
        switch (*(int *)&D_00096834) {
        case 0: steps = 32; break;
        case 1: steps = 32; break;
        case 2: steps = 32; break;
        case 3: steps = 32; break;
        case 4: steps = 32; break;
        case 5: steps = 32; break;
        case 6: steps = 32; break;
        case 7: steps = 32; break;
        default: steps = 32; break;
        }
    }
    D_00097508 = steps;
}
