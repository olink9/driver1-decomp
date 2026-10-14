extern void func_000539C8();
extern void func_0005717C();

void func_000573AC(int a0, unsigned n, int c, int d, int e, int g) {
    char *buf = __builtin_alloca((n << 7) >> 3);
    unsigned char d1 = d;
    unsigned char e1 = e;
    unsigned short f1 = g;
    func_000539C8(a0, buf, n, 0, 0);
    func_0005717C(buf, n, c, d1, e1, f1);
}
