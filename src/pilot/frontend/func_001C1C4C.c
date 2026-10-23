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

extern POLY_FT4 D_001D2C70[], D_001D2CC0[];
extern char D_001C015C[], D_001C0168[];
extern void func_001C334C();
extern int func_0007A7F4();

void func_001C1C4C(void)
{
    TEX tex;
    POLY_FT4 *p, *q, *r, *t;

    func_001C334C(D_001C015C, &tex);
    p = &D_001D2C70[0];
    q = &D_001D2C70[1];
    ((P_TAG *)p)->len = 9;
    p->code = 0x2C;
    ((P_TAG *)q)->len = 9;
    q->code = 0x2C;
    p->r0 = 128; p->g0 = 128; p->b0 = 128;
    q->r0 = 128; q->g0 = 128; q->b0 = 128;
    p->code |= 2;
    q->code |= 2;
    p->code &= ~1;
    q->code &= ~1;
    p->x0 = 0; p->y0 = 0; p->x1 = tex.u1 - tex.u0 + 1; p->y1 = 0;
    p->x2 = 0; p->y2 = tex.v2 - tex.v0 + 1; p->x3 = tex.u1 - tex.u0 + 1; p->y3 = tex.v2 - tex.v0 + 1;
    q->x0 = 0; q->y0 = 0; q->x1 = tex.u1 - tex.u0 + 1; q->y1 = 0;
    q->x2 = 0; q->y2 = tex.v2 - tex.v0 + 1; q->x3 = tex.u1 - tex.u0 + 1; q->y3 = tex.v2 - tex.v0 + 1;
    p->tpage = tex.tpage;
    D_001D2C70[1].tpage = tex.tpage;
    p->clut = func_0007A7F4(960, tex.clut + 256);
    q->clut = func_0007A7F4(960, tex.clut + 256);
    p->u0 = tex.u0; p->v0 = tex.v0; p->u1 = tex.u1; p->v1 = tex.v1;
    p->u2 = tex.u2; p->v2 = tex.v2; p->u3 = tex.u3; p->v3 = tex.v3;
    q->u0 = tex.u0; q->v0 = tex.v0; q->u1 = tex.u1; q->v1 = tex.v1;
    q->u2 = tex.u2; q->v2 = tex.v2; q->u3 = tex.u3; q->v3 = tex.v3;

    func_001C334C(D_001C0168, &tex);
    r = &D_001D2CC0[0];
    t = &D_001D2CC0[1];
    ((P_TAG *)r)->len = 9;
    r->code = 0x2C;
    ((P_TAG *)t)->len = 9;
    t->code = 0x2C;
    r->r0 = 128; r->g0 = 128; r->b0 = 128;
    t->r0 = 128; t->g0 = 128; t->b0 = 128;
    r->code |= 2;
    t->code |= 2;
    r->code &= ~1;
    t->code &= ~1;
    r->x0 = 0; r->y0 = 0; r->x1 = tex.u1 - tex.u0 + 1; r->y1 = 0;
    r->x2 = 0; r->y2 = tex.v2 - tex.v0 + 1; r->x3 = tex.u1 - tex.u0 + 1; r->y3 = tex.v2 - tex.v0 + 1;
    t->x0 = 0; t->y0 = 0; t->x1 = tex.u1 - tex.u0 + 1; t->y1 = 0;
    t->x2 = 0; t->y2 = tex.v2 - tex.v0 + 1; t->x3 = tex.u1 - tex.u0 + 1; t->y3 = tex.v2 - tex.v0 + 1;
    r->tpage = tex.tpage;
    D_001D2CC0[1].tpage = tex.tpage;
    r->clut = func_0007A7F4(960, tex.clut + 256);
    t->clut = func_0007A7F4(960, tex.clut + 256);
    r->u0 = tex.u0; r->v0 = tex.v0; r->u1 = tex.u1; r->v1 = tex.v1;
    r->u2 = tex.u2; r->v2 = tex.v2; r->u3 = tex.u3; r->v3 = tex.v3;
    t->u0 = tex.u0; t->v0 = tex.v0; t->u1 = tex.u1; t->v1 = tex.v1;
    t->u2 = tex.u2; t->v2 = tex.v2; t->u3 = tex.u3; t->v3 = tex.v3;
}
