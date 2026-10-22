/* run3-r4 (r4-cell, no improvement; 90.22 kept). One shared pointer-returning inline for both lookups
   (sibling forms, separate remainder locals, all 20 valid statement orders, flat and 2D index) scores 75-85:
   with params substituted, sched1 always issues the z divide first (its chain D - zq -> &1 -> <<1 is longer)
   and finishes the x quotient before the z mult; the target overlaps them (mult x; mfhi; mult z) in both lookups.
   Moving the wip's q60 form into a shared inline also drops to 83-85. */
/* WIP 90.22 (run3-r4; was 88.33). Driver 2: unknown - road direction at (x, z).
   run3-r4 fixes: D_0009E714 as `int[]` with `t = D[3] + 750; cz = (t - z) / 1500;` (sibling form);
   first lookup `xc = cx / 60; zc = q60(cz);` - routing ONE quotient through a tiny static inline is what
   makes sched1 issue the cx divide first (no statement order does); loop lookup order swept (zq, D, xr, xq, zr);
   `int count = 0;` declared before `unsigned char dir`; `while (cell == GetCell(..))` gives `bne t6, v0`.
   Rows 0-45 (prologue, bounds, allocation of base/hi/cx/cz) now identical.
   Remaining: (1) lookup scheduling - target is xq, zq, xr, zr, then D - zq in BOTH lookups (same code twice,
   probably one inline); ours differs in the z>>31 sra / andi placement, and the loop lookup uses other temps.
   (2) dir/count swap: target dir=t4 (inited first), count=t3 (`addu t3,t4,zero`), ours dir=t3, count=t4;
   every init order/decl order/`count = dir` variant either swaps the regs or the copy. Permuter (2 runs, ~25 min)
   found only semantics-breaking candidates (dropping `& 1`, assignments inside conditions). */
typedef struct {
    short x0;
    short junction;
    short x4;
    unsigned char x6;
    unsigned char x7;
    int x8;
    int xC;
} CELLINFO;

typedef struct {
    short a;
    short id;
    int b;
} JENTRY;

typedef struct {
    int x0;
    int x4;
    JENTRY e[4];
    int x28;
} JUNCTION;

extern int D_000967A8;
extern int D_0009E714[];
extern short (*D_0009E724[])[60];
extern long long D_00097374;
extern JUNCTION *D_00097040;
extern CELLINFO *D_00097044;

static inline int q60(int v) { return v / 60; }

static inline short GetCell(int x, int z)
{
    int xc, zc, lx, lz;
    zc = z / 60; zc = *(int *)&D_00097374 - zc; lx = x % 60; xc = x / 60; lz = z % 60;
    return D_0009E724[(xc & 1) + ((zc & 1) << 1)][lz][lx];
}

int func_00020AE4(int x, int z)
{
    int count = 0;
    unsigned char dir = 0;
    int cx, cz, t, i;
    short cell;
    JUNCTION *j;
    CELLINFO *ci;

    if (D_000967A8 == 0)
        return -1;

    cx = (x + D_0009E714[2]) / 1500;
    t = D_0009E714[3] + 750;
    cz = (t - z) / 1500;
    if (cx < 0 || cx >= D_0009E714[0] || cz < 0 || cz >= D_0009E714[1])
        return -1;

    {
        short *cp;
        int xc, zc, lx, lz;
        xc = cx / 60; zc = q60(cz); zc = *(int *)&D_00097374 - zc; lz = cz % 60; lx = cx % 60;
        cp = &D_0009E724[(xc & 1) + ((zc & 1) << 1)][lz][lx];
        cell = *cp;
        if (*cp & 0xC000)
            return *cp;
    }
    j = &D_00097040[D_00097044[cell].junction];
    for (i = 0; i < 4; i++)
    {
        if (j->e[i].id == cell)
            break;
        dir ^= 1;
    }

    while (cell == GetCell(cx, cz))
    {
        count++;
        if (dir)
            cz++;
        else
            cx++;
    }

    ci = &D_00097044[cell];
    count = ci->x6 - count;
    if (ci->x7 & (unsigned)(1 << count))
        return dir != 0;
    if (dir)
        return 3;
    return 2;
}
