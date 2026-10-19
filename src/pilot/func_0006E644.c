typedef struct { int d[15]; } S60;
typedef struct { int d[18]; } S72;
extern S60 D_000869DC[], D_00086BBC[], D_000868EC[];
extern S72 D_00086D9C[];
extern int func_0006E4C0(), func_0006E4CC(), func_0006E558(), func_0006E598(), func_0006E604(), func_0006EA0C(), func_1C8330();

static inline int *getp(int **x, int type, int idx) {
    int *r;
    switch (type) {
    case 17: r = (int *)&D_000869DC[idx]; break;
    case 18: r = (int *)&D_00086BBC[idx]; break;
    case 19: r = (int *)&D_000868EC[idx]; break;
    case 22: r = (int *)&D_00086D9C[idx]; break;
    default: return 0;
    }
    return r;
}

void func_0006E644(int **a, int type, int idx) {
    int *p;
    int s0;
    int loc[4];
    int buf[2];
    int *lp;

    p = getp(a, type, idx);
    if (p == 0) return;
    loc[2] = -1;
    lp = loc;
    switch (type) {
    case 17:
        loc[0] = func_0006E4C0(a[0], *(int *)a[1]);
        s0 = func_0006E4CC(loc[0], p);
        break;
    case 18:
        loc[0] = (int)a[0];
        s0 = func_0006E604(loc[0], p);
        break;
    case 19:
        loc[0] = (int)a[0];
        s0 = func_0006E558(loc[0], p);
        break;
    case 22:
        s0 = func_0006E598(a[0], a[1][7], p, lp);
        break;
    default:
        s0 = -1;
        break;
    }
    if (s0 == -1 && loc[2] == -1) return;
    if (func_1C8330(buf)) {
        if (s0 != -1) func_0006EA0C(p, s0, buf, loc[0]);
        if (loc[2] != -1) func_0006EA0C(p, 5, buf, loc[2]);
    }
}
