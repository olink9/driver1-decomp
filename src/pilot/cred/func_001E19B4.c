extern volatile int D_00097534;
extern int D_001F3860, D_001F3820, D_001F3848;
extern int D_001E1ABC;
extern void func_001E10D4();
extern void func_001E0D40();
extern void func_001E1660();
extern void func_001E1830(int a);

int func_001E19B4(void)
{
    if (D_00097534 > 60)
        func_001E10D4();
    func_001E0D40();
    if (D_001F3860 != 2 && D_001F3820 > 0x1928) {
        D_001E1ABC = 0;
        D_001F3860 = 2;
    }
    if (D_001F3848 == 0xF0)
        func_001E1660();
    if (D_001F3820 > 0x1928 && D_001F3848 == 0x200) {
        if (D_001E1ABC > 60)
            func_001E1830((D_001E1ABC - 60) * 4);
        if (D_001E1ABC > 200)
            return 1;
        D_001E1ABC++;
    }
    return 0;
}
