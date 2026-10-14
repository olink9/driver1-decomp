typedef struct { int a, b, c, d; } T;
extern T D_000A20DC[];
extern T D_000A211C[];

void func_00029B88(int a0) {
    if (a0 != 0) a0 = 2;
    D_000A20DC[a0] = D_000A211C[a0];
    D_000A20DC[a0 + 1] = D_000A211C[a0 + 1];
}
