typedef struct { unsigned char lo, hi, u, v; unsigned short tpage, clut; } RNG;
typedef struct {
    RNG r[2];
    unsigned short n;
    short val;
    unsigned char col[256][4];
} FONT;
extern FONT D_000ED8B0[];
extern unsigned char D_000ED530;
extern unsigned char D_000ED531;
extern short D_000ED532;
extern short D_000ED534;
extern short D_000ED536;
extern unsigned char D_000ED538[];
extern void func_000E7398();

/* print a string with the current font at (D_000ED532, D_000ED534); flags 1/2 = right/centre aligned */
void func_000E6F98(unsigned char *s)
{
    unsigned char *p;
    unsigned char *cp;
    unsigned char c;
    int w;
    short yoff;
    RNG *r;

    w = 0;
    if (D_000ED530 & 1) {
        for (p = s; (c = *p++) != 0;) {
            if (c < 0x21)
                w += D_000ED8B0[D_000ED531].col[32][2];
            else {
                cp = D_000ED8B0[D_000ED531].col[c];
                w += cp[2] + D_000ED8B0[D_000ED531].val;
            }
        }
        D_000ED532 -= w;
    } else if (D_000ED530 & 2) {
        for (p = s; (c = *p++) != 0;) {
            if (c < 0x21)
                w += D_000ED8B0[D_000ED531].col[32][2];
            else {
                cp = D_000ED8B0[D_000ED531].col[c];
                w += cp[2] + D_000ED8B0[D_000ED531].val;
            }
        }
        D_000ED532 -= w >> 1;
    }
    while ((c = *s++) != 0) {
        if (c < 0x21) {
            if (c == '\n') {
                D_000ED534 += D_000ED8B0[D_000ED531].col[32][3] + 1;
                D_000ED532 = D_000ED536;
            } else
                D_000ED532 += D_000ED8B0[D_000ED531].col[32][2];
        } else {
            cp = D_000ED8B0[D_000ED531].col[c];
            yoff = D_000ED8B0[D_000ED531].n - cp[3];
            for (r = D_000ED8B0[D_000ED531].r; r < &D_000ED8B0[D_000ED531].r[2]; r++) {
                if (c >= r->lo && c <= r->hi)
                    break;
            }
            func_000E7398(D_000ED532, D_000ED534 + yoff, cp[2], cp[3], r->u + cp[0], r->v + cp[1],
                          r->tpage, r->clut, D_000ED538);
            D_000ED532 += cp[2] + D_000ED8B0[D_000ED531].val;
        }
    }
}
