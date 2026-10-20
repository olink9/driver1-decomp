struct E { short a; unsigned short b; int c[3]; int *d; };
extern int D_000972BC;
extern int D_000972A8;
extern int *D_000972C8;
extern long long D_000972CC;
extern int *D_0009D8F4[];
extern struct E D_000A7EA4[];

void func_000354D8(void) {
    int *out, *src, *t;
    int n, c, v;
    int idx = D_000972BC;
    D_000972A8 = 0;
    t = D_0009D8F4[D_000A7EA4[idx].b];
    src = *(int **)&D_000972CC = D_000A7EA4[idx].d;
    out = D_000972C8 = t;
    n = 0xE10;
    do {
        c = src[0];
        v = src[1];
        src += 2;
        n -= c;
        for (; c != 0; c--) *out++ = v;
    } while (n != 0);
}
