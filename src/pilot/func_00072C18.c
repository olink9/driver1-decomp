typedef struct { unsigned char pad[0x818]; unsigned char *tab[1]; } G72C18;
typedef struct {
    unsigned char pad0[0x28];
    unsigned char flags;
    unsigned char lo;
    unsigned char c2A;
    unsigned char c2B;
    unsigned char hi;
    unsigned char pad2D;
    unsigned short x;
    unsigned char i;
    unsigned char pad31;
    unsigned char j;
} S72C18;
extern S72C18 *D_0009781C;
extern G72C18 *D_00097818;

#define EA(b, k) ((b)[(k) * 4 + 0x81] + ((b)[(k) * 4 + 0x82] << 8))
#define EB(b, k) ((b)[(k) * 4 + 0x83] + ((b)[(k) * 4 + 0x84] << 8))

static inline short interp(int b0, int c0, int c1, unsigned short x, short a0v, short a1v)
{
    int r;
    if (a0v == a1v) {
        r = (unsigned)(b0 << 18) >> 16;
    } else {
        short dy = c1 - c0;
        short dx = x - a0v;
        r = c0 + (dx * dy) / (short)(a1v - a0v);
    }
    return r;
}

short func_00072C18(int a0, int flag, int idx)
{
    unsigned char i = D_0009781C->i;
    unsigned char j = D_0009781C->j;
    unsigned short x = D_0009781C->x;
    unsigned char *b = D_00097818->tab[idx];
    int b0 = EB(b, i);
    int c0 = b0 * 4;
    int b1 = EB(b, j);
    int c1 = b1 << 2;
    short a0v = EA(b, i);
    short a1v = EA(b, j);
    short r;

    r = interp(b0, c0, c1, x, a0v, a1v);
    if (!((D_0009781C->flags & 2) && (flag & 0xff) && i == D_0009781C->c2A && x == a0v)) {
        x++;
        if (!((unsigned short)x < a1v)) {
            i = j;
            j++;
            if (D_0009781C->flags & 4) {
                if (D_0009781C->hi < j) {
                    i = D_0009781C->c2B;
                    j = i + 1;
                    x = EA(b, i);
                }
            } else {
                if (!(j < D_0009781C->lo)) {
                    j--;
                    x--;
                }
            }
        }
    }
    *(unsigned short *)&D_0009781C->i = i;
    *(unsigned short *)&D_0009781C->j = j;
    D_0009781C->x = x;
    return r;
}
