extern int D_001CDE5C, D_001CEC5C, D_001CEC58, D_001CDC80;
extern char D_001CEA60[], D_001CEA88[], D_001CEC38[];
extern void func_001C7900();
extern void func_001C2114();
extern void func_000514E0();
extern int func_001C26E4();
extern void func_001C19C4();
extern void func_001C1BD4();
extern void func_001C11E4();

int func_001C7730(void)
{
    int r;
    int prev;
    D_001CDE5C = 5;
    D_001CEC5C = 0;
    func_001C7900();
    D_001CEC58 = 0;
    func_001C2114(D_001CEA60, D_001CEA88, 1);
    for (;;) {
        prev = D_001CEC5C;
        func_000514E0();
        r = func_001C26E4(D_001CEC38, &D_001CEC58);
        if (r != 0) break;
        if (prev != D_001CEC5C)
            func_001C7900();
        if (D_001CDC80 != 0) {
            func_001C19C4();
            func_001C1BD4(D_001CEA88);
            func_001C11E4();
            func_001C19C4();
            func_001C1BD4(D_001CEA88);
            func_001C11E4();
        }
    }
    D_001CDE5C = 6;
    func_001C2114(D_001CEA60, D_001CEA88, 0);
    if (r == -1) return 0;
    return r;
}
