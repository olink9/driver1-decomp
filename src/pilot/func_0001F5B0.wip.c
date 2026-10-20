/* r3-13 (run2-r3) 98.69: fixes found - D_000967A0 is a plain `int *` scalar (struct form kept the cell-pointer store
 * above the reload); `info` must stay live across the loop (a `(int)info < 0` test inside it - target keeps info in s5
 * and spills h); exactly ONE copy of the A0E20/0x4000 bump code (two copies double the lui savings -> loop.c hoists the
 * table base; target hoists only the index). The identical arms around `ht -= h` reproduce the dead hoisted flip (s6).
 * Left: s6/s7 swap of the hoisted flip (info&0x40000000) and h-sext: h-sext has 5 refs (used in both arms) vs flip 3;
 * target needs flip >= h-sext in priority but h-sext above the index (3 refs/39) and above h (spilled). Tried: h once
 * before/after the arms (refs 2-3 but live 44 -> h-sext spilled instead of h), flip tested twice, nested arms. */
/* func_0001F5B0 - no Driver 2 counterpart (map height lookup, cf. MapHeight) */

typedef struct { int vx, vy, vz, pad; } VEC;

typedef struct {
    char pad0[0x20];
    int nx;
    int pad24;
    int nz;
    int d;
    short type;
    short pad32;
} SUB;

typedef struct {
    char pad0[6];
    short count;
    SUB sub[1];
} SURF;

extern int D_0009E714[];
extern long long D_00096834;
extern long long D_00097374;
extern long long D_00096858;
extern int *D_0009D8F4[];
extern SURF *D_0009D904[];
extern unsigned short *D_000A0E20[];
extern short D_0008EAA8[];
extern int *D_000967A0;
extern int func_00024418();
extern int func_00024370();

int func_0001F5B0(VEC *pos)
{
    int x, z;
    int cx, cz;
    int tile;
    int q;
    unsigned int info;
    short h;
    SURF *surf;
    SUB *sp;
    int n;
    int r;
    int ht;
    int rx, rz;
    char pad[16];

    x = D_0009E714[2] + pos->vx;
    r = pos->vz - 750;
    z = D_0009E714[3] - r;
    cx = x / 1500;
    cz = z / 1500;

    if (*(int *)&D_00096834 == 2)
        return 0;
    if ((unsigned)cx >= D_0009E714[0])
        return 0;
    if ((unsigned)cz < D_0009E714[1]) {

    cx = (cx & 0x7f) + ((cx & ~0x7f) >> 4);
    if (cx >= 120)
        cx -= 120;
    cz = (cz & 0x7f) + ((cz & ~0x7f) >> 4);
    if (cz >= 120)
        cz -= 120;

    q = 0;
    if (cx >= 60) {
        cx -= 60;
        q = 1;
    }
    if (cz >= 60) {
        cz -= 60;
        if (!(*(int *)&D_00097374 & 1))
            q += 2;
    } else {
        if (*(int *)&D_00097374 & 1)
            q += 2;
    }

    D_000967A0 = D_0009D8F4[q] + cz * 60 + cx;
    info = ((int (*)[60])D_0009D8F4[q])[cz][cx];
    h = info;
    if (!*(int *)&D_00096858)
        h = info & 0xfff8;

    tile = (info >> 16) & 0x3ff;
    if (tile < 900)
        surf = D_0009D904[tile];
    else
        surf = 0;

    if (surf) {
        rx = x % 1500 - 750;
        rz = z % 1500 - 750;
        sp = surf->sub;
        if (info & 0x40000000) {
            x = rz;
            z = rx;
        } else {
            x = rx;
            z = -rz;
        }
        if ((int)info < 0) {
            x = -x;
            z = -z;
        }
        for (n = surf->count; n > 0; n--, sp++) {
            if (sp->type == 4)
                r = func_00024418(x, z, sp);
            else
                r = func_00024370(x, z, sp);
            if (r) {
                ht = (x * sp->nx + z * sp->nz) / 4096 - sp->d; if (((int)info < 0) || (info & 0x40000000)) { ht -= h; } else { ht -= h; } if (D_000A0E20[(info >> 16) & 0x3ff][1] & 0x4000) ht += D_0008EAA8[((x + z) << 2) & 0x1ffc] >> 9;
                return ht;
            }
        }
    }

    if (*(int *)&D_00096834 == 1 && pos->vx > -0x543A8 && pos->vx < -0x50334 &&
        pos->vz <= 0x412F7 && pos->vz > 0x3D284)
        return -1500;
    return -h;
    }
    return 0;
}
