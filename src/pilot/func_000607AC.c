typedef struct { int a[4]; } V;
extern int D_000976C4;
extern int D_000C0250[];
extern V D_000C0280[];
extern int func_00060840();

int func_000607AC(int a) {
    int i;
    for (i = 0; i < D_000976C4; i++) {
        if (func_00060840(a, &D_000C0280[i]))
            return D_000C0250[i];
    }
    return 0;
}
