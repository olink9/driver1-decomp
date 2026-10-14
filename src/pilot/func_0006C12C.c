typedef struct { char pad[12]; char c, d, e, f; } E;
extern int D_00096F08, D_00096F0C, D_00096F10, D_00096F68;
extern long long D_0009763C;
extern E *D_000977C0;
extern char *D_000977CC;
extern char D_0008D024[], D_0008D064[];
extern void func_0006C8D4();
int func_0006C12C(void) {
    D_00096F0C = D_00096F08;
    D_00096F10 = 3;
    D_00096F08 = 3;
    D_00096F68 = 1;
    D_000977CC = D_0008D064;
    D_000977C0 = (E *)D_0008D024;
    func_0006C8D4(D_0008D064, 2, 2);
    D_00096F10 = 3;
    D_000977C0[D_00096F08].c = 0xFF;
    D_000977C0[D_00096F08].d = 0xFF;
    D_000977C0[D_00096F08].e = 0xFF;
    *(int *)&D_0009763C = 1;
    return D_00096F08;
}
