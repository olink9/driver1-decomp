/* Byte-identical: the only diff row is %lo(D_000C4E38+0x1ea0) vs %lo(D_000C6CD8), the same address (end of the 4-entry array). */
typedef struct {
    int pad0[3];
    int fC;
    int pad10[4];
    short f20[1];
} S7C;
typedef struct {
    char pad[0x20];
    int f20;
    int pad24;
    unsigned char f28;
} S120;
typedef struct {
    int pad[4];
    int f10;
    char rest[0x7A8 - 0x14];
} SE38;

extern int D_000976F8;
extern int D_0009770C;
extern int D_00097704;
extern int D_00097708;
extern int D_00096E68;
extern int D_00097714;
extern int D_00097760;
extern int D_00097764;
extern int D_00096E64;
extern long long D_00096FC8;
extern long long D_00096FF4;
extern int D_000C4C84[];
extern int D_000C4C98[];
extern long long D_00096E5C;
extern S7C D_000C4C7C;
extern S120 D_00099120;
extern char D_000988C8[];
extern SE38 D_000C4E38[];
extern void func_000635C0();
extern void func_000651F0();
extern void func_00019FEC();
extern void func_000635A8();

void func_00064A64(void)
{
    S7C *s;
    int i;
    int n;
    int best;
    int max;
    int v;
    int a;
    SE38 *p;
    short *q;

    if (D_000976F8 == 0) {
        D_0009770C = 0;
        D_00097704 = 0;
        return;
    }
    s = &D_000C4C7C;
    if (D_00097708 >= s->fC)
        D_00096E68 = 1;
    func_000635C0();
    func_000651F0(D_000988C8);
    func_00019FEC(&D_00099120);
    i = 0;
    q = s->f20;
    while (*q++ <= (short)D_00097708)
        i++;
    if (i < D_00097714)
        i = D_00097714;
    else if (i > D_00097760)
        i = D_00097760;
    if (i > D_00097764) {
        (*(int *)&D_00096FC8)++;
        a = (D_00099120.f28 == 2) ? D_00099120.f20 : D_00096E64;
        if ((a * (0x1000 - ((D_00097708 * D_000C4C84[0]) >> 12))) >> 12 < *(int *)&D_00096FC8)
            *(int *)&D_00096FF4 = 1;
    } else {
        *(int *)&D_00096FC8 = 0;
        if (i < D_00097764) {
            max = 0x7FFFFFFF;
            do {
                best = 0;
                n = 0;
                for (p = D_000C4E38; p < &D_000C4E38[4]; p++) {
                    if (p->f10 < max) {
                        n++;
                        if (best < p->f10)
                            best = p->f10;
                    }
                }
                max = best;
            } while (i < n);
            D_000C4C98[0] = max;
        } else {
            D_000C4C98[0] = 0x7FFFFFFF;
        }
    }
    if (*(unsigned char *)&D_00096E5C)
        func_000635A8();
}
