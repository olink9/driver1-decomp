extern int D_001CF460, D_001CF45C, D_001CF438, D_001CF44C[];
typedef struct { int a, b, c; unsigned char r, g, b2, pad; } E;
extern E D_001CF368[];

void func_001CCEDC(void)
{
    if (D_001CF460 == 0) {
        D_001CF45C += 16;
        if (D_001CF45C >= 256) {
            D_001CF45C = 255;
            D_001CF460 = 1;
        }
    } else {
        D_001CF45C -= 16;
        if (D_001CF45C < 0) {
            D_001CF45C = 0;
            D_001CF460 = 0;
        }
    }
    if (D_001CF438 != -1) {
        D_001CF368[D_001CF44C[D_001CF438]].r = D_001CF45C;
        D_001CF368[D_001CF44C[D_001CF438]].g = D_001CF45C;
        D_001CF368[D_001CF44C[D_001CF438]].b2 = D_001CF45C;
    }
}
