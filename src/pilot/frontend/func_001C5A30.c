extern int D_001CE4B8;
extern char D_001CE498[], D_001CE430[], D_001CE458[];
extern void func_001C2644();
extern void func_001C5D28();
extern void func_001C2114();
extern void func_000514E0();
extern int func_001C26E4();
extern void func_001C19C4();
extern void func_001C5EE0();
extern void func_001C1BD4();
extern void func_001C11E4();

int func_001C5A30(void)
{
    int r;
    D_001CE4B8 = 0;
    func_001C2644(D_001CE498, 2, 0);
    func_001C5D28();
    func_001C2114(D_001CE430, D_001CE458, 1);
    for (;;) {
        func_000514E0();
        r = func_001C26E4(D_001CE498, &D_001CE4B8);
        if (r != 0) break;
        func_001C19C4();
        func_001C5EE0();
        func_001C1BD4(D_001CE458);
        func_001C11E4();
    }
    if (r == -1) return 0;
    return r;
}
