typedef struct {
    int op;
    int a;
    int b;
    int c;
} CMD;

typedef struct {
    int w[8];
} ENT;

extern int D_000868DC[];
extern CMD *D_000977F0;
extern CMD *D_000977EC;
extern int D_00096F90;
extern int D_000977E8;
extern ENT *D_000977E0;
extern ENT *D_000977E4;
extern long long D_000967E0, D_00096814, D_000967F4, D_00096834, D_00096808, D_000967F8, D_000967C4;
extern int D_000C6F04[];
extern void func_00024754();
extern void func_0006EE90();
extern int func_0006EEAC();
extern CMD *func_0006EEC4();

void func_0006EB74(void)
{
    int run = 1;

    D_000977EC = &D_000977F0[D_000868DC[1]];
    D_00096F90 = D_000868DC[2];
    D_000977E8 = D_000868DC[3];
    do {
        switch (D_000977EC->op) {
        case 0:
            D_00096F90 = D_000977EC->a;
            if (*(int *)&D_000967E0 < D_00096F90 + 1)
                *(int *)&D_000967E0 = D_00096F90 + 1;
            D_000977EC = &D_000977F0[D_000977EC->b];
            break;
        case 1:
            D_000868DC[0] = *(int *)&D_000967E0;
            D_000868DC[1] = D_000977EC - D_000977F0;
            D_000868DC[2] = D_00096F90;
            D_000868DC[3] = D_000977E8;
            D_000977E4 = &D_000977E0[D_000977EC->a];
            D_000977EC = func_0006EEC4(D_000977E4);
            if (D_000977EC == 0) {
                *(int *)&D_00096814 = 0;
                run = 0;
            }
            break;
        case 2:
            *(int *)&D_000967F4 = 5;
            *(int *)&D_00096834 = D_00096F90;
            *(int *)&D_00096808 = D_000977EC->a;
            func_00024754();
            *(int *)&D_000967F4 = 0x20;
            D_000977EC = &D_000977F0[D_000977EC->b];
            break;
        case 3:
            *(int *)&D_000967F4 = 8;
            *(int *)&D_000967F8 = D_000977EC->a;
            func_00024754();
            if (*(int *)&D_000967C4 == 1) {
                *(int *)&D_00096814 = 0;
                run = 0;
            }
            D_000977EC = &D_000977F0[D_000977EC->b];
            break;
        case 4:
            func_0006EE90(D_000977EC->a);
            D_000977EC = &D_000977F0[D_000977EC->b];
            break;
        case 5:
            if (func_0006EEAC(D_000977EC->a) == 0)
                D_000977EC = &D_000977F0[D_000977EC->c];
            else
                D_000977EC = &D_000977F0[D_000977EC->b];
            break;
        case 6:
            if (D_000C6F04[0x104 / 4] == 0xFF0)
                D_000977EC = &D_000977F0[D_000977EC->c];
            else
                D_000977EC = &D_000977F0[D_000977EC->b];
            break;
        case 7:
            D_000977EC = &D_000977F0[D_000977EC->b];
            break;
        case 8:
            if (D_000C6F04[0x104 / 4] == 0xFF0) {
                D_000977EC = &D_000977F0[D_000868DC[1]];
                D_00096F90 = D_000868DC[2];
                D_000977E8 = D_000868DC[3];
            } else
                D_000977EC = &D_000977F0[D_000977EC->b];
            break;
        case 9:
            if (D_000C6F04[0x104 / 4] == 0xFF0) {
                run = 0;
                *(int *)&D_00096814 = 0;
                D_000977EC = &D_000977F0[D_000868DC[1]];
                D_00096F90 = D_000868DC[2];
                D_000977E8 = D_000868DC[3];
            } else
                D_000977EC = &D_000977F0[D_000977EC->b];
            break;
        default:
            run = 0;
            break;
        }
        if (*(int *)&D_000967C4 == -1) {
            *(int *)&D_00096814 = 0;
            run = 0;
        }
    } while (run);
}
