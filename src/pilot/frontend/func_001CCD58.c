extern int D_001CF438;
extern char D_001CF3F8[];
extern char D_001CF368[];
extern void func_001CCDF4();
extern void func_000514E0();
extern int VSync();
extern int func_001CB790();
extern void func_001CCEDC();
extern void func_001CB380();
extern void func_001CA298();
extern void func_001CB95C();
extern int DrawSync();
extern void func_000513D4();

void func_001CCD58(void)
{
    func_001CCDF4();
    D_001CF438 = 0;
    do {
        func_000514E0();
        VSync(0);
        D_001CF438 = func_001CB790(D_001CF3F8, D_001CF438);
        func_001CCEDC();
        func_001CB380();
        func_001CA298();
        func_001CB95C(D_001CF368);
        DrawSync(0);
        func_000513D4();
    } while (D_001CF438 != -1);
}
