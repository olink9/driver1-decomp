typedef struct { int a; int b; int pad[3]; int d; } T58278;
extern T58278 D_0008B080[];
extern long long D_00096834;

void func_00058278(T58278 *t)
{
    int *p = (int *)t;
    int x = p[0];
    int m = *(int *)&D_00096834;

    if (m == 0 || m == 5) {
        t = &D_0008B080[m];
        p[0] = t->a + (p[1] / t->d + 0x31);
        p[1] = t->b + (x / t->d + 0x31);
    } else if (m == 1) {
        t = &D_0008B080[1];
        p[1] = p[1] / t->d + 0x83;
        p[0] = 0xCC - p[0] / t->d;
    } else {
        t = &D_0008B080[m];
        p[1] = -0x1C - p[1] / t->d;
        p[0] = p[0] / t->d + 0x118;
    }
}
