struct E { char pad[0x10]; int f10; short f14; unsigned char f16; char p17; };
struct Q { int a, b; };
extern int D_00097448;
extern volatile struct Q D_00097534;
extern struct E D_000B738C[];
extern int D_000BBDD0[];

void func_000430B8(void)
{
    int i;
    for (i = 0; i < 20; i++) {
        if (D_00097448 == 0) break;
        if (D_000B738C[i].f16 == 0) {
            int v = D_000BBDD0[--D_00097448];
            short t = D_00097534.a - 2;
            D_000B738C[i].f10 = v;
            D_000B738C[i].f14 = t;
        }
    }
    for (i = 0; i < 20; i++) {
        D_000BBDD0[i] = 0;
        D_000B738C[i].f16 = 0;
    }
    D_00097448 = 0;
}
