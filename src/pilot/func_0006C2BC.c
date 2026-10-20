extern long long D_00096834;
extern int D_000868E0[];
extern long long D_000967E0;
extern long long D_000977E8;
extern long long D_00096F90;
extern int D_00096F08;
extern long long D_000977EC;
extern int D_000868DC[];
extern long long D_00097530, D_00097524;
extern char D_000BDFA4[];
extern char D_000115E8[];
extern void func_00074158(), func_00033AF8(), func_000513D4(), DrawSync(), func_0006EB48();
extern void func_00050E28(), func_EC280(), func_E7508(), func_0003A068();

typedef struct { char *primptr; int x; char *primtab; } DB;
#define current (*(DB **)&D_00097530)
#define last (*(DB **)&D_00097524)
int func_0006C2BC(void)
{
    char pad[8];
    func_00074158(0);
    func_00033AF8();
    func_000513D4();
    func_000513D4();
    DrawSync(0);
    func_0006EB48();
    if (*(int *)&D_00096834 == 2)
        D_000868E0[0] = 0;
    else
        D_000868E0[0] = (*(int *)(*(int *)&D_000977EC + 8) << 4) >> 4;
    D_000868DC[0] = *(int *)&D_000967E0;
    D_000868DC[2] = *(int *)&D_00096F90;
    D_000868DC[3] = *(int *)&D_000977E8;
    func_00050E28(D_000115E8, 0xE6600);
    func_EC280(1);
    current->primptr = current->primtab;
    last->primptr = last->primtab;
    func_E7508(4, D_000868DC, 0x1E00);
    *(int *)&D_00097530 = (int)D_000BDFA4;
    *(int *)&D_00097524 = (int)D_000BDFA4 + 0x80;
    func_0003A068();
    func_00033AF8();
    func_00074158(1);
    return D_00096F08;
}
