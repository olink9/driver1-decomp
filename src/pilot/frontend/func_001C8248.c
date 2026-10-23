extern int D_001CECB8;
extern char D_001CECD4[];
extern int func_001C7730();
extern void func_001C2114();

int func_001C8248(void)
{
    int r;
    D_001CECB8 = 0x13;
    func_001C2114(0, D_001CECD4, 0);
    r = func_001C7730();
    func_001C2114(0, D_001CECD4, 1);
    return r;
}
