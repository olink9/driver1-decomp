extern void LoadfileSeg();
int func_0005C3D0(int a, int b, int *c, int d) {
    int r;
    LoadfileSeg(a, c, 0, 0x640);
    if (c[b] != -1) {
        LoadfileSeg(a, c, c[b], d);
        r = 1;
    } else {
        r = 0;
    }
    return r;
}
