int func_0006E598(int x, int y, int *p, int *out) {
    int i, r;
    for (i = 0; i < 5; i++) {
        if (x < p[i*3+2]) break;
    }
    r = i;
    if (i == 5) r = -1;
    if (y < p[17]) out[2] = y; else out[2] = -1;
    out[0] = x;
    out[1] = r;
    return r;
}
