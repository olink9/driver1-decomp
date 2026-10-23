extern int D_00096C94, D_00096C90, D_000967EC, D_00096784;
extern int D_001D32F8, D_001D32FC, D_001D3300, D_001D3304;
extern int D_001CF850, D_001CF878, D_001CDECC, D_001CEF88, D_001CEF54;
extern char D_001CF7C0[], D_001CF710[];
extern void func_001C9A18(), func_001CD780(), func_000514E0(), VSync();
extern int func_001CB790();
extern void func_001CD90C(), func_001CB380(), func_001CA298(), func_001CB95C();
extern void DrawSync(), func_000513D4(), func_0004E454(), SetMasterVolume();
extern void func_001CD9FC(), SetTextColour(), func_001CA270();

int func_001CD5A8(void)
{
    func_001C9A18(8);
    D_001D32F8 = D_00096C94;
    D_001D32FC = D_00096C90;
    D_001D3300 = D_000967EC;
    D_001D3304 = D_00096784;
    func_001CD780();
    D_001CF878 = 0;
    D_001CF850 = 0;
    D_001CDECC = 1;
    do {
        func_000514E0();
        VSync(0);
        if (D_001CEF88 == 0) {
            D_001CF850 = func_001CB790(D_001CF7C0, D_001CF850);
            func_001CD90C();
        }
        func_001CB380();
        if (D_001CEF88 == 0) {
            func_001CA298();
            func_001CB95C(D_001CF710);
        }
        DrawSync(0);
        func_000513D4();
    } while (D_001CF850 != -1 && D_001CF878 == 0);
    if (D_001CF878 == 0) {
        D_00096C94 = D_001D32F8;
        D_00096C90 = D_001D32FC;
        D_000967EC = D_001D3300;
        D_00096784 = D_001D3304;
        func_0004E454(D_00096C90);
        SetMasterVolume(D_00096C94);
    } else {
        D_001CEF54 = D_00096C90;
    }
    func_001CD9FC(9);
    SetTextColour(0, 0, 0);
    func_001CA270(255, 255, 255, 0x40);
    func_001C9A18(3);
    return -1;
}
