extern long long D_00097670;
extern long long current;
extern long long D_00097524;
extern int D_00096F08;
extern int D_000BDFA4[];
extern void SetDispMask();
extern void func_00033AF8();
extern void func_000513D4();
extern void DrawSync();
extern char D_000115E8[];
extern void Loadfile();
extern void func_EC280();
extern void func_E7508();
extern void func_0003A068();

int func_0006C1E4(void)
{
    int *a;
    int *b;

    SetDispMask(0);
    func_00033AF8();
    func_000513D4();
    func_000513D4();
    DrawSync(0);
    Loadfile(D_000115E8, 0xE6600);
    func_EC280(1);
    a = D_000BDFA4;
    b = a + 0x20;
    *(int *)&current = (int)a;
    *(int *)&D_00097524 = (int)b;
    D_000BDFA4[0] = a[2];
    b[0] = b[2];
    func_E7508(5, *(int *)&D_00097670, 0x3E00);
    *(int *)&current = (int)a;
    *(int *)&D_00097524 = (int)b;
    func_0003A068();
    func_00033AF8();
    SetDispMask(1);
    return D_00096F08;
}
