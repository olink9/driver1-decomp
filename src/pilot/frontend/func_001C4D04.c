extern int D_001CE200;
extern char D_001CE1A0[], D_001CE120[];
extern void func_001C2644();
extern void func_001C2114();
extern void func_000514E0();
extern int func_001C26E4();
extern void func_001C19C4();
extern void func_001C1BD4();
extern void func_001C11E4();

int func_001C4D04(void)
{
    int r;
    D_001CE200 = 0;
    func_001C2644(D_001CE1A0, 6, 0);
    func_001C2114(0, D_001CE120, 1);
    for (;;) {
        func_000514E0();
        r = func_001C26E4(D_001CE1A0, &D_001CE200);
        if (r != 0) break;
        func_001C19C4();
        func_001C1BD4(D_001CE120);
        func_001C11E4();
    }
    if (r == -1) return 0;
    return r;
}
