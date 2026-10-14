/* func_0001638C - no Driver 2 counterpart: spawn a car */
typedef struct {
    unsigned char id;
    char pad1;
    unsigned char f2;
    char pad3[0xC8 - 3];
    int x, y, z;
    char padD4[0xF8 - 0xD4];
    int fF8;
    char padFC[0x18C - 0xFC];
} CAR;

typedef struct {
    unsigned char id, f1, f2, pad3, pad4, f5, pad6, f7, f8, f9;
    char padA[0x10 - 0xA];
    unsigned char f10, f11;
    char pad12[0x17 - 0x12];
    unsigned char f17;
    char pad18[0x28 - 0x18];
    int f28;
    char pad2C[0x38 - 0x2C];
    int f38, f3C, f40, f44, f48, f4C;
} ENT;

typedef struct {
    char pad0[0x40];
    int f40;
    char pad44[0x78C - 0x44];
    int f78C, f790, f794;
    char pad798[0x7A4 - 0x798];
    CAR *f7A4;
} PED;

typedef struct {
    int a[6];
} SEG;

extern CAR D_0009B230[];
extern ENT D_00098950[];
extern PED D_000C4E38[];
extern int D_00096770;
extern int D_00096774;
extern long long D_00097764;
extern long long D_000970E8;
extern unsigned char func_00017A50();
extern int func_00063AA8();
extern int func_00017A64();
extern int func_000635B0();
extern void func_00021820();
extern void func_00016F0C();
extern void func_00020A64();
extern int func_0004FFE8();
extern int func_00063B14();
extern void func_0003EB0C();
extern void func_0001B720();
extern unsigned char func_00015338();

static inline void ClearMem(unsigned char *p, int n)
{
    unsigned char *end = p + n;

    while (((int)p & 3) && p < end)
        *p++ = 0;
    while (p <= (unsigned char *)((int *)end - 1)) {
        *(int *)p = 0;
        p += 4;
    }
    while (p < end)
        *p++ = 0;
}

CAR *func_0001638C(int dir, int x, int z)
{
    SEG seg;
    CAR *cp = 0;
    ENT *e;
    PED *pd;
    int slot;
    int r;
    int n;
    unsigned char one;

    if (func_00017A50() == 0 || func_00063AA8() == 0)
        goto end;
    slot = func_00017A64();
    cp = &D_0009B230[slot];
    e = &D_00098950[slot];
    ClearMem((unsigned char *)cp, sizeof(CAR));
    ClearMem((unsigned char *)e, sizeof(ENT));
    e->id = slot;
    cp->id = slot + 1;
    e->f7 = 0;
    e->f8 = 0;
    e->f2 = 0;
    e->f28 = -1;
    one = 1;
    e->f8 = one;
    func_00021820(x, z, func_000635B0(dir), &seg);
    func_00016F0C(e, &seg);
    cp->x = x;
    cp->z = z;
    func_00020A64(cp);
    cp->fF8 = dir;
    e->f44 = 0x7FFFFFFF;
    e->f40 = 0x7FFFFFFF;
    e->f38 = 0;
    e->f3C = 0x82;
    if (D_00096774 != 0 && (D_00096770 & func_0004FFE8(x)) == 0) {
        r = func_0004FFE8(slot);
        e->f10 = r % 2 + 2;
        e->f11 = r % 4 + 1;
    } else {
        e->f10 = one;
    }
    n = func_00063B14();
    e->f5 = n + 1;
    pd = &D_000C4E38[n];
    pd->f78C = 1;
    pd->f790 = 0;
    pd->f794 = 1;
    pd->f40 = 0;
    pd->f7A4 = cp;
    pd->f794 = 0;
    (*(int *)&D_00097764)++;
    e->f1 = 4;
    cp->f2 = e->f10;
    func_0003EB0C(slot + 1, e->f10);
    func_0001B720(cp, 1);
    e->f48 = D_0009B230[e->id].x / 1500;
    e->f4C = D_0009B230[e->id].z / 1500;
    e->f9 = func_00015338(e);
    e->f17 = 2;
    if (*(int *)&D_000970E8 != 0)
        e->f17 = 0x42;
end:
    return cp;
}
