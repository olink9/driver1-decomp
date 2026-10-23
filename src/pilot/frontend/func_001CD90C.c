extern int D_001CF880, D_001CF87C, D_001CF850, D_001CF864[];
typedef struct { int a, b, c; unsigned char r, g, b2, pad; } E;
extern E D_001CF710[];

void func_001CD90C(void)
{
    if (D_001CF880 == 0) {
        D_001CF87C += 16;
        if (D_001CF87C >= 256) {
            D_001CF87C = 255;
            D_001CF880 = 1;
        }
    } else {
        D_001CF87C -= 16;
        if (D_001CF87C < 0) {
            D_001CF87C = 0;
            D_001CF880 = 0;
        }
    }
    if (D_001CF850 != -1) {
        D_001CF710[D_001CF864[D_001CF850]].r = D_001CF87C;
        D_001CF710[D_001CF864[D_001CF850]].g = D_001CF87C;
        D_001CF710[D_001CF864[D_001CF850]].b2 = D_001CF87C;
    }
}
