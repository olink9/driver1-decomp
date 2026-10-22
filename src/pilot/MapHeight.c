/* FAKEMATCH: the `if ((info & 0x40000000) || (int)info < 0) r = 0; else r = 0;` statement in the loop is dead code
 * kept only for steering. Target hoists `info & 0x40000000` (s6) out of the loop and never uses it: the identical
 * arms are cross-jumped away after global-alloc, leaving the dead hoisted flip. Testing the flip FIRST in the `||`
 * gives it 4 refs (vs 3 for the hoisted h-sext), so it wins s6 over h-sext (s7) as in the target; `ht -= h` before
 * the test puts the h-sext first in hoist order. Real source probably had some flip-dependent code here that
 * compiled to identical arms. (r4-15, run3) */
/* MapHeight - no Driver 2 counterpart (map height lookup, cf. MapHeight) */

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
extern unsigned short *modelpointers[];
extern short rcossin_tbl[];
extern int *D_000967A0;
extern int func_00024418();
extern int func_00024370();

int MapHeight(VEC *pos)
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
                ht = (x * sp->nx + z * sp->nz) / 4096 - sp->d;
                ht -= h;
                if ((info & 0x40000000) || ((int)info < 0))
                    r = 0;
                else
                    r = 0;
                if (modelpointers[(info >> 16) & 0x3ff][1] & 0x4000)
                    ht += rcossin_tbl[((x + z) << 2) & 0x1ffc] >> 9;
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
