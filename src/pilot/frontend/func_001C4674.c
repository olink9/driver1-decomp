extern int D_000967F4;
extern int D_00096808;
extern char D_001CDFC0[];
extern char D_001CDFE8[];
extern void func_001C11E4();
extern void DrawSync();
extern void VSync();
extern void func_001C2114();
extern void SetDispMask();

int func_001C4674(void)
{
    if (D_000967F4 != 0x13) {
        func_001C11E4();
        DrawSync(0);
        VSync(0);
        func_001C2114(D_001CDFC0, D_001CDFE8, 0);
        SetDispMask(0);
        D_00096808 = 0x7C;
    } else {
        func_001C2114(D_001CDFC0, D_001CDFE8, 0);
    }
    return 1;
}
