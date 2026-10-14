/* WIP 94.38: remaining diff is the in-place cx%60/cz%60 vs idx ordering and D_00097374 load hoisting
   (a separate local for D_00097374 gives 94.22 with only that block differing).
   func_00021820 - no Driver 2 counterpart: locate the road segment at (x, z) for a heading */
typedef struct {
    int d0, d1;
    short f8, fA, fC, fE, f10;
    unsigned char f12;
} SEG;

typedef struct {
    short f0, f2, f4, f6, f8, fA, fC, fE;
} ROAD;

typedef struct {
    short a, b;
    unsigned short c, d;
} JLINK;

typedef struct {
    char pad0[2];
    unsigned char f2;
    char pad3[5];
    JLINK l[4];
    int pad28;
} JUNC;

typedef struct {
    int f0, f4, f8, fC, f10;
} RECT;

extern int D_0009E714[];
extern long long D_00097374;
extern int *D_0009D8F4[];
extern unsigned short *D_0009E724[];
extern ROAD *D_00097044;
extern JUNC *D_00097040;
extern RECT *D_0009704C;
extern unsigned char D_000967B4[];

int func_00021820(int x, int z, unsigned char dir, SEG *seg)
{
    int t, xr, zr, cx, cz, xc, zc, idx, cell, info, d;
    unsigned short id;
    short sid;
    ROAD *rd;
    JUNC *j;
    RECT *r;
    unsigned char k;

    seg->fC = -1;
    seg->fE = -1;
    seg->f12 = 0;
    seg->f10 = 0;
    cx = (x + D_0009E714[2]) / 1500;
    t = D_0009E714[3] + 750;
    cz = (t - z) / 1500;
    if (cx < 0 || cx >= D_0009E714[0] || cz < 0 || cz >= D_0009E714[1])
        return -1;
    t = *(int *)&D_00097374;
    zc = cz / 60;
    xc = cx / 60;
    idx = (((t - zc) & 1) << 1) + (xc & 1);
    cell = (cz % 60) * 60 + cx % 60;
    id = D_0009E724[idx][cell];
    info = D_0009D8F4[idx][cell];
    seg->d1 = 0x7FFFFFFF;
    seg->d0 = 0x7FFFFFFF;
    sid = id;
    if (sid == -0x8000)
        return -1;
    if (id & 0x4000) {
        seg->fC = id & 0x3FFF;
        return 1;
    }
    seg->f8 = (info >> 16) & 0xC3FF;
    seg->fA = info;
    seg->fE = id;
    rd = &D_00097044[sid];
    k = D_000967B4[dir];
    if (D_00097040[rd->f2].l[k].b == sid)
        seg->fC = rd->f2;
    else
        seg->fC = rd->f4;
    j = &D_00097040[seg->fC];
    seg->f10 = j->l[k].c;
    if (j->f2 == 1)
        seg->f10 = 4;
    d = 0;
    r = &D_0009704C[(short)id];
    switch (dir) {
    case 0:
        t = r->f4 * 1500 - 750;
        d = t + z;
        d = -d;
        break;
    case 1:
        t = (r->f8 + 1) * 1500 - 750;
        d = t - x;
        break;
    case 2:
        t = (r->fC + 1) * 1500 - 750;
        d = t + z;
        break;
    case 3:
        t = r->f0 * 1500 - 750;
        d = x - t;
        break;
    }
    seg->d0 = d;
    seg->d1 = rd->f8 * 1500 - d;
    return 0;
}
