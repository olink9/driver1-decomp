extern void func_00050EC4();
int func_0005C3D0(int a, int b, int *c, int d) {
    int r;
    func_00050EC4(a, c, 0, 0x640);
    if (c[b] != -1) {
        func_00050EC4(a, c, c[b], d);
        r = 1;
    } else {
        r = 0;
    }
    return r;
}
