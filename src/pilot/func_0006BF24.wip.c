struct T { char pad[0xC]; unsigned char c, d, e, f; };
extern int D_00096F68;
extern int D_00096F08;
extern int D_00096F0C;
extern int D_00096F10;
extern char *D_000977CC;
extern struct T *D_000977C0;
extern char D_0008D064[];
extern char D_0008D024[];
extern void func_0006C8D4();

int func_0006BF24(void)
{
    int x;
    D_00096F68 = 1;
    D_00096F0C = D_00096F08;
    D_00096F08 = x = 2;
    D_000977CC = D_0008D064;
    D_000977C0 = (struct T *)D_0008D024;
    func_0006C8D4(D_0008D064, x, x);
    D_00096F10 = x;
    D_000977C0[D_00096F08].c = 0xFF;
    D_000977C0[D_00096F08].d = 0xFF;
    D_000977C0[D_00096F08].e = 0xFF;
    return D_00096F08;
}
