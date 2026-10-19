typedef struct { unsigned char u0, v0, u1, v1, u2, v2, u3, v3; unsigned short tpage, clut; } UVS;
typedef struct {
    unsigned long tag; unsigned char r0, g0, b0, code;
    short x0, y0; unsigned char u0, v0; unsigned short clut;
    short x1, y1; unsigned char u1, v1; unsigned short tpage;
    short x2, y2; unsigned char u2, v2; unsigned short pad2;
    short x3, y3; unsigned char u3, v3; unsigned short pad3;
} PF4;
extern short D_0008EAA8[];
extern long long D_00097530;
extern long long D_00097534;
extern UVS D_000B9170;
extern void DrawPrim();

void func_0005813C(unsigned char *col, int *pos)
{
    int x, y, d;
    PF4 *p;
    UVS *uv;
    PF4 **pp = (PF4 **)*(int *)&D_00097530;
    x = pos[0];
    y = pos[1];
    d = (D_0008EAA8[(*(int *)&D_00097534 << 9) & 0x1E00] + 0x1388) >> 11;
    p = *pp;
    ((unsigned char *)p)[3] = 9;
    p->code = 0x2C;
    p->r0 = col[0];
    p->g0 = col[1];
    p->b0 = col[2];
    p->x0 = x - d; p->y0 = y - d;
    p->x1 = x + d; p->y1 = y - d;
    p->x2 = x - d; p->y2 = y + d;
    p->x3 = x + d; p->y3 = y + d;
    uv = &D_000B9170;
    p->u0 = uv->u0; p->v0 = uv->v0;
    p->u1 = uv->u1; p->v1 = uv->v1;
    p->u2 = uv->u2; p->v2 = uv->v2;
    p->u3 = uv->u3; p->v3 = uv->v3;
    p->tpage = uv->tpage | 0x20;
    p->clut = uv->clut;
    p->code |= 2;
    DrawPrim(p);
}
