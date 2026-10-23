/* WIP 92.3%: load/lui order and the a0 vs v0 split of x and x+1 differ (ov1). */
extern int D_00096808;
extern int D_001D2DC0;
extern char D_001CDFC0[];
extern char D_001CDFE8[];
extern void func_001C11E4();
extern void DrawSync();
extern void VSync();
extern void func_001C2114();
extern void SetDispMask();

int func_001C4A7C(void)
{
    int a, b;
    func_001C11E4();
    DrawSync(0);
    VSync(0);
    func_001C2114(D_001CDFC0, D_001CDFE8, 0);
    SetDispMask(0);
    a = D_001D2DC0;
    b = a + 1;
    D_00096808 += a;
    return b;
}
