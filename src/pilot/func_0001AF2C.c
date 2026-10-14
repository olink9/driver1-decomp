typedef struct { int vx, vy, vz, pad; } VECTOR;

typedef struct {
    unsigned char id;       /* 0x00 */
    unsigned char pad1;
    unsigned char controlType; /* 0x02 */
    char pad3[0xC8 - 3];
    int x;                  /* 0xC8 */
    int y;                  /* 0xCC */
    int z;                  /* 0xD0 */
    char padD4[0xF8 - 0xD4];
    int fF8;                /* 0xF8 */
    char padFC[396 - 0xFC];
} CAR_DATA;

typedef struct {
    unsigned char f0;       /* 0x00 */
    unsigned char f1;       /* 0x01 */
    unsigned char f2;       /* 0x02 */
    char pad3[2];
    unsigned char f5;       /* 0x05 */
    char pad6[10];
    unsigned char f10;      /* 0x10 */
    unsigned char f11;      /* 0x11 */
    char pad12[0x50 - 0x12];
} CIV;

extern CAR_DATA D_0009B0A4[];
extern CIV D_00098900[];
extern VECTOR D_0008A8F0;
extern long long D_00096834;
extern int func_0001F5B0(VECTOR *pos);
extern void func_0001B4A4(CAR_DATA *cp, VECTOR *pos);
extern void func_0001B720(CAR_DATA *cp, int n);
extern void func_0003EB0C(int a, int b);

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

void func_0001AF2C(int slot, VECTOR *pos, int a2, int type, int arg4)
{
    VECTOR tmp;
    CAR_DATA *cp;
    CIV *civ;

    ClearMem((unsigned char *)&D_0009B0A4[slot], sizeof(CAR_DATA));
    tmp.vx = pos->vx;
    tmp.vz = pos->vz;
    tmp.vy = func_0001F5B0(&tmp) + 0xA3;
    cp = &D_0009B0A4[slot];
    func_0001B4A4(cp, &tmp);
    cp->fF8 = a2;
    func_0001B720(cp, 1);

    if (slot != 0) {
        civ = &D_00098900[slot];
        ClearMem((unsigned char *)civ, sizeof(CIV));
        civ->f10 = type;
        civ->f11 = arg4;
        civ->f5 = 0;
        civ->f1 = 7;
        civ->f2 = 1;
        civ->f0 = slot - 1;
        func_0003EB0C(slot, civ->f10);
        if (civ->f10 == 3 && *(int *)&D_00096834 == 5)
            civ->f11 = 5;
    } else {
        func_0003EB0C(0, 0);
    }

    D_0009B0A4[slot].id = slot;
    D_0009B0A4[slot].controlType = type;
    D_0008A8F0.vx = D_0009B0A4[slot].x;
    D_0008A8F0.vy = -400;
    D_0008A8F0.vz = D_0009B0A4[slot].z;
}
