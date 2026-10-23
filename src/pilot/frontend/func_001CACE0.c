typedef struct { int a, b, c, d; } E_32B0;
extern E_32B0 D_001D32B0[];
extern int D_001CF09C, D_001CF0A0, D_001CF0A4, D_001CF0DC, D_001CEF88;
extern short D_00097538;
extern char D_001CF0B8[];
extern void func_0006F5CC();
extern int func_0006F6DC();
extern void func_000514E0(), func_0006F728(), func_001CB380(), func_001CB95C(), func_001CAA0C();
extern void DrawSync(), func_000513D4();

void func_001CACE0(int mode)
{
    int saved = D_001CF09C;

    switch (mode) {
    case 8:
        func_0006F5CC(8, D_001D32B0[D_001CF0A0].c, D_001D32B0[D_001CF0A0].d);
        D_001CF0A4--;
        break;
    case 1:
        func_0006F5CC(1, D_001CF0A4 - 1, 0);
        break;
    case 4:
        func_0006F5CC(4, 0, D_001D32B0[D_001CF0A0].d);
        D_001CF0A4--;
        break;
    default:
        return;
    }
    D_001CF09C = -1;
    while (func_0006F6DC()) {
        func_000514E0();
        if (D_00097538)
            func_0006F728();
        func_001CB380();
        if (D_001CEF88 == 0) {
            func_001CB95C(D_001CF0B8);
            func_001CAA0C();
            D_001CF0DC++;
        }
        DrawSync(0);
        func_000513D4();
    }
    D_001CF09C = saved;
}
