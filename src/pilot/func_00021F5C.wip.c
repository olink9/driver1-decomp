typedef struct { unsigned char b[4]; short s[8]; } R20;
typedef struct { short p0, idx; char p1[12]; } A16;
typedef struct { char p0[10]; short a; char p1[14]; short b; char p2[16]; } B44;
extern int D_000967A8;
extern A16 *D_00097044;
extern B44 *D_00097040;
extern R20 D_0009E744[];
extern void func_000213C8();

void func_00021F5C(void)
{
    int i;
    for (i = 0; i < D_000967A8; i++) {
        unsigned char f;
        int v[4];
        R20 *r = &D_0009E744[i];
        short *w = &w[0];
        short n = i;
        B44 *o = &D_00097040[D_00097044[n].idx];
        f = 0;
        if (o->a != n) { f = 1; if (o->b != n) f = 0; }
        func_000213C8(i, 0, v);
        if (f == 0) r->b[0] = (unsigned char)v[2] - (unsigned char)v[0] + 1; else r->b[0] = 0;
        w[-7] = v[0]; w[-6] = -v[3];
        func_000213C8(i, 1, v);
        if (f == 1) ((unsigned char *)w)[-17] = (unsigned char)v[3] - (unsigned char)v[1] + 1; else ((unsigned char *)w)[-17] = 0;
        w[-5] = v[0]; w[-4] = -v[1];
        func_000213C8(i, 2, v);
        if (f == 0) ((unsigned char *)w)[-16] = (unsigned char)v[2] - (unsigned char)v[0] + 1; else ((unsigned char *)w)[-16] = 0;
        w[-3] = v[2]; w[-2] = -v[1];
        func_000213C8(i, 3, v);
        if (f == 1) ((unsigned char *)w)[-15] = (unsigned char)v[3] - (unsigned char)v[1] + 1; else ((unsigned char *)w)[-15] = 0;
        w[-1] = v[2]; w[0] = -v[3];
    }
}
