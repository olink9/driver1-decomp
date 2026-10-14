extern int D_00096F08, D_00096F0C, D_00096F10, D_00096F68;
extern long long D_0009763C;
extern char D_0008D064[], D_0008D024[];
extern char *D_000977CC;
extern char *D_000977C0;
extern void func_0006C8D4();

int func_0006C078(void) {
    int old;
    D_00096F10 = 3;
    old = D_00096F08;
    D_00096F08 = 2;
    D_00096F68 = 1;
    *(int *)&D_0009763C = 0;
    D_000977CC = D_0008D064;
    D_000977C0 = D_0008D024;
    D_00096F0C = old;
    func_0006C8D4(D_0008D064, 2, 2);
    D_00096F10 = 3;
    D_000977C0[D_00096F08 * 16 + 0xC] = 0xFF;
    D_000977C0[D_00096F08 * 16 + 0xD] = 0xFF;
    D_000977C0[D_00096F08 * 16 + 0xE] = 0xFF;
    return D_00096F08;
}
