/* func_001C5D28: twin of func_001C4810 (2 sprites). setSemiTrans(p,0); setShadeTex(p,0) as two separate &= after rgb. */
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
typedef struct { unsigned addr:24; unsigned len:8; unsigned char r0, g0, b0, code; } P_TAG2;
typedef struct {
    unsigned char u0, v0, u1, v1, u2, v2, u3, v3;
    unsigned short tpage;
    unsigned short clut;
} TEX;
typedef struct { short y, pad; } YPOS;

extern POLY_FT4 D_001D2DD0[];
extern POLY_FT4 *D_001D2E20;
extern int D_001CE4BC[];
extern YPOS D_001CE4C4[];
extern void func_001C334C();
extern int func_0007A7F4();

void func_001C5D28(void)
{
    TEX tex;
    POLY_FT4 *p;
    int i;

    for (i = 0; i < 2; i++) {
        func_001C334C(D_001CE4BC[i], &tex);
        p = &D_001D2DD0[i];
        ((P_TAG *)p)->len = 9;
        p->code = 0x2C;
        p->r0 = 128;
        p->g0 = 128;
        p->b0 = 128;
        p->code &= ~2;
        p->code &= ~1;
        p->x0 = 160;
        p->y0 = D_001CE4C4[i].y;
        p->x1 = tex.u1 - tex.u0 + 161;
        p->y1 = D_001CE4C4[i].y;
        p->x2 = 160;
        p->y2 = D_001CE4C4[i].y + (tex.v2 - tex.v0 + 1);
        p->x3 = tex.u1 - tex.u0 + 161;
        p->y3 = D_001CE4C4[i].y + (tex.v2 - tex.v0 + 1);
        p->tpage = tex.tpage;
        p->clut = func_0007A7F4(960, tex.clut + 256);
        p->u0 = tex.u0;
        p->v0 = tex.v0;
        p->u1 = tex.u1;
        p->v1 = tex.v1;
        p->u2 = tex.u2;
        p->v2 = tex.v2;
        p->u3 = tex.u3;
        p->v3 = tex.v3;
    }
    D_001D2E20 = D_001D2DD0;
}
