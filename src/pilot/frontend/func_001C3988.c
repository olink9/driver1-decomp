typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short x1, y1;
    unsigned char u1, v1;
    unsigned short tpage;
    short x2, y2;
    unsigned char u2, v2;
    unsigned short pad1;
    short x3, y3;
    unsigned char u3, v3;
    unsigned short pad2;
} POLY_FT4;
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
typedef struct {
    unsigned char u0, v0, u1, v1, u2, v2, u3, v3;
    unsigned short tpage;
    unsigned short clut;
} TEX;
typedef struct { short x, y, w, h; } RECT;
typedef struct { RECT disp; RECT screen; char pad[4]; } DISPENV;
typedef struct {
    int pad0;
    P_TAG *ot;
    int pad8;
    int id;
    char draw[0x5C];
    DISPENV disp;
} DB;
typedef struct { char pad[0xC]; short x, y; } SCR;
typedef struct { short xmin, ymin, xmax, ymax; } LIM;

extern DB *current, *D_00097524;
extern short D_0009752C;
extern unsigned short D_00097538;
extern int D_001CDEC8;
extern LIM D_001CDFAC[];
extern SCR D_0008AB8C, D_0008AB9C;
extern char D_001C02F8[], D_001C02FC[], D_001C0260[];
extern void func_00051540(), func_000EC550(), func_0007457C(), func_001C334C(), SetDispMask(), func_000514E0();
extern void func_0005C8F4(), func_001C11E4(), DrawSync(), func_001C2DAC(), func_001C3FD0(), func_001C1B4C();
extern int func_0007A7F4();

/* screen position adjust: d-pad moves both display envs within D_001CDFAC[mode] limits, X keeps, triangle restores */
int func_001C3988(void)
{
    POLY_FT4 a[2][2];
    TEX tex;
    POLY_FT4 *p;
    POLY_FT4 *q;
    int i;
    int done = 0;

    func_00051540();
    func_000EC550(0x15, 0xE);
    func_000EC550(0x16, 0xF);
    func_0007457C(current->ot, 0x1080);
    func_0007457C(D_00097524->ot, 0x1080);
    for (i = 0; i < 2; i++) {
        func_001C334C(D_001C02F8, &tex);
        ((P_TAG *)&a[0][i])->len = 9;
        a[0][i].code = 0x2C;
        a[0][i].r0 = 128; a[0][i].g0 = 128; a[0][i].b0 = 128;
        a[0][i].code &= ~2;
        a[0][i].code |= 1;
        a[0][i].x0 = 256; a[0][i].y0 = 0;
        a[0][i].x1 = 512; a[0][i].y1 = 0;
        a[0][i].x2 = 256; a[0][i].y2 = 240;
        a[0][i].x3 = 512; a[0][i].y3 = 240;
        a[0][i].tpage = 24;
        a[0][i].clut = func_0007A7F4(960, tex.clut + 256);
        a[0][i].u0 = tex.u0; a[0][i].v0 = tex.v0; a[0][i].u1 = tex.u1; a[0][i].v1 = tex.v1;
        a[0][i].u2 = tex.u2; a[0][i].v2 = tex.v2; a[0][i].u3 = tex.u3; a[0][i].v3 = tex.v3;
    }
    for (i = 0; i < 2; i++) {
        func_001C334C(D_001C02FC, &tex);
        q = (POLY_FT4 *)(i * sizeof(POLY_FT4) + (int)a);
        p = q + 2;
        p->code = 0x2C;
        ((P_TAG *)p)->len = 9;
        p->r0 = 128; p->g0 = 128; p->b0 = 128;
        p->code &= ~2;
        p->code |= 1;
        p->x0 = 0; p->y0 = 0;
        p->x1 = 256; p->y1 = 0;
        p->x2 = 0; p->y2 = 240;
        p->x3 = 256; p->y3 = 240;
        a[1][i].tpage = 25;
        p->clut = func_0007A7F4(960, tex.clut + 256);
        p->u0 = tex.u0; p->v0 = tex.v0; p->u1 = tex.u1; p->v1 = tex.v1;
        p->u2 = tex.u2; p->v2 = tex.v2; p->u3 = tex.u3; p->v3 = tex.v3;
    }
    SetDispMask(1);
    while (!done) {
        func_000514E0();
        if (D_0009752C & 0x8000) {
            if (current->disp.screen.x > D_001CDFAC[D_001CDEC8].xmin) {
                current->disp.screen.x--;
                D_00097524->disp.screen.x--;
            }
        } else if ((unsigned short)D_0009752C & 0x2000) {
            if (current->disp.screen.x < D_001CDFAC[D_001CDEC8].xmax) {
                current->disp.screen.x++;
                D_00097524->disp.screen.x++;
            }
        } else if ((unsigned short)D_0009752C & 0x1000) {
            if (current->disp.screen.y > D_001CDFAC[D_001CDEC8].ymin) {
                current->disp.screen.y--;
                D_00097524->disp.screen.y--;
            }
        } else if ((unsigned short)D_0009752C & 0x4000) {
            if (current->disp.screen.y < D_001CDFAC[D_001CDEC8].ymax) {
                current->disp.screen.y++;
                D_00097524->disp.screen.y++;
            }
        } else if (D_00097538 & 0x40) {
            D_0008AB8C.x = current->disp.screen.x;
            D_0008AB8C.y = current->disp.screen.y;
            D_0008AB9C.x = current->disp.screen.x;
            D_0008AB9C.y = current->disp.screen.y;
            break;
        } else if (D_00097538 & 0x10) {
            if (D_001CDEC8) {
                current->disp.screen.x = D_0008AB8C.x;
                current->disp.screen.y = D_0008AB8C.y;
                D_00097524->disp.screen.x = D_0008AB8C.x;
                D_00097524->disp.screen.y = D_0008AB8C.y;
            } else {
                current->disp.screen.x = D_0008AB9C.x;
                current->disp.screen.y = D_0008AB9C.y;
                D_00097524->disp.screen.x = D_0008AB9C.x;
                D_00097524->disp.screen.y = D_0008AB9C.y;
            }
            break;
        }
        ((P_TAG *)&a[0][current->id])->addr = current->ot[1].addr;
        current->ot[1].addr = (unsigned)&a[0][current->id];
        ((P_TAG *)&a[1][current->id])->addr = current->ot[1].addr;
        current->ot[1].addr = (unsigned)&a[1][current->id];
        func_0005C8F4(D_001C0260, 120);
        func_001C11E4();
    }
    func_0007457C(current->ot, 0x1080);
    func_0007457C(D_00097524->ot, 0x1080);
    DrawSync(0);
    SetDispMask(0);
    func_001C2DAC(0x280);
    func_001C3FD0(0, 0);
    func_001C1B4C();
    return 0;
}
