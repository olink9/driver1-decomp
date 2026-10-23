extern int D_000967F0, D_000967F4, D_001CDE5C;
extern char D_001CDCD8[], D_001CDC84[];
extern void func_001C2114();
extern void func_001C8D08();
extern int DrawSync();
extern int VSync();
extern void func_001C11E4();
extern void SetDispMask();
extern void func_00024754();

int func_001C0CF8(void)
{
    func_001C2114(D_001CDCD8, 0, 0);
    D_001CDE5C = 6;
    func_001C8D08();
    if (D_000967F0 != 0) {
        DrawSync(0);
        VSync(0);
        func_001C11E4();
        D_000967F4 = 16;
        func_001C2114(0, D_001CDC84, 0);
        SetDispMask(0);
        func_00024754();
    }
    D_001CDE5C = 1;
    return 2;
}
