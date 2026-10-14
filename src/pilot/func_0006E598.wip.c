int func_0006E598(int x, int y, int *p, int *out) {
    int i;
    int *e;
    e = p;
    for (i = 0; i < 5; i++) {
        if (x < e[2]) break;
        e += 3;
    }
    if (i == 5) i = -1;
    if (y < p[17]) out[2] = y; else out[2] = -1;
    out[0] = x;
    out[1] = i;
    return i;
}
