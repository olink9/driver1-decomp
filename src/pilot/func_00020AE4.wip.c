/* WIP 88.33 (run2-r3 r3-cell; was 84.58). Driver 2: unknown - road direction at (x, z).
   Now: `%` remainders + `rz = D_00097374 - rz;` reassigned (form of matched func_00021558/00021690/000143CC).
   Remaining: (1) sched1 puts the cz divide first; the target keeps cx first with the whole lookup in RTL order
   (xq, zq, xr, zr, D) - all 120 statement orders x {%, x - q*60} x {expr, reassign, local m} swept, none gives
   cx first; a pointer-returning inline CellPtr(x, z) with in-place params is worse (75). (2) global alloc:
   target has base(&D_0009E714)=a3, cz=t0, cx=t1, hi=t2, count=t3, dir=t4 - i.e. base/hi outrank cx/cz/dir/count,
   which their refs/live (-dl) cannot explain; `count` is a copy of dir's zero (`addu t3,t4,zero`). */
typedef struct {
    int width;
    int height;
    int ofs_x;
    int ofs_z;
} MAPINFO;

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

typedef struct {
    short x0;
    short junction;
    short x4;
    unsigned char x6;
    unsigned char x7;
    int x8;
    int xC;
} CELLINFO;

extern int D_000967A8;
extern MAPINFO D_0009E714;
extern short (*D_0009E724[])[60];
extern long long D_00097374;
extern JUNCTION *D_00097040;
extern CELLINFO *D_00097044;

static inline short GetCell(int cx, int cz)
{
    int rx, rz, lx, lz, m;
    rz = cz / 60; lx = cx % 60; rx = cx / 60; lz = cz % 60; rz = *(int *)&D_00097374 - rz;
    return D_0009E724[(rx & 1) + ((rz & 1) << 1)][lz][lx];
}

int func_00020AE4(int x, int z)
{
    unsigned char dir = 0;
    int count = 0;
    int cx, cz, i;
    short cell;
    JUNCTION *j;
    CELLINFO *ci;

    if (D_000967A8 == 0)
        return -1;

    cx = (x + D_0009E714.ofs_x) / 1500;
    cz = (750 - z + D_0009E714.ofs_z) / 1500;

    if (cx < 0 || cx >= D_0009E714.width || cz < 0 || cz >= D_0009E714.height)
        return -1;

    {
        short *cp;
        int rx, rz, lx, lz, m;
        rz = cz / 60; lx = cx % 60; rx = cx / 60; lz = cz % 60; rz = *(int *)&D_00097374 - rz;
        cp = &D_0009E724[(rx & 1) + ((rz & 1) << 1)][lz][lx];
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

    while (GetCell(cx, cz) == cell)
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
