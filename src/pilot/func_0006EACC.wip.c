extern int D_00011650[];
extern int D_000868DC[];
extern int *D_000977E0;
extern int D_000977E8;
extern int *D_000977F0;
extern void func_00050E28();
void func_0006EACC(void) {
    int *p = (int *)0x1FE000;
    int *q;
    func_00050E28(D_00011650, 0x1FE000);
    D_000977F0 = (int *)0x1FE004;
    D_000977E8 = 0;
    __asm__("" : : "r"(1));
    q = D_000868DC;
    q[1] = 0;
    q[3] = 0;
    q[2] = 0;
    D_000977E0 = &(&p[(*p << 2) + 1])[1];
}
