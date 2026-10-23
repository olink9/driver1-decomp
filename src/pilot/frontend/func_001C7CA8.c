extern int D_001CEDF8;
extern char D_001CEDC4[], D_001CED34[];
extern void func_001C2644();
extern void func_001C2114();
extern void func_000514E0();
extern int func_001C26E4();
extern void func_001C19C4();
extern void func_001C1BD4();
extern void func_001C11E4();

int func_001C7CA8(void)
{
    int r;
    D_001CEDF8 = 0;
    func_001C2644(D_001CEDC4, 3, 0);
    func_001C2114(0, D_001CED34, 1);
    for (;;) {
        func_000514E0();
        r = func_001C26E4(D_001CEDC4, &D_001CEDF8);
        if (r != 0) break;
        func_001C19C4();
        func_001C1BD4(D_001CED34);
        func_001C11E4();
    }
    func_001C2114(0, D_001CED34, 0);
    if (r == -1) return 0;
    return r;
}
