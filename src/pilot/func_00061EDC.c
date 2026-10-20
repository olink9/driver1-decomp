/* 64x64 grid of 4-byte cells: mark top row, bottom row (written as [0][i + 63*64]) and left column */
struct T { char p[3]; char v; };
extern struct T D_000C039C[64][64];
extern char D_000C429B[];
extern void func_00061C58();
extern void func_00062DB0();
void func_00061EDC(int a, int b, int c) {
    int i;
    func_00061C58();
    for (i = 0; i < 64; i++) {
        D_000C039C[0][i].v = 30;
        D_000C039C[0][i + 63*64].v = 30;
        D_000C039C[i][0].v = 30;
    }
    D_000C429B[0] = 30;
    func_00062DB0(c);
}
