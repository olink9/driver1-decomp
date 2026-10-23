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
typedef struct { char *primptr; P_TAG **ot; } DB;
extern DB *current;
extern int func_0007A7F4();

void func_000E7398(int x, int y, int w, int h, int u, int v, int tpage, int clut, unsigned char *rgb)
{
    POLY_FT4 *p;

    p = (POLY_FT4 *)current->primptr;
    ((P_TAG *)p)->len = 9;
    p->code = 0x2C;
    if (rgb[0] == 0xFF) {
        p->r0 = 128;
        p->g0 = 128;
        p->b0 = 128;
    } else {
        p->r0 = rgb[0];
        p->g0 = rgb[1];
        p->b0 = rgb[2];
    }
    p->x0 = x >> 1;
    p->y0 = y;
    p->x1 = (x >> 1) + (w >> 1);
    p->y1 = y;
    p->x2 = x >> 1;
    p->y2 = y + h;
    p->x3 = (x >> 1) + (w >> 1);
    p->y3 = y + h;
    p->u0 = u;
    p->v0 = v;
    p->u1 = u + (w - 1);
    p->v1 = v;
    p->u2 = u;
    p->v2 = v + (h - 1);
    p->u3 = u + (w - 1);
    p->v3 = v + (h - 1);
    p->code &= ~3;
    p->tpage = tpage;
    p->clut = func_0007A7F4(960, clut + 256);
    ((P_TAG *)p)->addr = current->ot[1]->addr;
    current->ot[1]->addr = (unsigned int)p;
    current->primptr += sizeof(POLY_FT4);
}
