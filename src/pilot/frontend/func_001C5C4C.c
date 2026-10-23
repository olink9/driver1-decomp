/* FAKEMATCH: local `one` (=1) only steers register allocation */
extern int D_000967F4, D_00096834, D_00096858, D_001CE4B8, D_001CDE5C;
extern char D_001CE430[], D_001CE458[], D_001CE498[];
extern int D_001C0544[];
typedef struct { int a; int b; int c; int d; } E;
extern void func_001C2114();
extern void SetDispMask();
extern void func_00024754();
extern void func_001C5D28();
extern void func_001C2644();

int func_001C5C4C(void)
{
    int one;
    E *t = (E *)D_001CE430;
    D_00096834 = 6;
    D_000967F4 = 3;
    func_001C2114(t, D_001CE458, 0);
    SetDispMask(0);
    one = 1;
    func_00024754();
    D_001CE4B8 = one;
    func_001C5D28();
    func_001C2644(D_001CE498, 2, one);
    t->b = (int)D_001C0544;
    D_001CE4B8 = one;
    D_001CDE5C = 3;
    func_001C2114(t, D_001CE458, one);
    D_00096858 = 0;
    return 0;
}
