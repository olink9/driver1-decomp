extern int D_0009733C;
extern int D_00097350;
extern long long D_00097098;
extern long long D_000970E0;
extern long long D_00097108;
extern long long D_000970D4;
extern long long D_000AA7E4;
extern int D_000AB050[];
#define P(x) (*(short **)&(x))
#define I(x) (*(int *)&(x))

void func_00039748(void) {
    int n, i, a, b;
    short *p;
    n = D_0009733C * D_0009733C;
    p = P(D_00097098);
    P(D_000970E0) = p;
    for (i = n * 4 - 1; i > 0; i--) ((unsigned short *)p)[i] = 0xFFFF;
    a = (D_000AB050[0] + D_00097350) << 3;
    b = I(D_000AA7E4) << 1;
    P(D_00097108) = p = p + D_0009733C * D_0009733C * 4;
    P(D_000970D4) = p = p + a;
    P(D_00097098) = p += b;
}
