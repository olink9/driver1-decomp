extern int D_00096C90, D_00096C88;
extern void func_000735DC();
void func_0004E454(int x) {
    int v;
    D_00096C90 = x;
    v = x + 10000;
    if (v < 0 || v <= 10000) {
        if (v < 0) v = 0;
    } else v = 10000;
    func_000735DC(D_00096C88, ((v >> 7) + (v >> 8)) & 0xFF);
}
