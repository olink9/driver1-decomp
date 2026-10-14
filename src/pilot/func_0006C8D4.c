typedef struct { void (*f0)(); void (*f4)(); int a, b; } ENT;
extern int D_00096F08;

void func_0006C8D4(ENT *t, int n, int idx) {
    int i;
    for (i = 0; i < n; i++) {
        D_00096F08 = i;
        t[i].f4();
    }
    D_00096F08 = idx;
    t[idx].f0();
}
