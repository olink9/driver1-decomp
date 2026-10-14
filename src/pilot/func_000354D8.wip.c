extern int D_000972BC, D_000972A8, D_000972C8;
extern long long D_000972CC;
struct S { short pad; unsigned short h; int pad2[3]; int *p; };
extern struct S D_000A7EA4[];
extern int *D_0009D8F4[];
void func_000354D8(void) {
    int *dst;
    int *src;
    int left;
    int n, v;
    src = D_000A7EA4[D_000972BC].p;
    dst = D_0009D8F4[D_000A7EA4[D_000972BC].h];
    D_000972A8 = 0;
    *(int *)&D_000972CC = (int)src;
    D_000972C8 = (int)dst;
    left = 0xE10;
    do {
        n = src[0];
        v = src[1];
        src += 2;
        left -= n;
        if (n) {
            do {
                *dst++ = v;
            } while (--n);
        }
    } while (left);
}
