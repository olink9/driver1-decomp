struct E { short t; unsigned char a, b; };
extern struct E *D_00096DC8;
extern int D_00096DCC;
extern int *D_00097668;
extern long long D_000970D8;
int func_00058A00(int unused, int *out) {
    struct E *e = &D_00096DC8[D_00096DCC];
    int v, r;
    if (D_00096DC8 != 0) {
        if ((unsigned)D_00096DCC < 0x190) {
            int t = *(int *)&D_000970D8;
            if (t + 20 < *D_00097668) {
                if ((t & 0xFFFF) >= e->t) {
                    v = e->a;
                    r = e->b;
                    *out = v;
                    D_00096DCC = D_00096DCC + 1;
                    return r;
                }
            }
        }
        return 0xFF;
    }
    return 0xFF;
}
