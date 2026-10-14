extern int D_00096F08, D_00096F0C, D_00096F10, D_00096F68, D_000977C0, D_000977C4, D_000977C8, D_000977CC, D_000977D0;
extern long long D_0009763C;
extern void func_0006C8D4();
int func_0006C438(void) {
    int a = D_000977D0, b = D_000977C4, c = D_00096F0C, d = D_000977C8;
    D_00096F68 = 1;
    D_00096F10 = -1;
    D_000977CC = a;
    D_000977C0 = b;
    D_00096F08 = c;
    func_0006C8D4(a, d, c);
    *(int *)&D_0009763C = 0;
    return D_00096F08;
}
