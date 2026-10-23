/* FAKEMATCH: unused pad[32] only sizes the frame (0x38); probably an unused local aggregate in the original. */
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
typedef struct {
    unsigned tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short w, h;
} SPRT;
typedef struct {
    unsigned tag;
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
} POLY_FT3;
typedef struct {
    unsigned char u0, v0, u1, v1, u2, v2, u3, v3;
    unsigned short tpage;
    unsigned short clut;
    unsigned char idx, slot;
} TEXINF;
typedef struct { char *primptr; P_TAG **ot; } DB;
extern DB *current;
extern unsigned short GetTPage();
#define addPrim(ot, p) (((P_TAG *)(p))->addr = ((P_TAG *)(ot))->addr, ((P_TAG *)(ot))->addr = (unsigned)(p))

void func_001CA408(int x, int y, TEXINF *tex, unsigned char r, unsigned char g, unsigned char b, int unused, unsigned char mode)
{
    SPRT *sp;
    POLY_FT3 *tp;
    char pad[32]; /* FAKEMATCH: frame size */

    sp = (SPRT *)current->primptr;
    ((P_TAG *)sp)->len = 4;
    sp->code = 0x64;
    sp->r0 = r;
    sp->g0 = g;
    sp->b0 = b;
    sp->x0 = x;
    sp->y0 = y;
    sp->code |= 2;
    sp->code &= ~1;
    sp->u0 = tex->u0;
    sp->v0 = tex->v0;
    sp->w = tex->u1 - tex->u0 + 1;
    sp->h = tex->v2 - tex->v0 + 1;
    sp->clut = tex->clut;
    addPrim(current->ot[1], sp);
    current->primptr += sizeof(SPRT);

    tp = (POLY_FT3 *)current->primptr;
    ((P_TAG *)tp)->len = 7;
    tp->code = 0x24;
    tp->x0 = -1;
    tp->y0 = -1;
    tp->x1 = -1;
    tp->y1 = -1;
    tp->x2 = -1;
    tp->y2 = -1;
    if (mode == 3) {
        tp->code |= 2;
        tp->tpage = GetTPage(0, 0, 0x240, 0);
    } else {
        tp->code |= 2;
        tp->tpage = tex->tpage;
    }
    addPrim(current->ot, tp);
    current->primptr += sizeof(POLY_FT3);
}
