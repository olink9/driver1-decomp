extern int D_000967F4, D_00096808;
extern char D_001CDFC0[], D_001CDFE8[];
extern void func_001C2114();
extern int func_001C4AE4();

int func_001C4560(void)
{
    int r;
    if (D_000967F4 == 0x13) {
        func_001C2114(D_001CDFC0, D_001CDFE8, 0);
        return 1;
    }
    func_001C2114(D_001CDFC0, D_001CDFE8, 0);
    D_00096808 = 0x7A;
    r = func_001C4AE4();
    if (r == 0) {
        func_001C2114(D_001CDFC0, D_001CDFE8, 1);
    }
    return r;
}
