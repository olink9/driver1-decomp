typedef struct DB { int a; char *b; int c; char pad[0x74]; } DB;
extern int D_000ED780;
extern DB *current;
extern DB *D_00097524;
extern DB D_000BDFA4[];
extern DB D_000BE024[];
extern void DrawSync();
extern void VSync();
extern void PutDispEnv();
extern void PutDrawEnv();
extern void func_00074684();
extern void func_0007457C();

void func_000E90D8(void)
{
    if (D_000ED780 & 1) {
        current = D_000BDFA4;
        D_00097524 = D_000BDFA4 + 1;
    } else {
        current = D_000BE024;
        D_00097524 = D_000BE024 - 1;
    }
    PutDispEnv((char *)current + 0x6C);
    PutDrawEnv((char *)current + 0x10);
    current->a = current->c;
    func_00074684(D_00097524->b + 0x34);
    func_0007457C(current->b, 0xE);
    DrawSync(0);
    VSync(0);
    D_000ED780++;
}
