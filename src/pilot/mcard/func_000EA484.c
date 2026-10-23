typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
typedef struct {
    P_TAG tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0; unsigned short clut;
    short x1, y1;
    unsigned char u1, v1; unsigned short tpage;
    short x2, y2;
    unsigned char u2, v2; unsigned short pad1;
    short x3, y3;
    unsigned char u3, v3; unsigned short pad2;
} POLY_FT4;
typedef struct {
    P_TAG tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    short x1, y1;
    short x2, y2;
    short x3, y3;
} POLY_F4;
typedef struct {
    unsigned char u0, v0, u1, v1, u2, v2, u3, v3;
    unsigned short tpage;
    unsigned short clut;
    unsigned char idx, slot;
} OUT;
extern POLY_FT4 D_000EF150[2][15];
extern POLY_F4 D_000EF600[2][15];
extern POLY_FT4 D_000EF8D0[4];
extern POLY_F4 D_000EF970[2];
extern POLY_F4 D_000EF9C0[2];
extern char D_000E6BAC[];
extern char D_000E6BB4[];
extern int GetTPage();
extern int func_0007A7F4(); /* GetClut */
extern void func_000EC6BC();

#define setlen(p, _len) (((P_TAG *)(p))->len = (_len))
#define setcode(p, _code) ((p)->code = (_code))
#define setPolyFT4(p) setlen(p, 9), setcode(p, 0x2C)
#define setPolyF4(p) setlen(p, 5), setcode(p, 0x28)
#define setRGB0(p, _r0, _g0, _b0) (p)->r0 = (_r0), (p)->g0 = (_g0), (p)->b0 = (_b0)
#define setSemiTrans(p, abe) ((abe) ? setcode(p, (p)->code | 2) : setcode(p, (p)->code & ~2))
#define setXY4(p, _x0, _y0, _x1, _y1, _x2, _y2, _x3, _y3) \
    (p)->x0 = (_x0), (p)->y0 = (_y0), (p)->x1 = (_x1), (p)->y1 = (_y1), \
    (p)->x2 = (_x2), (p)->y2 = (_y2), (p)->x3 = (_x3), (p)->y3 = (_y3)
#define setXYWH(p, _x0, _y0, _w, _h) \
    (p)->x0 = (_x0), (p)->y0 = (_y0), (p)->x1 = (_x0) + (_w), (p)->y1 = (_y0), \
    (p)->x2 = (_x0), (p)->y2 = (_y0) + (_h), (p)->x3 = (_x0) + (_w), (p)->y3 = (_y0) + (_h)
#define setUV4(p, _u0, _v0, _u1, _v1, _u2, _v2, _u3, _v3) \
    (p)->u0 = (_u0), (p)->v0 = (_v0), (p)->u1 = (_u1), (p)->v1 = (_v1), \
    (p)->u2 = (_u2), (p)->v2 = (_v2), (p)->u3 = (_u3), (p)->v3 = (_v3)

