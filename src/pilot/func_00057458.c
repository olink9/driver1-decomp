typedef struct { int a,b,c,d; } V;
typedef struct { int pad[3]; int flags; V *p; int pad2[6]; } E; /* 0x2C */
extern long long D_00096D1C;
extern void func_000573AC();
void func_00057458(char *base) {
    V buf[6];
    V *out = buf;
    int n = 0;
    E *e;
    for (e = (E *)(base + 0x130); (char *)e < base + 0x238; e++) {
        if ((e->flags & 0x11) == 1) {
            n++;
            *out++ = *e->p;
        }
    }
    if (n) func_000573AC(buf, n, &D_00096D1C, 4, 1, 0x20);
}
