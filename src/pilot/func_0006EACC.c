/* FAKEMATCH: empty asm on r keeps the return value 1 in a pseudo set early (target loads v0=1 mid-function, temps avoid v0) */
extern int D_00011650[];
extern int D_000868DC[];
extern int *D_000977E0;
extern int D_000977E8;
extern int *D_000977F0;
extern void func_00050E28();
int func_0006EACC(void) {
    int *p = (int *)0x1FE000;
    int r;
    func_00050E28(D_00011650, 0x1FE000);
    D_000977F0 = (int *)0x1FE004;
    r = 1;
    __asm__("" : "=r"(r) : "0"(r));
    D_000977E8 = 0;
    D_000868DC[1] = 0;
    D_000868DC[3] = 0;
    D_000868DC[2] = 0;
    D_000977E0 = &(&p[(*p << 2) + 1])[1];
    return r;
}
