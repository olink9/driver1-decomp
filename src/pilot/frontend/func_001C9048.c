extern int D_001CEF58, D_001CEF54, D_00096C90, D_001CEF84, D_001CEF38, D_000BFA78, D_001CF28C, D_001CEF50;
extern int D_001CEF5C, D_001CEF88, D_001CEF60, D_001D3110, D_001D3118, D_00096804;
extern unsigned char D_00096F1C, D_000967BC;
extern char D_001C09C8[], D_001C09D4[], D_001D30F0[], D_001D3100[], D_001CEF30[];
extern void func_0004D5F8(), func_001CB190(), CloseShutters(), func_001C9A0C(), GetTextureDetails();
extern int func_001C9A18();
extern void func_001CB4A0(), SetTextColour(), func_000514E0(), func_001CB380(), func_001C9298(), func_001CB95C();
extern void func_001C977C(), DrawSync(), func_000513D4(), func_0006F6E8();
extern int func_001C9344(), VSync();

int func_001C9048(int a0)
{
    int n = 0;

    D_001CEF58 = a0;
    D_00096F1C = 0;
    D_000967BC = 1;
    D_001CEF54 = D_00096C90;
    func_0004D5F8(n);
    func_001CB190();
    CloseShutters(2, 320, 512);
    D_001CEF84 = 0;
    func_001C9A0C(5);
    D_001CEF38 = D_000BFA78;
    D_001CEF84 = 0;
    GetTextureDetails(D_001C09C8, D_001D30F0);
    GetTextureDetails(D_001C09D4, D_001D3100);
    D_001CF28C = -1;
    D_001CEF50 = 0;
    func_001CB4A0(1, func_001C9A18(n));
    D_001CEF5C = 0;
    SetTextColour(255, 255, 255);
    for (;;) {
        func_000514E0();
        if (func_001C9344() == -1)
            break;
        func_001CB380();
        if (D_001CEF5C == 1) {
            D_00096C90 += D_001D3118;
            if (D_00096C90 >= D_001D3110) {
                D_00096C90 = D_001D3110;
                D_001CEF5C = 0;
            }
        }
        if (D_001CEF88 == 0) {
            if (n == 1)
                func_001C9298();
            if (D_001CEF88 == 0)
                func_001CB95C(D_001CEF30);
        }
        n = D_001CEF88;
        if (n == 0)
            func_001C977C();
        while ((unsigned)(VSync(-1) - D_001CEF60) < 2)
            ;
        D_001CEF60 = VSync(-1);
        DrawSync(0);
        func_000513D4();
    }
    D_00096804 = 0;
    D_00096C90 = D_001CEF54;
    func_0006F6E8();
    return D_001CF28C;
}
