typedef struct { char pad[0xE]; unsigned short n; char pad2[4]; void *p; } T;
extern long long D_00096804, D_00096834;
extern int D_000973E4, D_000973E0, D_000973D0, D_000973D4;
extern int D_0008A6BC[], D_0008A6DC[], D_0008A71C[], D_0008A6FC[];
extern T *D_000A1AE0[];
extern char D_000B6DBC[];
extern void func_0003ECCC(), func_0003EB0C(), func_0003ECD4();

void func_0003E540(void)
{
    int i, j;
    if (*(int *)&D_00096804 == 1) return;
    D_000973E4 = D_0008A6BC[*(int *)&D_00096834];
    D_000973E0 = D_0008A6DC[*(int *)&D_00096834];
    D_000973D0 = D_0008A71C[*(int *)&D_00096834];
    D_000973D4 = D_0008A6FC[*(int *)&D_00096834];
    for (i = 1; i < 4; i++) {
        for (j = 0; j < D_000A1AE0[i]->n; j++)
            D_000B6DBC[i * 50 + j] = 0;
        func_0003ECCC(D_000A1AE0[i]->p, &D_000B6DBC[i * 50], D_000A1AE0[i]->n);
    }
    func_0003EB0C(0, 0);
    func_0003ECD4();
}
