typedef struct { short x, y, w, h; } RECT;
extern char D_000BDFB4[];
extern RECT D_000976A4;
extern int D_000ED7D0;
extern char D_000E6BEC[];
extern char D_000E6BFC[];
extern void SetDispMask(), SetTextColour(), func_0007D66C(), func_0007D720();
extern void PutDispEnv(), PutDrawEnv(), LoadFont(), func_000EC508(), func_000EC550();
extern void Loadfile(), func_0007D608(), DrawSync();

void func_000EC280(int noinit)
{
    RECT r;
    char *p;
    short *e;
    int *buf;
    int n, i, y, h;

    buf = (int *)0xD0200;
    SetDispMask(0);
    SetTextColour(0xD6, 0x5C, 0);
    if (noinit == 0) {
        p = D_000BDFB4;
        func_0007D66C(p, 0, 0, 512, 240);
        func_0007D66C(p + 0x80, 0, 256, 512, 240);
        func_0007D720(p + 0x5C, 0, 256, 512, 240);
        func_0007D720(p + 0xDC, 0, 0, 512, 240);
        PutDispEnv(p + 0x5C);
        PutDrawEnv(p);
        D_000976A4.x = 0x3D0;
        D_000976A4.y = 0x103;
        D_000976A4.w = 0x10;
        D_000976A4.h = 1;
        LoadFont(&r);
    }
    func_000EC508(D_000E6BEC);
    if (D_000ED7D0 == 1) {
        func_000EC550(0x13, 0x11);
        func_000EC550(0x14, 0x12);
    } else {
        func_000EC550(0xE, 0x11);
        func_000EC550(0xF, 0x12);
    }
    Loadfile(D_000E6BFC, buf);
    e = (short *)buf;
    n = *(int *)e;
    e += 2;
    y = 0x3B0;
    h = 0x1FF;
    if (D_000ED7D0 == 1) {
        for (i = 0; i < n; i++) {
            if (*e == 0x80)
                *e = -1;
            if ((unsigned)(i - 21) < 2) {
                func_0007D608(e, y, h);
                DrawSync(0);
                y -= 16;
            }
            e += 16;
        }
    } else {
        for (i = 0; i < n; i++) {
            if (*e == 0x80)
                *e = -1;
            if ((unsigned)(i - 21) < 2) {
                func_0007D608(e, y, h);
                DrawSync(0);
                y -= 16;
            }
            e += 16;
        }
    }
}
