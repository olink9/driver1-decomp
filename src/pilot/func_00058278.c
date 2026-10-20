static inline int sc(int v, int d) { return v / d + 0x31; }
typedef struct { int a; int b; int pad[3]; int d; } T58278;
extern T58278 D_0008B080[];
extern long long D_00096834;

void func_00058278(int *p)
{
    int x = p[0];
    int m = *(int *)&D_00096834;

    if (m == 0 || m == 5) {
        T58278 *base = D_0008B080;
        T58278 *t = &base[m];
        p[0] = t->a + sc(p[1], t->d);
        p[1] = t->b + sc(x, t->d);
    } else if (m == 1) {
        T58278 *base = D_0008B080;
        T58278 *t = &base[m];
        p[1] = p[1] / t->d + 0x83;
        p[0] = 0xCC - p[0] / t->d;
    } else {
        T58278 *base = D_0008B080;
        T58278 *t = &base[m];
        p[1] = -0x1C - p[1] / t->d;
        p[0] = p[0] / t->d + 0x118;
    }
}
