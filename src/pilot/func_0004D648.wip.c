extern int D_00096C94;
extern int D_00096C7C;
void func_0004D648(int a) {
    int x;
    volatile int pad[4];
    D_00096C94 = a;
    x = a + 10000;
    if (x >= 0 && x > 10000) {
        a = 10000;
    } else {
        a = x;
        if (a < 0) a = 0;
    }
    D_00096C7C = a + (a >> 1) + (a >> 3) + (a >> 7);
}
