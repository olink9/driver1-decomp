typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct {
    unsigned char id;
    char pad1;
    unsigned char model;
    char pad3[0xC8 - 3];
    int x, y, z;
    char padD4[0xF8 - 0xD4];
    int fF8;
    char padFC[0x18C - 0xFC];
} CAR;
typedef struct {
    unsigned char id, f1, f2, pad3, pad4, f5, pad6, f7, f8;
    char pad9[0x10 - 9];
    unsigned char f10, f11;
    char pad12[0x17 - 0x12];
    unsigned char f17;
    char pad18[0x28 - 0x18];
    int f28;
    char pad2C[0x38 - 0x2C];
    int f38, f3C, f40, f44, f48, f4C;
} ENT;

extern CAR D_0009B230[];
extern ENT D_00098950[];
extern int func_00017A64(void);
extern void func_00020A64(CAR *);
extern void func_000774C4(void *, int, int);
extern void func_0001B4A4(CAR *, VECTOR *);
extern void func_0003EB0C(int, unsigned int);
extern void func_0001B720(CAR *, int);

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

CAR *func_00016094(int model, int a1, int a2, int x, int z)
{
    VECTOR v2;
    VECTOR v;
    int slot;
    ENT *e;
    CAR *cp = 0;

    slot = func_00017A64();
    if (slot != -1) {
        cp = &D_0009B230[slot];
        e = &D_00098950[slot];
        ClearMem((unsigned char *)cp, sizeof(CAR));
        ClearMem((unsigned char *)e, sizeof(ENT));
        e->id = slot;
        cp->id = slot + 1;
        e->f1 = 1;
        e->f7 = 1;
        e->f28 = -1;
        e->f8 = 0;
        e->f2 = 0;
        cp->x = x;
        cp->y = 200;
        cp->z = z;
        func_00020A64(cp);
        func_000774C4(&v, 0, 16);
        v.vx = cp->x;
        v.vy = cp->y;
        v.vz = cp->z;
        v2 = v;
        func_0001B4A4(cp, &v2);
        cp->fF8 = a2;
        cp->model = model;
        e->f10 = model;
        e->f44 = 0x7FFFFFFF;
        e->f40 = 0x7FFFFFFF;
        e->f3C = 0;
        e->f38 = 0;
        e->f11 = a1 + 1;
        e->f5 = 0;
        func_0003EB0C(slot + 1, e->f10);
        func_0001B720(cp, 1);
        e->f48 = D_0009B230[e->id].x / 1500;
        e->f4C = D_0009B230[e->id].z / 1500;
        e->f17 = 0;
    }
    return cp;
}
