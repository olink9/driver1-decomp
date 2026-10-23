extern int D_00096808;
extern int D_000967F4;
extern char D_001CEE68[];
extern char D_001CEED8[];
extern int D_001CEF28;
extern void func_001C2114();
extern void func_001C2644();
extern void func_00024754();
extern void func_001C8F54();

int func_001C8E20(void)
{
    func_001C2114(0, D_001CEE68, 0);
    switch (D_001CEF28) {
    case 0:
        func_001C8F54();
        break;
    case 1:
        D_00096808 = 1;
        D_000967F4 = 7;
        func_00024754();
        func_001C2644(D_001CEED8, 5, 1);
        break;
    case 2:
        D_00096808 = 2;
        D_000967F4 = 7;
        func_00024754();
        func_001C2644(D_001CEED8, 5, 2);
        break;
    case 3:
        D_00096808 = 3;
        D_000967F4 = 7;
        func_00024754();
        func_001C2644(D_001CEED8, 5, 3);
        break;
    case 4:
        D_00096808 = 4;
        D_000967F4 = 7;
        func_00024754();
        func_001C2644(D_001CEED8, 5, 4);
        break;
    case 5:
        break;
    }
    return 1;
}
