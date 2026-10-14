extern long long D_000967F4;
extern long long D_0009709C;
extern int D_00096F18, D_00096F08, D_00096F10, D_00096F04, D_00096F0C, D_00096F68;
extern int D_000977CC, D_000977C0;
extern char D_0008D064[], D_0008D024[];
extern void func_0006C8D4();

int func_0006BE58(void)
{
    int old;

    if (*(int *)&D_000967F4 > 16) {
        if (*(int *)&D_0009709C == 0) {
        old = D_00096F08;
        D_00096F08 = 2;
        D_00096F68 = 1;
        D_000977CC = (int)D_0008D064;
        D_000977C0 = (int)D_0008D024;
        D_00096F0C = old;
        func_0006C8D4(D_0008D064, 2, 2);
        D_00096F10 = 1;
        } else {
            D_00096F10 = 1;
            D_00096F04 = 1;
        }
    } else if (D_00096F18 != 1) {
        old = D_00096F08;
        D_00096F08 = 2;
        D_00096F68 = 1;
        D_000977CC = (int)D_0008D064;
        D_000977C0 = (int)D_0008D024;
        D_00096F0C = old;
        func_0006C8D4(D_0008D064, 2, 2);
        D_00096F10 = 1;
    } else {
        D_00096F04 = D_00096F18;
    }
    return D_00096F08;
}
