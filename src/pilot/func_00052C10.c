typedef struct {
    int f0;
    int f4;
    int f8;
    int fC;
} AO;
typedef struct {
    char pad0[4];
    short f4;
} M;
typedef struct {
    char pad[0xE];
    unsigned short fE;
} CO;
extern M *D_000A0E20[];
extern AO *D_0008AE3C[];
extern long long D_00096834;
extern int D_000975AC;
extern void func_00052CDC();

/* DrawAnimatingObject */
void func_00052C10(int a0, CO *cop)
{
    AO *aop;
    int loop;
    int type;

    M *model;
    aop = D_0008AE3C[*(int *)&D_00096834];
    model = D_000A0E20[cop->fE];
    type = model->f4 == -1 ? cop->fE : model->f4;
    for (loop = 0; loop < D_000975AC; loop++, aop++) {
        if (type == aop->f4) {
            type = aop->f0;
            func_00052CDC(cop, type);
        }
    }
}
