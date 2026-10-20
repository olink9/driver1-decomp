extern unsigned char *D_000BEED4[];
extern unsigned char D_00097654[];
extern unsigned char D_0008B6C8[];

void func_0005BF2C(int i, int *out, char *a, char *b)
{
    unsigned char *p;
    unsigned char c;
    unsigned char **pp;
    unsigned u;
    int v;
    int x;
    volatile int t;
    volatile int *q;

    pp = &D_000BEED4[i];
    p = *pp;
    q = &t;
    c = D_00097654[i];
    x = (p[0] << 8) | p[1];
    if (c >= p[2]) {
        *pp = p + 3;
        D_00097654[i] = 0;
    } else {
        D_00097654[i] = c + 1;
    }
    *q = x;
    u = t;
    t = u & 0xA0FC;
    u >>= 8;
    v = u & 0xF;
    if (v == 0) {
        *a = 0;
        *b = 0;
    } else {
        *a = D_0008B6C8[v];
        *b = 2;
    }
    *out = t;
}
