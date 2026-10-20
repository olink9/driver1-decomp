typedef struct { unsigned char b[4]; short s[8]; } R20;
typedef struct { short p0, idx; char p1[12]; } A16;
typedef struct { char p0[10]; short a; char p1[14]; short b; char p2[16]; } B44;
extern int D_000967A8;
extern A16 *D_00097044;
extern B44 *D_00097040;
extern R20 D_0009E744[];
extern void func_000213C8();

static inline unsigned char side(B44 *o, short n)
{
    if (o->a == n) return 0;
    if (o->b != n) return 1;
    return 0;
}

void func_00021F5C(void)
{
    int i;
    R20 *r = D_0009E744;
    for (i = 0; i < D_000967A8; i++, r++) {
        unsigned char f;
        int v[4];
        short n = i;
        B44 *o = (B44 *)(D_00097044[n].idx * sizeof(B44) + (int)D_00097040);
        f = side(o, n);
        func_000213C8(i, 0, v);
        if (f == 0) r->b[0] = v[2] - v[0] + 1; else r->b[0] = 0;
        r->s[0] = v[0]; r->s[1] = -v[3];
        func_000213C8(i, 1, v);
        if (f == 1) r->b[1] = v[3] - v[1] + 1; else r->b[1] = 0;
        r->s[2] = v[0]; r->s[3] = -v[1];
        func_000213C8(i, 2, v);
        if (f == 0) r->b[2] = v[2] - v[0] + 1; else r->b[2] = 0;
        r->s[4] = v[2]; r->s[5] = -v[1];
        func_000213C8(i, 3, v);
        if (f == 1) r->b[3] = v[3] - v[1] + 1; else r->b[3] = 0;
        r->s[6] = v[2]; r->s[7] = -v[3];
    }
}
