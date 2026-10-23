typedef struct DB_11E4 {
    struct DB_11E4 *a;
    char *b;
    struct DB_11E4 *c;
} DB_11E4;
extern DB_11E4 *current;
extern DB_11E4 *D_00097524;
extern void DrawSync();
extern void VSync();
extern void PutDispEnv();
extern void PutDrawEnv();
extern void func_00074684();
extern void func_0007457C();

void func_001C11E4(void)
{
    DB_11E4 *t;
    DrawSync(0);
    VSync(0);
    PutDispEnv((char *)current + 0x6C);
    PutDrawEnv((char *)current + 0x10);
    t = current;
    D_00097524->a = D_00097524->c;
    current = D_00097524;
    D_00097524 = t;
    func_00074684(t->b + 0x40);
    func_0007457C(current->b, 0x10);
}
