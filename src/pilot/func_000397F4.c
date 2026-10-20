extern int D_00097398, D_00097368, D_0009735C, D_00097364;
int func_000397F4(int *p, char *tbl, int a, int b) {
    int x = (p[0] + D_00097398) / 3000 - a;
    int y = (p[2] + D_00097368) / 3000 - b;
    int lim = D_0009735C;
    if (abs(x) > lim || abs(y) > lim) return 0;
    y += 9; x += 9;
    return tbl[x + y * D_00097364] != 0;
}
