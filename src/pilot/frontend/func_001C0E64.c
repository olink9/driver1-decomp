extern int D_000967F4;
extern int D_001CDE5C;
extern int D_001CDE64;
extern char D_001CDCD8[];
extern void func_001C2114();
extern int func_001C41D8();
extern void func_00024754();

int func_001C0E64(void)
{
    func_001C2114(D_001CDCD8, 0, 0);
    D_001CDE5C = 2;
    D_001CDE64 = 2;
    D_000967F4 = 0x17;
    if (func_001C41D8()) {
        D_000967F4 = 0x17;
        func_00024754();
    }
    D_001CDE5C = 1;
    return 6;
}
