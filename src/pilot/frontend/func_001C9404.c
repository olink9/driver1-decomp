typedef struct { char c[0x80]; } DB;
extern DB *current, *D_00097524;
extern DB D_000BDFA4[];
extern int D_001CEF84, D_001CEF88, D_001CF28C, D_001CEF5C, D_001D3110, D_001D3118, D_00096C90, D_00096834;
extern char D_001CEF30[], D_001C09F4[], D_000868DC[];
extern int func_001CAF70(), func_001C9A18(), func_001CA600();
extern void func_001CB4A0(), func_001CB380(), func_001CA298(), func_001CB95C(), DrawSync(), func_000513D4();
extern void func_001CD5A8(), SetDispMask(), func_00033AF8(), Loadfile(), func_000EC280(), func_000E7508();
extern void func_0003A068(), func_0006EB48(), func_001C9A0C(), func_0006F57C();

int func_001C9404(void)
{
    int s;

    switch (D_001CEF84) {
    case 1:
        if (func_001CAF70() != 1)
            return 0;
        func_001C9A18(6);
        func_001CB4A0(0, func_001C9A18(6));
        while (D_001CEF88 != 0) {
            func_001CB380();
            func_001CA298();
            func_001CB95C(D_001CEF30);
            DrawSync(0);
            func_000513D4();
        }
        D_001CF28C = -1;
        return 1;
    case 2:
        func_001CB4A0(0, func_001C9A18(7));
        while (D_001CEF88 != 0) {
            func_001CB380();
            func_001CA298();
            func_001CB95C(D_001CEF30);
            DrawSync(0);
            func_000513D4();
        }
        D_001CF28C = -2;
        return -1;
    case 3:
        func_001CD5A8();
        return 0;
    case 4:
        D_001D3110 = D_00096C90;
        if (func_001CA600() == 1)
            return -1;
        D_001D3118 = 10000 / func_001C9A18(4);
        D_001CEF5C = 1;
        break;
    case 0:
        s = D_00096834;
        func_001CB4A0(0, func_001C9A18(5));
        SetDispMask(0);
        func_00033AF8();
        func_000513D4();
        func_000513D4();
        DrawSync(0);
        Loadfile(D_001C09F4, 0xE6600);
        func_000EC280(1);
        func_000E7508(4, D_000868DC, 0x1E00);
        SetDispMask(0);
        D_00096834 = 4;
        current = D_000BDFA4;
        D_00097524 = D_000BDFA4 + 1;
        func_0003A068();
        func_00033AF8();
        D_00096834 = s;
        func_0006EB48();
        D_001CEF84 = 0;
        func_001C9A0C(5);
        func_001CB4A0(1, func_001C9A18(0));
        func_0006F57C();
        break;
    }
    return 0;
}
