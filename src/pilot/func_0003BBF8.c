/* no Driver 2 counterpart */
typedef struct {
    short vx, vy, vz, pad;
} SVEC;

typedef struct {
    int a0, a1;
    int b0, b1;
    int c0, c1;
    int d0, d1;
    int e0, e1;
    int f0, f1;
    int g0, g1;
    int h;
} SEG;

typedef struct {
    char pad[0xC];
    unsigned short nverts;
    char pad2[2];
    SVEC *verts;
} MODEL;

typedef struct {
    int f0, f4, f8, fC, f10, f14;
} ELEM;

extern long long D_00096834;
extern SEG D_000AF5B0[];
extern short *D_000AF624[];
extern MODEL *D_000A1AB4[];
extern MODEL *D_000A1AD4[];
extern MODEL *D_000A1AB0[];
extern unsigned short D_00096A4C;
extern unsigned short D_00096A4E;
extern unsigned short D_00096A50;
extern void func_0003BDC8();

void func_0003BBF8(void)
{
    int *p;
    int lo, hi;
    int i, j;
    ELEM *e;
    short *a, *b;

    func_0003BDC8(&D_000AF5B0[0], D_000A1AB0[0], 1, 0, 1);
    func_0003BDC8(&D_000AF5B0[1], D_000A1AB0[1], 0, 0, 0);
    func_0003BDC8(&D_000AF5B0[2], D_000A1AB0[2], 0, 1, 0);
    func_0003BDC8(&D_000AF5B0[3], D_000A1AB0[3], 0, 1, 0);

    p = (int *)D_000AF5B0[0].a1;
    lo = p[2] & 0xFFFF0000;
    hi = p[3] & 0xFFFF0000;
    for (i = 0; i < 4; i++) {
        e = (ELEM *)D_000AF5B0[i].g1;
        for (j = 0; j < D_000AF5B0[i].g0; j++) {
            e->f8 = lo;
            e->fC = hi;
            e++;
        }
    }

    if (*(int *)&D_00096834 == 1) {
        a = D_000AF624[0];
        b = (short *)D_000A1AD4[0]->verts;
        i = D_000A1AB4[0]->nverts;
        while (i-- > 0) {
            a[0] += D_00096A4C;
            *(short *)((char *)a + 2) += D_00096A4E;
            *(short *)((char *)a + 4) += D_00096A50;
            b[0] += D_00096A4C;
            *(short *)((char *)b + 2) += D_00096A4E;
            *(short *)((char *)b + 4) += D_00096A50;
            a += 4;
            b += 4;
        }
    }
}
