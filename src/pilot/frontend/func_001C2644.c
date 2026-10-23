typedef struct { void (*a)(); void (*b)(); int c, d; } E_001C2644;
extern int D_001CFDE8;

void func_001C2644(E_001C2644 *t, int n, int idx)
{
    int i;
    D_001CFDE8 = 1;
    for (i = 0; i < n; i++) {
        t[i].b();
    }
    t[idx].a();
    D_001CFDE8 = 0;
}