/* build the memory card screen prims: 2 x 15 slot icons + boxes, card frames, banners */
void func_000EA484(void)
{
    OUT e;
    int i;
    int j;
    int r;
    int c;
    int x;
    int y;
    int xs;
    int xb;
    int top;    /* top of the slot grid */
    int by;     /* top of the card frame */
    int cy;

    top = 0x5A;
    by = 0x5A;
    for (i = 0; i < 2; i++) {
        if (i == 0) {
            xs = 0x38;
            xb = 0x20;
        } else {
            xs = 0x158;
            xb = 0x140;
        }
        for (j = 0; j < 15; j++) {
            c = j % 3;
            r = j / 3;
            setPolyFT4(&D_000EF150[i][j]);
            setRGB0(&D_000EF150[i][j], 128, 128, 128);
            D_000EF150[i][j].tpage = GetTPage(0, 0, 0x380, 0x100);
            x = xs + c * 32;
            y = top + r * 20;
            setUV4(&D_000EF150[i][j], j * 16, i * 16, j * 16 + 15, i * 16, j * 16, i * 16 + 15, j * 16 + 15, i * 16 + 15);
            setXY4(&D_000EF150[i][j], x, y, x + 25, y, x, y + 16, x + 25, y + 16);
            cy = j + 0x160;
            D_000EF150[i][j].clut = func_0007A7F4(0x390, (i << 4) + cy);
            setPolyF4(&D_000EF600[i][j]);
            setRGB0(&D_000EF600[i][j], 0x40, 0x40, 0xFF);
            setXY4(&D_000EF600[i][j], x, y, x + 25, y, x, y + 16, x + 25, y + 16);
        }
        setPolyF4(&D_000EF9C0[i]);
        setRGB0(&D_000EF9C0[i], 0, 0, 0);
        setXYWH(&D_000EF9C0[i], xb, by, 0xA6, 0x28);
        D_000EF9C0[i].code = (D_000EF9C0[i].code | 2) & ~1;
    }

    func_000EC6BC(D_000E6BAC, &e);
    {
        POLY_FT4 *p = &D_000EF8D0[0];
        setPolyFT4(p);
        setRGB0(p, 128, 128, 128);
        setSemiTrans(p, 0);
        setXYWH(p, 0, 0, 0x100, e.v2 - e.v0 + 1);
        D_000EF8D0[0].tpage = e.tpage;
        p->clut = func_0007A7F4(0x3B0, 0x1FF);
        setUV4(p, e.u0, e.v0, e.u1, e.v1, e.u2, e.v2, e.u3, e.v3);
    }
    func_000EC6BC(D_000E6BAC, &e);
    {
        POLY_FT4 *p = &D_000EF8D0[1];
        setPolyFT4(p);
        setRGB0(p, 128, 128, 128);
        setSemiTrans(p, 0);
        setXYWH(p, 0, 0, 0x100, e.v2 - e.v0 + 1);
        D_000EF8D0[1].tpage = e.tpage;
        p->clut = func_0007A7F4(0x3B0, 0x1FF);
        setUV4(p, e.u0, e.v0, e.u1, e.v1, e.u2, e.v2, e.u3, e.v3);
    }
    func_000EC6BC(D_000E6BB4, &e);
    {
        POLY_FT4 *p = &D_000EF8D0[2];
        setPolyFT4(p);
        setRGB0(p, 128, 128, 128);
        setSemiTrans(p, 0);
        setXYWH(p, 0x100, 0, 0x100, e.v2 - e.v0 + 1);
        D_000EF8D0[2].tpage = e.tpage;
        p->clut = func_0007A7F4(0x3A0, 0x1FF);
        setUV4(p, e.u0, e.v0, e.u1, e.v1, e.u2, e.v2, e.u3, e.v3);
    }
    func_000EC6BC(D_000E6BB4, &e);
    {
        POLY_FT4 *p = &D_000EF8D0[3];
        setPolyFT4(p);
        setRGB0(p, 128, 128, 128);
        setSemiTrans(p, 0);
        setXYWH(p, 0x100, 0, 0x100, e.v2 - e.v0 + 1);
        D_000EF8D0[3].tpage = e.tpage;
        p->clut = func_0007A7F4(0x3A0, 0x1FF);
        setPolyF4(&D_000EF970[0]);
        setRGB0(&D_000EF970[0], 0, 0, 0);
        setSemiTrans(&D_000EF970[0], 0);
        setXYWH(&D_000EF970[0], 0, 0, 0x200, 12);
        setUV4(p, e.u0, e.v0, e.u1, e.v1, e.u2, e.v2, e.u3, e.v3);
        setPolyF4(&D_000EF970[1]);
        setRGB0(&D_000EF970[1], 0, 0, 0);
        setSemiTrans(&D_000EF970[1], 0);
        setXYWH(&D_000EF970[1], 0, 0xDC, 0x200, 0x24);
    }
}
