struct E { char pad[0x10]; int f10; short f14; unsigned char f16; char p17; };
struct Q { int a, b; };
extern int D_00097448;
extern volatile struct Q D_00097534;
extern struct E D_000B738C[];
extern int D_000BBDD0[];

void func_000430B8(void)
{
    int i, n;
    for (i = 0; i < 20; i++) {
        if (D_00097448 == 0) break;
        if (D_000B738C[i].f16 == 0) {
            n = D_00097448 - 1;
            D_000B738C[i].f10 = D_000BBDD0[n];
            D_00097448 = n;
            D_000B738C[i].f14 = D_00097534.a - 2;
        }
    }
    for (i = 0; i < 20; i++) {
        D_000BBDD0[i] = 0;
        D_000B738C[i].f16 = 0;
    }
    D_00097448 = 0;
}
