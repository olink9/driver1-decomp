/* WIP 99.95 (run2-r3): 'civ->t != 1' swap fixed by moving the rcossin step into static inline step_car().
 * Remaining: dz*dz product lands in v0 (target v1). Cause (.greg): dz*dz pseudo wins LO, dx*dx gets a2, then
 * reload spills LO for the dx*dx mult and retries dz*dz into the first free GPR (v0). In the target dx*dx must win
 * LO instead. Tried: operand/statement order, sq() inline, dist2() inline, in-place *=, 730 <= ..., !(<).
 * Plus reloc D_0008EAA8+2 vs D_0008EAAA (same bytes). */
/* Driver 2 name: unknown (per-frame civ car update / cull loop) */
typedef struct {
    unsigned char id;     /* 0x00 */
    unsigned char t;      /* 0x01 */
    unsigned char c2;     /* 0x02 */
    char p3[2];
    unsigned char f5;     /* 0x05 */
    char p6[2];
    unsigned char f8;     /* 0x08 */
    char p9;
    unsigned char fA;     /* 0x0A */
    char pB[0x18 - 0xB];
    int f18;              /* 0x18 */
    char p1C[0x38 - 0x1C];
    int f38;              /* 0x38 */
    char p3C[4];
    int f40;              /* 0x40 */
    int f44;              /* 0x44 */
    int x;                /* 0x48 */
    int z;                /* 0x4C */
} S80;
typedef struct {
    char p0[0xC8];
    int xC8;              /* 0xC8 */
    int pCC;
    int xD0;              /* 0xD0 */
    char pD4[0xF8 - 0xD4];
    int fF8;              /* 0xF8 */
    char pFC[0x104 - 0xFC];
    int f104;             /* 0x104 */
    int f108;             /* 0x108 */
    char p10C[0x12C - 0x10C];
    short f12C;           /* 0x12C */
    char p12E[0x18C - 0x12E];
} CAR;
typedef struct { char p[1960]; } S1960;
typedef struct { int x, z; } CELL;

extern S80 D_00098950[];
extern CAR D_0009B230[];
extern S1960 D_000C4690[];
extern unsigned char D_00099AF0[];
extern char D_00099120[];
extern short D_0008EAA8[];
extern short D_0008EAAA[];
extern void (*D_0008539C[])();
extern int D_0009677C;
extern int D_00096780;
extern long long D_000970D8;
extern int D_00096FD0;
extern int D_00096FF0;
extern int D_00096FCC;
extern int D_00096FEC;
extern unsigned char D_00096FC4;
extern void func_00063AB4();
extern void func_0001A008();
extern void func_000143CC();
extern void func_00013D88();
extern void func_0001D608();
extern void func_00018C74();
extern void func_00018618();
extern void func_00064FE0();
extern void func_00016B24();

static inline void remove_civ(int i)
{
    S80 *s = &D_00098950[i];
    int a = s->f5;
    if (s->t != 0 && D_0009677C != 0) {
        if (*(int *)&D_000970D8 != D_00096FD0 || i != D_00096FF0) {
            D_00096FF0 = i;
            D_00096FD0 = *(volatile int *)&D_000970D8;
        }
    }
    if (a != 0)
        func_00063AB4(&D_000C4690[s->f5]);
    if (s->t == 9)
        func_0001A008(D_00099120);
    s->t = 0;
    s->f8 = 0;
    {
        int t = D_00096FC4 - 1;
        D_00096FC4 = t;
        D_00099AF0[(unsigned char)t] = i;
    }
}

static inline void step_car(CAR *car, int idx)
{
    idx &= 0xFFF;
    car->xC8 += (D_0008EAA8[idx * 2] * 500 + 0x800) >> 12;
    car->xD0 += (D_0008EAA8[idx * 2 + 1] * 500 + 0x800) >> 12;
}
void func_0001A30C(void)
{
    char pad[0x50];
    struct { int x, z; } cell;
    int count;
    int px, pz;
    int i;
    CAR *car = D_0009B230;
    S80 *civ = D_00098950;
    int limit;
    int n;

    if (D_0009677C || D_00096780) {
        CAR *base = D_0009B230 - 1;
        px = base[23].xC8;
        pz = base[23].xD0;
    } else {
        CAR *base = D_0009B230 - 1;
        px = base[0].xC8;
        pz = base[0].xD0;
    }
    cell.x = px + 750;
    cell.z = pz - 750;
    cell.x /= 1500;
    cell.z /= 1500;
    func_000143CC();

    if (D_00096780 || D_0009677C)
        count = 24;
    else
        count = 22;

    for (i = 0; i < count; i++, civ++, car++) {
        int idx = car->fF8;
        if (civ->t != 1)
            step_car(car, idx);
        D_0008539C[civ->t](civ);
        if (civ->t != 0) {
            int dx, dz;
            dx = D_0009B230[i].xC8 - px;
            if (dx < 0)
                dx = px - D_0009B230[i].xC8;
            dz = D_0009B230[i].xD0 - pz;
            if (dz < 0)
                dz = pz - D_0009B230[i].xD0;
            dx /= 1500;
            dz /= 1500;
            if (dx * dx + dz * dz >= 730) {
                if (i < 22)
                    remove_civ(i);
                continue;
            }
            civ->x = D_0009B230[civ->id].xC8 / 1500;
            civ->z = D_0009B230[civ->id].xD0 / 1500;
            if ((*(int *)civ & 0xFFFF00) == 0x100 && car->f12C == 0) {
                if (civ->fA >= 20)
                    continue;
                civ->fA++;
            }
            if (civ->t != 4) {
                if (car->f104 > 0)
                    civ->f38 = car->f108;
                else
                    civ->f38 = -car->f108;
                if (i < 22) {
                    func_00013D88(civ, car);
                    func_0001D608(car, 0);
                }
                if (civ->t == 2 && (*(int *)&D_000970D8 & 1)) {
                    if (civ->f18 > 0x4650 && civ->f40 == 0x7FFFFFFF && civ->f44 == 0x7FFFFFFF)
                        func_00018C74(civ);
                    else
                        func_00018618(civ);
                }
            } else {
                func_0001D608(car, 0);
            }
        }
        if (civ->f5)
            func_00064FE0(civ);
    }

    D_00096FCC = 0;
    for (civ = D_00098950, i = 0; i < 22; i++, civ++) {
        if (!civ->t)
            D_00096FCC++;
    }
    D_00096FEC = 22 - D_00096FCC;
    if (D_00096780)
        D_00096FC4 = D_00096FEC;
    limit = 22;
    if (D_0009677C)
        limit = 18;
    if (D_00096FEC < limit) {
        n = D_00096FCC;
        if (n > limit)
            n = limit;
        while (n--)
            func_00016B24();
    }
}
