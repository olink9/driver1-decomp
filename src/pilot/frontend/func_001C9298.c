extern unsigned int D_001CEF84;
extern int D_000BFA68, D_000BFA6C, D_000BFA70, D_000BFA74, D_000BFA78;
extern int D_001CEF38;
extern void func_001CA270();

void func_001C9298(void)
{
    switch (D_001CEF84) {
    case 1: D_001CEF38 = D_000BFA68; break;
    case 2: D_001CEF38 = D_000BFA6C; break;
    case 3: D_001CEF38 = D_000BFA70; break;
    case 4: D_001CEF38 = D_000BFA74; break;
    case 0: D_001CEF38 = D_000BFA78; break;
    }
    func_001CA270(255, 255, 255, 64);
}
