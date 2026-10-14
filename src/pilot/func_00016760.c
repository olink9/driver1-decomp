/* func_00016760 - no Driver 2 counterpart: create a car at a position (sibling of func_0001638C) */
typedef struct {
    unsigned char id;
    char pad1;
    unsigned char f2;
    char pad3[0xC8 - 3];
    int x, y, z;
    char padD4[0xF8 - 0xD4];
    int fF8;
    int fFC;
    char pad100[0x18C - 0x100];
} CAR;

typedef struct {
    unsigned char id, f1, f2, pad3, pad4, f5, pad6, f7, f8, f9, fA, fB;
    char padC[0x10 - 0xC];
    unsigned char f10, f11;
    char pad12[0x28 - 0x12];
    int f28;
    char pad2C[0x38 - 0x2C];
    int f38, f3C, f40, f44, f48, f4C;
} ENT;

typedef struct {
    int x, y, z, pad;
} VEC;

extern CAR D_0009B230[];
extern ENT D_00098950[];
extern int D_00096770;
extern void func_000774C4();
extern int func_00017A64();
extern void func_00020A64();
extern int func_0004FFE8();
extern void func_0003EB0C();
extern void func_0001B4A4();
extern void func_0001B720();

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

int func_00016760(int x, int y, int z, int dir, int flag)
{
    VEC v;
    CAR *cp;
    ENT *e;
    int slot;
    int r;

    func_000774C4(&v, 0, 16);
    v.x = x;
    v.y = y;
    v.z = z;
    slot = func_00017A64();
    if (slot == -1)
        return slot;
    e = &D_00098950[slot];
    cp = &D_0009B230[slot];
    ClearMem((unsigned char *)e, sizeof(ENT));
    ClearMem((unsigned char *)cp, sizeof(CAR));
    cp->id = slot + 1;
    e->id = slot;
    e->f7 = 0;
    e->f8 = 0;
    e->f2 = 0;
    e->f28 = -1;
    if ((func_0004FFE8(slot) & 1) && flag == 0)
        e->f7 = 1;
    e->fA = 0;
    e->fB = 0xFF;
    cp->x = x;
    cp->y = y;
    cp->z = z;
    cp->fF8 = dir;
    func_00020A64(cp);
    v.y = cp->y;
    e->f44 = 0x7FFFFFFF;
    e->f40 = 0x7FFFFFFF;
    e->f38 = 0;
    if (flag) {
        if (D_00096770) {
            e->f10 = 1;
            e->f11 = 1;
        } else {
            r = func_0004FFE8(slot);
            e->f10 = r % 2 + 2;
            e->f11 = r % 4 + 1;
        }
        e->f1 = 9;
        e->f2 = 1;
        e->f3C = 0;
        cp->fFC = 1;
    } else {
        r = func_0004FFE8(slot);
        e->f10 = r % 2 + 2;
        e->f11 = r % 4 + 1;
        e->f3C = 0x82;
        e->f1 = 1;
    }
    e->f5 = 0;
    cp->f2 = e->f10;
    func_0003EB0C(slot + 1, e->f10);
    func_0001B4A4(cp, &v);
    func_0001B720(cp, 0);
    e->f48 = D_0009B230[e->id].x / 1500;
    e->f4C = D_0009B230[e->id].z / 1500;
    return slot;
}
