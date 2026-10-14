#include "common.h"
extern unsigned short D_00089118[];
extern char D_00089134[];
extern POLY_FT4 D_000A7B3C;
extern void func_00074404();
extern int func_0007A7F4();

void func_0003336C(void) {
    int i;
    unsigned short *pal = D_00089118;
    struct { short x, y, w, h; } r;
    POLY_FT4 *p;
    for (i = 0; i < 14; i++) pal[i] &= 0x7FFF;
    pal[0] = 0;
    r.x = 0x3C0;
    r.y = 0x1AB;
    r.w = 8;
    r.h = 0x20;
    pal[14] |= 0x8000;
    pal[15] |= 0x8000;
    func_00074404(&r, D_00089134);
    p = &D_000A7B3C;
    ((unsigned char *)p)[3] = 9;
    p->code = 0x2E;
    p->r0 = 0x80; p->g0 = 0x80; p->b0 = 0x80;
    p->v0 = 0xAC;
    p->v1 = 0xAC;
    p->u0 = 0;
    p->u1 = 0x20;
    p->u2 = 0;
    p->v2 = 0xCC;
    p->u3 = 0x20;
    p->v3 = 0xCC;
    p->clut = func_0007A7F4(0x3C0, 0x1AA);
    p->tpage = GetTPage(0, 0, 0x3C0, 0x100);
    p->x0 = 0x1B0; p->y0 = 0x80;
    p->x1 = 0x1D6; p->y1 = 0x80;
    p->x2 = 0x1B0; p->y2 = 0x95;
    p->x3 = 0x1D6; p->y3 = 0x95;
}
