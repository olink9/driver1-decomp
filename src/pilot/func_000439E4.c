/* Driver 2 name unknown (flashing light pair, sibling of func_0004388C) */
typedef struct { short x, y, z, pad; } SV;
typedef struct {
    char pad0[2]; unsigned char model; char pad1[0x111];
    int f114, f118; int pad2; int f120, f124;
    char pad3[0x18C - 0x128];
} CAR; /* 0x18C */
typedef struct { unsigned char b0; char b1; unsigned char b2; char pad[0x18C - 3]; } CAR2;
typedef struct { char f0; unsigned char f1; char pad[78]; } ENT; /* 80 */
typedef struct { char pad[8]; SV a; SV b; char pad2[100 - 0x18]; } MODEL; /* 100 */
extern CAR car_data[];
extern CAR2 D_0009B1D4[];
extern ENT D_00098950[];
extern MODEL car_cosmetics[];
extern long long pauseflag;
extern char D_000B9170[];
extern void func_00043FDC();

void func_000439E4(int idx, int mode)
{
    unsigned char col[4];
    SV v2;
    SV v1;
    MODEL *m;
    unsigned char c;
    int t;
    unsigned char *b0 = &D_0009B1D4[idx].b0;
    unsigned char *b2 = &D_0009B1D4[idx].b2;
    if (*b0 < 128)
        t = *b0 * 2;
    else
        t = (255 - *b0) * 2;
    col[0] = t;
    c = t;
    col[1] = 0;
    col[2] = 0;
    if (*(int *)&pauseflag == 0)
        *b0 += 8;
    if (car_data[idx].model == 1 && D_00098950[idx - 1].f1 == 4)
        return;
    if (idx == 0)
        m = car_cosmetics;
    else
        m = &car_cosmetics[car_data[idx].model];
    v1 = m->b;
    v2 = m->a;
    if (mode == 0 || mode == 2) {
        if (car_data[idx].f124 < 500) {
            *b2 += c >> 3;
            func_00043FDC(&v1, idx, col, 30, D_000B9170, 0);
        }
        if (car_data[idx].f114 < 500) {
            col[0] = c;
            func_00043FDC(&v2, idx, col, 30, D_000B9170, 1);
        }
    }
    if ((unsigned)(mode - 1) < 2) {
        if (car_data[idx].f120 < 500) {
            v1.x = -v1.x;
            *b2 += c >> 3;
            col[0] = c;
            func_00043FDC(&v1, idx, col, 30, D_000B9170, 0);
        }
        if (car_data[idx].f118 < 500) {
            col[0] = c;
            v2.x = -v2.x;
            func_00043FDC(&v2, idx, col, 30, D_000B9170, 1);
        }
    }
}
