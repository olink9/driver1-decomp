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
typedef struct { short x, y; char *name; short h8; short pad[4]; short h12; } T20;
typedef struct { POLY_FT4 m[3]; } P120;

extern T20 D_001CDCD8[];
extern char *D_001CDE6C[];
extern P120 D_001CF888[];
extern char D_001C0138[];
extern void func_001C334C();
extern int func_0007A7F4();

/* builds the frontend menu sprites: 11 POLY_FT4 icons, 7 overlays and one extra (D_001CF888 = 3 prims per entry) */
void func_001C1270(void)
{
    TEX tex;
    POLY_FT4 *p;
    POLY_FT4 *q;
    int i;

    for (i = 0; i < 7; i++) {
        func_001C334C(D_001CDCD8[i + 1].name, &tex);
        ((P_TAG *)&D_001CF888[i].m[0])->len = 9;
        D_001CF888[i].m[0].code = 0x2C;
        D_001CF888[i].m[0].r0 = 128; D_001CF888[i].m[0].g0 = 128; D_001CF888[i].m[0].b0 = 128;
        D_001CF888[i].m[0].code |= 2;
        D_001CF888[i].m[0].code &= ~1;
        D_001CF888[i].m[0].x0 = D_001CDCD8[i + 1].x;
        D_001CF888[i].m[0].y0 = D_001CDCD8[i + 1].y;
        D_001CF888[i].m[0].x1 = D_001CDCD8[i + 1].x + (tex.u1 - tex.u0 + 1);
        D_001CF888[i].m[0].y1 = D_001CDCD8[i + 1].y;
        D_001CF888[i].m[0].x2 = D_001CDCD8[i + 1].x;
        D_001CF888[i].m[0].y2 = D_001CDCD8[i + 1].y + (tex.v2 - tex.v0 + 1);
        D_001CF888[i].m[0].x3 = D_001CDCD8[i + 1].x + (tex.u1 - tex.u0 + 1);
        D_001CF888[i].m[0].y3 = D_001CDCD8[i + 1].y + (tex.v2 - tex.v0 + 1);
        D_001CF888[i].m[0].tpage = tex.tpage;
        D_001CF888[i].m[0].clut = func_0007A7F4(960, tex.clut + 256);
        D_001CF888[i].m[0].u0 = tex.u0; D_001CF888[i].m[0].v0 = tex.v0; D_001CF888[i].m[0].u1 = tex.u1; D_001CF888[i].m[0].v1 = tex.v1;
        D_001CF888[i].m[0].u2 = tex.u2; D_001CF888[i].m[0].v2 = tex.v2; D_001CF888[i].m[0].u3 = tex.u3; D_001CF888[i].m[0].v3 = tex.v3;
    }
    for (i = 7; i < 11; i++) {
        func_001C334C(D_001CDCD8[i + 1].name, &tex);
        ((P_TAG *)&D_001CF888[i].m[0])->len = 9;
        D_001CF888[i].m[0].code = 0x2C;
        D_001CF888[i].m[0].r0 = 128; D_001CF888[i].m[0].g0 = 128; D_001CF888[i].m[0].b0 = 128;
        D_001CF888[i].m[0].code &= ~2;
        D_001CF888[i].m[0].code |= 1;
        D_001CF888[i].m[0].x0 = D_001CDCD8[i + 1].x;
        D_001CF888[i].m[0].y0 = D_001CDCD8[i + 1].y;
        D_001CF888[i].m[0].x1 = D_001CDCD8[i + 1].x + (tex.u1 - tex.u0 + 1);
        D_001CF888[i].m[0].y1 = D_001CDCD8[i + 1].y;
        D_001CF888[i].m[0].x2 = D_001CDCD8[i + 1].x;
        D_001CF888[i].m[0].y2 = D_001CDCD8[i + 1].y + (tex.v2 - tex.v0 + 1);
        D_001CF888[i].m[0].x3 = D_001CDCD8[i + 1].x + (tex.u1 - tex.u0 + 1);
        D_001CF888[i].m[0].y3 = D_001CDCD8[i + 1].y + (tex.v2 - tex.v0 + 1);
        D_001CF888[i].m[0].tpage = tex.tpage;
        D_001CF888[i].m[0].clut = func_0007A7F4(960, tex.clut + 256);
        D_001CF888[i].m[0].u0 = tex.u0; D_001CF888[i].m[0].v0 = tex.v0; D_001CF888[i].m[0].u1 = tex.u1; D_001CF888[i].m[0].v1 = tex.v1;
        D_001CF888[i].m[0].u2 = tex.u2; D_001CF888[i].m[0].v2 = tex.v2; D_001CF888[i].m[0].u3 = tex.u3; D_001CF888[i].m[0].v3 = tex.v3;
    }
    for (i = 0; i < 7; i++) {
        func_001C334C(D_001CDE6C[i], &tex);
        p = &D_001CF888[i].m[1];
        ((P_TAG *)p)->len = 9;
        p->code = 0x2C;
        p->r0 = 128; p->g0 = 128; p->b0 = 128;
        p->code |= 2;
        p->code &= ~1;
        p->x0 = D_001CDCD8[i + 1].x;
        p->y0 = D_001CDCD8[i + 1].y;
        p->x1 = D_001CDCD8[i + 1].x + (tex.u1 - tex.u0 + 1);
        p->y1 = D_001CDCD8[i + 1].y;
        p->x2 = D_001CDCD8[i + 1].x;
        p->y2 = D_001CDCD8[i + 1].y + (tex.v2 - tex.v0 + 1);
        p->x3 = D_001CDCD8[i + 1].x + (tex.u1 - tex.u0 + 1);
        p->y3 = D_001CDCD8[i + 1].y + (tex.v2 - tex.v0 + 1);
        D_001CF888[i].m[1].tpage = tex.tpage;
        p->clut = func_0007A7F4(960, tex.clut + 256);
        p->u0 = tex.u0; p->v0 = tex.v0; p->u1 = tex.u1; p->v1 = tex.v1;
        p->u2 = tex.u2; p->v2 = tex.v2; p->u3 = tex.u3; p->v3 = tex.v3;
    }
    func_001C334C(D_001C0138, &tex);
    q = &D_001CF888[6].m[2];
    ((P_TAG *)q)->len = 9;
    q->code = 0x2C;
    q->r0 = 128; q->g0 = 128; q->b0 = 128;
    q->code |= 2;
    q->code &= ~1;
    q->x0 = D_001CDCD8[7].x;
    q->y0 = D_001CDCD8[7].y;
    q->x1 = D_001CDCD8[7].x + (tex.u1 - tex.u0 + 1);
    q->y1 = D_001CDCD8[7].y;
    q->x2 = D_001CDCD8[7].x;
    q->y2 = D_001CDCD8[7].y + (tex.v2 - tex.v0 + 1);
    q->x3 = D_001CDCD8[7].x + (tex.u1 - tex.u0 + 1);
    q->y3 = D_001CDCD8[7].y + (tex.v2 - tex.v0 + 1);
    q->tpage = tex.tpage;
    q->clut = func_0007A7F4(960, tex.clut + 256);
    q->u0 = tex.u0; q->v0 = tex.v0; q->u1 = tex.u1; q->v1 = tex.v1;
    q->u2 = tex.u2; q->v2 = tex.v2; q->u3 = tex.u3; q->v3 = tex.v3;
}
