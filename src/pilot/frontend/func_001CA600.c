extern int D_00096C90, D_001CEF88, D_001CF098, D_001CF09C, D_001CF0A0, D_001CF0A4, D_001CF0D8, D_001CF0DC;
extern char D_001CF0B8[];
extern void func_001CA270(), func_001CA7EC(), func_000514E0(), func_0004E454(), func_001CACE0();
extern void func_001CB380(), func_001CA298(), func_001CB95C(), func_001CAA0C(), DrawSync(), func_000513D4();
extern void SetTextColour(), func_001CB4A0();
extern int func_001C9A18(), func_001CA834();

int func_001CA600(void)
{
    int r;
    int step;

    func_001CA270(0, 0, 0, 0x40);
    step = (D_00096C90 - 10000) / func_001C9A18(9);
    if (D_001CF098 == 0)
        func_001CA7EC();
    D_001CF09C = 1;
    D_001CF0A0 = 0;
    D_001CF0DC = 0;
    D_001CF0D8 = 1;
    for (;;) {
        func_000514E0();
        r = func_001CA834();
        if (r != 0)
            break;
        if (D_001CF0DC == 2) {
            D_00096C90 = -10000;
            func_0004E454(-10000);
            D_001CF0D8 = 0;
            if (D_001CF0A4 > 0)
                func_001CACE0(1);
        }
        func_001CB380();
        func_001CA298();
        if (D_001CF0D8) {
            D_00096C90 += step;
            func_0004E454(D_00096C90);
        }
        if (D_001CEF88 == 0) {
            func_001CB95C(D_001CF0B8);
            func_001CAA0C();
            D_001CF0DC++;
        }
        DrawSync(0);
        func_000513D4();
    }
    SetTextColour(0, 0, 0);
    func_001CA270(0, 0, 0, 0x40);
    if (r != -1)
        func_001CB4A0(0, 0x20);
    return r;
}
