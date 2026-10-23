/* func_001C5B28: menu screen enter. D_001CE4B8 = 0 BEFORE D_001CDE5C = 3: shortens the %hi(D_001CE4B8) pseudo so it
   beats the shared constant 1 in a local-alloc priority tie (.2308 each; ties go to the earlier-born qty). */
extern int D_000967F4, D_00096834, D_00096858, D_001CE4B8, D_001CDE5C;
extern char D_001CE430[], D_001CE458[], D_001CE498[];
extern int D_001C054C[];
typedef struct { int a; int b; int c; int d; } E;
extern void func_001C2114();
extern void SetDispMask();
extern void func_00024754();
extern void func_001C5D28();
extern void func_001C2644();

int func_001C5B28(void)
{
    E *t = (E *)D_001CE430;
    D_00096858 = 1;
    D_00096834 = 2;
    D_000967F4 = 3;
    func_001C2114(t, D_001CE458, 0);
    SetDispMask(0);
    func_00024754();
    D_001CE4B8 = 1;
    func_001C5D28();
    func_001C2644(D_001CE498, 2, 0);
    D_001CE4B8 = 0;
    D_001CDE5C = 3;
    t->b = (int)D_001C054C;
    func_001C2114(t, D_001CE458, 1);
    D_00096858 = 0;
    return 0;
}
