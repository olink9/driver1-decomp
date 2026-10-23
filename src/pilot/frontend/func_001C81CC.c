extern int D_001CECB8;
extern char D_001CECD4[];
extern int func_001C7730();
extern void func_001C2114();

int func_001C81CC(void)
{
    int r;
    func_001C2114(0, D_001CECD4, 0);
    D_001CECB8 = 0x11;
    r = func_001C7730();
    func_001C2114(0, D_001CECD4, 1);
    return r;
}
