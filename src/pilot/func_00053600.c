extern int D_000BE744[3];
extern int D_00096D18;
void func_00053600(int *p) {
    if (p[0] != 0xFD46FEC0) {
        D_000BE744[0] = p[0];
        D_000BE744[1] = p[1];
        D_000BE744[2] = p[2];
        D_00096D18 = ((unsigned short *)p)[7];
    }
    p[0] = 0xFD46FEC0;
}
