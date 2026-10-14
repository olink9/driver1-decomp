typedef struct { char c[8]; } B8;
typedef struct { char p0[0x18]; B8 v; char p1[100 - 0x20]; } T100;
typedef struct {
    char p0[2]; unsigned char f2; char p1[0x120 - 3];
    int f120; int f124; char p2[0x132 - 0x128]; unsigned char f132;
    char p3[0x18C - 0x133];
} CAR;
extern T100 D_0009AF04[];
extern CAR D_0009B0A4[];
extern CAR D_0009B1D4[];
extern char D_000B9170[];
extern void func_00043FDC();

void func_00043D0C(int idx) {
    B8 v;
    char col[3];
    T100 *t;
    unsigned char *p;
    col[0] = 200;
    p = &D_0009B1D4[idx].f2;
    col[1] = 0;
    col[2] = 0;
    if (idx == 0)
        t = &D_0009AF04[0];
    else
        t = &D_0009AF04[*((unsigned char *)&D_0009B1D4[idx] - 0x12E)];
    v = t->v;
    if (D_0009B0A4[idx].f124 < 500) {
        func_00043FDC(&v, idx, col, 25, D_000B9170, 0);
        *p += 8;
    }
    if (D_0009B0A4[idx].f120 < 500) {
        *(short *)&v = -*(short *)&v;
        func_00043FDC(&v, idx, col, 25, D_000B9170, 0);
        *p += 8;
    }
}
