struct E { short a; unsigned short b; int c[2]; short *d; int e; };
extern int D_000972BC;
extern int D_000972A4;
extern short *D_00097258;
extern long long D_0009725C;
extern short *D_0009E724[];
extern struct E D_000A7EA4[];

void func_0003542C(void) {
    short *out, *src, *t;
    int n, c, v;
    int idx = D_000972BC;
    D_000972A4 = 0;
    t = D_0009E724[D_000A7EA4[idx].b];
    src = *(short **)&D_0009725C = D_000A7EA4[idx].d;
    out = D_00097258 = t;
    n = 0xE10;
    if (*(int *)src == -1) {
        do {
            *out++ = -1;
        } while (--n);
    } else {
        do {
            c = src[0];
            v = src[1];
            src += 2;
            n -= c;
            for (; c > 0; c--) *out++ = v;
        } while (n > 0);
    }
}
