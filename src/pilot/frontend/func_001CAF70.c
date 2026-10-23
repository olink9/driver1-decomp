extern int D_001CF26C, D_001CF27C, D_001CF278, D_001CDECC;
extern char D_001CF208[], D_001CF248[];
extern void func_000514E0();
extern int func_001CB790();
extern void func_001CB380();
extern void func_001CA298();
extern void func_001CB95C();
extern void DrawSync();
extern void func_000513D4();

int func_001CAF70(void)
{
    D_001CF26C = 1;
    D_001CF208[0x1C] = 255;
    D_001CF208[0x1D] = 255;
    D_001CF208[0x1E] = 255;
    D_001CF208[0x2C] = 255;
    D_001CF208[0x2D] = 0;
    D_001CF208[0x2E] = 0;
    D_001CF27C = 0;
    D_001CDECC = 0;
    do {
        func_000514E0();
        D_001CF26C = func_001CB790(D_001CF248, D_001CF26C);
        if (D_001CF26C == -1) D_001CF27C = 1;
        func_001CB380();
        func_001CA298();
        func_001CB95C(D_001CF208);
        DrawSync(0);
        func_000513D4();
    } while (D_001CF27C == 0);
    return D_001CF278;
}
