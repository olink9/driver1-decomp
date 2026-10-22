extern int MapHeight();

void func_00067E58(int *a0, int *a1) {
    int *p = (int *)a0[4];
    int v;
    a1[0] = p[0];
    a1[2] = p[2];
    if (a0[3] & 0x40) {
        a1[1] = -p[1];
    } else {
        v = p[1];
        if (v == 0x7FFFFFFF) {
            a1[1] = -MapHeight(a1);
        } else {
            a1[1] = v;
        }
    }
}
