/* Driver 1 PC port - the 10 non-matching C functions that the PS1 build still takes as INCLUDE_ASM (pc_compat.h
 * empties INCLUDE_ASM, so their PC versions live here). Each section is the src/pilot draft named in its header,
 * copied verbatim apart from the scoping below; the drafts' matching notes are left in src/pilot. NFMV's main is
 * not here (NFMV is not linked into the PC build yet).
 *
 * Scoping: the drafts were written as stand-alone files, so several declare the same type names (DB, P_TAG, POLY_FT4,
 * E, H...) or the same globals with different local types. Each section renames its own typedefs and those globals
 * with #define (globals keep their real link name through an __asm__ label) and #undefs them afterwards.
 *
 * Verification: every draft was compiled with the PS1 toolchain (tools/fnscore.py, in a private build dir) and the
 * instruction listing compared with the original; all ten emit the same instruction count, and the differences
 * are register allocation / scheduling only (details per section). A draft's types describe the PS1 layout, which
 * the 32-bit PC build keeps (see port/CMakeLists.txt).
 * PC-PORT: like the rest of the game these still write 24-bit PS1 OT/prim tags (P_TAG addr:24) - see the
 * section-B report about OT pointers on PC. */

/* ======== func_00020AE4 - main (roadbits.c); draft src/pilot/func_00020AE4.wip.c (PS1 match 90.22%) ========
 * Checked by hand against asm_fn/nonmatchings/main/func_00020AE4.s (every load/offset/divide/branch).
 * Source: own decompilation (draft), verified as above; confidence: high. */
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
extern unsigned char D_00097374[]; /* PC: was long long (matching only) */
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

/* ======== func_00062DB0 - main (pathfind.c); draft src/pilot/func_00062DB0.wip.c (PS1 match 99.12%) ========
 * Objdiff: same 217 instructions, only a0/a1 and v0/v1 swapped.
 * Source: own decompilation (draft), verified as above; confidence: high. */
#define D_0009E714 D_0009E714_00062DB0
typedef struct {
    char pad0[0xC8];
    int x;          /* 0xC8 */
    char pad1[4];
    int z;          /* 0xD0 */
    char pad2[0xF8 - 0xD4];
    int ang;        /* 0xF8 */
    char pad3[0x108 - 0xFC];
    int len;        /* 0x108 */
    char pad4[396 - 0x10C];
} CAR;
typedef struct {
    unsigned char b0;
    unsigned char active;
    char pad[3];
    unsigned char id;
    char pad2[80 - 6];
} SLOT;
typedef struct {
    int pad[2];
    int x;
    int z;
} CAM;
typedef struct { unsigned char c[4]; } CELL;
typedef struct {
    int id;
    int pad[0x13];
    int x;      /* 0x50 */
    int z;      /* 0x54 */
} MAP;

extern unsigned char D_00098950[]; /* PC: was long long (matching only) */
extern CAR D_0009B230[];
extern CAM D_0009E714 __asm__("D_0009E714");
extern short rcossin_tbl[];
extern CELL D_000C039C[];

static inline int add750(int v) { return v + 750; }
#define FIX(a) (((a) + 2048) >> 12)

void func_00062DB0(MAP *m)
{
    int i;
    short *tb;
    int pass;
    int d;
    int len;
    int x, z;
    int px;
    int ns, c;
    int stepx, stepz;
    int dx;
    int z0;
    int dz;
    int idx;
    unsigned char bits;
    int pos[2];
    CAR *car;
    SLOT *s;

    for (i = 0; i < 22; i++) {
        car = &D_0009B230[(unsigned char)i];
        s = &((SLOT *)&D_00098950)[(unsigned char)i];
        if (s->active == 0)
            continue;
        if (s->id == m->id + 1)
            continue;
        ns = -rcossin_tbl[(car->ang & 0xFFF) * 2];
        stepx = FIX(ns * 750);
        z0 = add750(D_0009E714.z) - car->z;
        c = rcossin_tbl[(car->ang & 0xFFF) * 2 + 1];
        len = car->len * 20 + 450;
        px = car->x + D_0009E714.x;
        stepz = FIX(c * 750);
        dx = c * 180;
        dz = c * -900 - ns * 180;
        pos[0] = px + FIX(ns * -900 - dx);
        pos[1] = z0 + FIX(dz);
        for (pass = 0; pass < 2; pass++) {
            if (pass) {
                pos[0] = px + FIX(c * 180 + ns * -900);
                pos[1] = z0 + FIX(ns * 180 - c * 900);
            }
            for (d = -900; d < len; d += 750) {
                x = (pos[0] + 0x7270E0) / 750;
                z = (pos[1] + 0x7270E0) / 750;
                x -= m->x * 2;
                x -= 9936;
                z -= m->z * 2;
                z -= 9936;
                if ((unsigned)x < 128 && (unsigned)z < 128) {
                    idx = (x >> 1) + ((z >> 1) << 6);
                    switch ((z & 1) << 1 | (x & 1)) {
                    case 0:
                    default:
                        bits = 0x22;
                        break;
                    case 1:
                        bits = 0x24;
                        break;
                    case 2:
                        bits = 0x28;
                        break;
                    case 3:
                        bits = 0x30;
                        break;
                    }
                    D_000C039C[idx].c[3] = bits | D_000C039C[idx].c[3];
                }
                pos[0] += stepx;
                pos[1] += stepz;
            }
        }
    }
}
#undef D_0009E714

/* ======== func_00064360 - main (felony.c); draft src/pilot/func_00064360.wip.c (PS1 match 97.07%) ========
 * Objdiff: same instructions modulo registers/order except the target's andi 0xffff on a 5-bit table index (no-op).
 * Source: own decompilation (draft), verified as above; confidence: high. */
extern int D_0008BA0C[];
extern unsigned char D_00097374[]; /* PC: was long long (matching only) */
extern int D_0009770C;
extern int D_00097724;
extern int D_00097738;
extern int D_0009773C;
extern int D_00097748;
extern int D_0009774C;
extern int car_data[];
extern int (*D_0009D8F4[])[60];
extern int D_0009E714[];
extern int func_000641C0();
extern void func_00061EDC();

typedef struct {
    char pad0[0x48];
    int x;
    int z;
    int cx;
    int cz;
    char pad58[0x75C - 0x58];
    int px;
    int py;
    int pz;
} OBJ;

static inline int GetCellType(int x, int z)
{
    int zq, zoff, sel, xq, xoff;
    unsigned short *c;

    zq = z / 60;
    zoff = (z % 60) * 240;
    xq = x / 60;
    sel = ((*(int *)&D_00097374 - zq) << 1) & 2;
    xoff = (x % 60) * 4;
    c = (unsigned short *)((char *)D_0009D8F4[sel | (xq & 1)] + zoff + xoff);
    return D_0008BA0C[(unsigned short)(((c[1] & 0x3C00) >> 9) | (c[2] & 1))];
}

void func_00064360(OBJ *obj, int *dx, int *dz, int force)
{
    int tx, tz;
    int cz;
    int cx;
    int px;
    int pz;
    int flag;
    int v;
    int *car;

    car = car_data;
    tz = D_0009E714[3] - (car[52] - 750);
    tz /= 1500;
    cx = car[50] + D_0009E714[2];
    cx /= 1500;
    v = GetCellType(cx, tz);
    flag = 0;
    obj->x = D_00097738;
    obj->z = D_0009773C;
    if (force == 0) {
        if (D_0009770C > 0 && (D_00097724 == 0 || v != 0x1000)) {
            D_00097724 = 1;
            D_00097738 = obj->x = car[50];
            D_0009773C = obj->z = car[52];
        } else {
            flag = 1;
        }
        if (func_000641C0(obj))
            flag = 1;
    } else {
        obj->x = car[50];
        obj->z = car[52];
    }
    if (flag) {
        cx = obj->x + D_0009E714[2];
        cz = D_0009E714[3] - (obj->z - 750);
        cx = obj->cx = cx / 1500;
        cz = obj->cz = cz / 1500;
    } else {
        cx = obj->x + D_0009E714[2];
        cz = D_0009E714[3] - (obj->z - 750);
        cx = obj->cx = cx / 1500;
        cz = obj->cz = cz / 1500;
    }
    px = (obj->px + D_0009E714[2]) / 1500;
    pz = (D_0009E714[3] - (obj->pz - 750)) / 1500;
    D_00097748 = cx;
    D_0009774C = cz;
    func_00061EDC(cx, cz, obj);
    *dx = cx - px;
    *dz = cz - pz;
}

/* ======== func_001C2114 - FRONTEND (fecore.c); draft src/pilot/frontend/func_001C2114.wip.c (PS1 match 95.13%) ========
 * Objdiff: same instructions modulo registers/order (the constant 5 is rematerialised instead of kept in s8).
 * Source: own decompilation (draft), verified as above; confidence: high. */
#define P_TAG P_TAG_001C2114
#define DB DB_001C2114
#define current current_001C2114
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
typedef struct { unsigned int tag; unsigned char r0, g0, b0, code; short x0, y0, x1, y1, x2, y2, x3, y3; } PF4;
typedef struct { unsigned int *p; unsigned int **ot; } DB;
typedef struct { unsigned char r, g, b, cd; } CVEC;
typedef struct { short a, b; char c, d; short e; int f; int g; } E_001C1BD4;
extern int D_001CDE88, D_001CDE8C, D_001CDE5C;
extern DB *current __asm__("current");
extern void memset();
extern void SetDispMask();
extern void func_001C11E4();
extern void func_001C18AC();
extern void func_001C19C4();
extern void func_001C49C8();
extern void func_001C5EE0();
extern void func_000E7350();
extern void func_000E6F14();
extern void func_000E6F98();
extern void PC_ModMenuExtra(void *tbl);

static inline void func_001C2014(int x, int y, CVEC *c, int w, int h, int z)
{
    char pad[32];
    PF4 *p;
    x += D_001CDE88;
    y += D_001CDE8C;
    p = (PF4 *)current->p;
    ((unsigned char *)p)[3] = 5;
    p->y0 = y;
    p->y1 = y;
    ((unsigned char *)p)[7] = 0x29;
    p->x0 = x;
    p->x1 = x + w;
    p->x2 = x;
    p->y2 = y + h;
    p->x3 = x + w;
    p->y3 = y + h;
    p->r0 = c->r;
    p->g0 = c->g;
    p->b0 = c->b;
    ((P_TAG *)p)->addr = ((P_TAG *)current->ot[z])->addr;
    ((P_TAG *)current->ot[z])->addr = (unsigned)p;
    current->p += 6;
}

static inline void func_001C1BD4(E_001C1BD4 *p)
{
    E_001C1BD4 *q = p;
    func_000E7350();
    while (q->f != 0) {
        func_000E6F14(q->a, q->b, q->c, q->d, &q->g);
        func_000E6F98(q->f);
        q++;
    }
    PC_ModMenuExtra(p);    /* port/pc/pc_mods.c: mod menu entries (as fecore.c's func_001C1BD4) */
}

void func_001C2114(int unused, E_001C1BD4 *tbl, int mode)
{
    CVEC col;
    int h;
    int done;
    int i;

    memset(&col, 0, 4);
    done = 0;
    if (mode == 0) {
        h = 1;
        do {
            func_001C2014(0, 0, &col, 320, h, 1);
            func_001C2014(0, 512 - h, &col, 320, h, 1);
            h += 48;
            if (h >= 256) done = 1;
            func_001C11E4();
        } while (!done);
        SetDispMask(0);
    } else {
        h = 256;
        i = 0;
        SetDispMask(0);
        do {
            if (i == 2) SetDispMask(1);
            switch (D_001CDE5C) {
            case 1:
                func_001C18AC();
                break;
            case 2:
                func_001C19C4();
                func_001C49C8();
                break;
            case 3:
                func_001C19C4();
                func_001C5EE0();
                break;
            case 4:
            case 5:
            case 6:
                func_001C19C4();
                break;
            }
            func_001C2014(0, 0, &col, 320, h, 1);
            func_001C2014(0, 512 - h, &col, 320, h, 1);
            if (tbl) func_001C1BD4(tbl);
            if (i >= 3) h -= 48;
            i++;
            if (h <= 0) done = 1;
            func_001C11E4();
        } while (!done);
    }
}
#undef P_TAG
#undef DB
#undef current

/* ======== func_001CD7C0 - FRONTEND (ladoptions.c); draft src/pilot/frontend/func_001CD7C0.wip.c (PS1 match 99.13%) ========
 * Checked by hand against the asm: one extra pointer copy in the target, same stores (offsets 0x48/0x28/0x88/0x68).
 * Source: own decompilation (draft), verified as above; confidence: high. */
typedef struct { int a, b; char *s; int c; } T_001CF710;
extern T_001CF710 D_001CF710[];
extern int D_00096C90, D_00096C94, D_000967EC, D_00096784;
extern unsigned char D_000A801D;
extern char *D_001CF85C, *D_001CF860;
extern char D_001D32F0[], D_001D32F4[];
extern int sprintf();

static inline void sel(T_001CF710 *e, int on)
{
    if (on)
        e->s = D_001CF85C;
    else
        e->s = D_001CF860;
}

void func_001CD7C0(void)
{
    T_001CF710 *p;
    sprintf(D_001D32F0, "%d", D_00096C94 / 100 + 100);
    sprintf(D_001D32F4, "%d", D_00096C90 / 100 + 100);
    p = D_001CF710;
    p[4].s = D_001D32F0;
    p[2].s = D_001D32F4;
    if (D_000A801D & 4) {
        sel(&p[8], D_000967EC);
    } else {
        D_000967EC = 0;
        D_001CF710[8].s = D_001CF860;
    }
    p = D_001CF710;
    if (D_00096784)
        p[6].s = D_001CF85C;
    else
        p[6].s = D_001CF860;
}

/* ======== func_000E7508 - MCARD (mcard.c); draft src/pilot/mcard/func_000E7508.wip.c (PS1 match 96.93%) ========
 * Objdiff: same instructions modulo registers (msg string kept in s6 vs rematerialised).
 * Source: own decompilation (draft), verified as above; confidence: high. */
#define RECT RECT_000E7508
#define DB DB_000E7508
#define E E_000E7508
#define H H_000E7508
#define current current_000E7508
#define D_000EE930 D_000EE930_000E7508
#define D_00097524 D_00097524_000E7508
typedef struct { short x, y, w, h; } RECT;
typedef struct DB { int a; char *b; int c; char pad[0x74]; } DB;
typedef struct { char fname[0x18]; char title[0x24]; int size; unsigned char idx; unsigned char n; unsigned char f42; unsigned char f43; } E;
typedef struct { unsigned short s; unsigned char c2; char c3; void *p4; E e[15]; } H;
extern H D_000EE930[] __asm__("D_000EE930");
extern DB *current __asm__("current");
extern DB *D_00097524 __asm__("D_00097524");
extern short D_00097538;
extern int D_000EF144;
extern int D_000ED870;
extern int D_000EE900;
extern int D_000EFC00;
extern int D_000ED7CC;
extern int D_000ED7C4;
extern int D_000ED7C8;
extern int D_000EF138;
extern int D_000EF13C;
extern int D_000EF140;
extern char D_000ED784[];
extern char D_000E6608[];
extern char D_000E6610[];
extern char D_000E6618[];
extern char D_000E6620[];
extern char D_000E6628[];
extern char D_000E6634[];
extern char D_000E6644[];
extern char D_000E6650[];
extern char D_000E6664[];
extern char D_000E6670[];
extern char D_000E6680[];
extern char D_000E6690[];
extern char D_000E66AC[];
extern char D_000E66C4[];
extern void func_00074374();
extern void DrawSync();
extern void VSync();
extern void SetDispMask();
extern void func_00077E48();
extern void func_00077E98();
extern void func_00077ED8();
extern void func_00078114();
extern int func_00078E4C();
extern void func_0007457C();
extern void func_000514E0();
extern int sprintf();
extern int printf();
extern void func_000E8DD4();
extern void func_000E7B2C();
extern void func_000EA484();
extern void func_000E950C(), func_000E91B8(), func_000E90D8();
extern int func_000E96F8();
extern int func_000E9CE0();
extern int func_000EC4DC();

void func_000E7508(int mode, unsigned char *buf, unsigned int size)
{
    char str[16];
    RECT rect;
    int r[2];
    unsigned int st;
    char *msg;
    H *h;
    int ev;
    int port;
    int retry;
    int busy;
    int scr;
    int done;

    scr = 0;
    port = 0;
    done = 0;
    r[1] = 0;
    r[0] = 0;
    rect.x = 0;
    rect.y = 0;
    rect.w = 0x200;
    rect.h = 0x200;
    func_00074374(&rect, 0, 0, 0);
    DrawSync(0);
    func_00077E48();
    if (size + 0x200 <= 0x2000)
        sprintf(str, D_000E6608);
    else
        sprintf(str, D_000E6610);
    D_000ED870 = size / 8192 + (size % 8192 != 0);
    switch (mode) {
    case 0:
        D_000EF144 = 0;
        sprintf(D_000ED784, D_000E6618, D_000E6620);
        break;
    case 1:
        D_000EF144 = 1;
        sprintf(D_000ED784, D_000E6628, D_000E6620);
        break;
    case 2:
        D_000EF144 = 2;
        sprintf(D_000ED784, D_000E6634, D_000E6620);
        break;
    case 3:
        D_000EF144 = 5;
        sprintf(D_000ED784, D_000E6644, D_000E6650, size / 8192 + (size % 8192 != 0), str);
        break;
    case 4:
        D_000EF144 = 3;
        sprintf(D_000ED784, D_000E6644, D_000E6664, size / 8192 + (size % 8192 != 0), str);
        *(int *)(buf + 0x5E0) = D_000EE900 = func_000EC4DC(buf, 0x5E0);
        break;
    case 5:
        D_000EF144 = 4;
        sprintf(D_000ED784, D_000E6644, D_000E6670, size / 8192 + (size % 8192 != 0), str);
        break;
    case 6:
        D_000EF144 = 6;
        sprintf(D_000ED784, D_000E6644, D_000E6680, size / 8192 + (size % 8192 != 0), str);
        *(int *)(buf + 0x5E0) = D_000EE900 = func_000EC4DC(buf, 0x5E0);
        break;
    default:
        printf(D_000E6690);
        return;
    }
    func_000E8DD4(0);
    func_000E8DD4(1);
    msg = D_000E66AC;
    D_000EE930[0].c3 = 1;
    D_000EE930[1].c3 = 1;
    D_000ED7CC = 1;
    D_000ED7C4 = 0;
    D_000ED7C8 = 12;
    D_000EF138 = 0;
    D_000EF13C = 0;
    D_000EE930[0].p4 = msg;
    D_000EE930[1].p4 = msg;
    D_000EF140 = 0;
    func_000EA484();
    DrawSync(0);
    retry = 2;
    func_0007457C(current->b, 0x1080);
    busy = 1;
    func_0007457C(D_00097524->b, 0x1080);
    VSync(0);
    func_00077ED8(scr);
    D_000EFC00 = 0;
    if (!done) {
    h = D_000EE930;
    do {
        ev = func_00078E4C(1, &r[0], &r[1]);
        if (D_000EFC00 == 2)
            SetDispMask(1);
        switch (ev) {
        case 0:
            break;
        case -1:
            retry = 2;
            busy = 0;
            D_000ED7CC = 0;
            scr ^= 0x10;
            port ^= 1;
            func_00077ED8(scr);
            break;
        case 1:
            switch (r[0]) {
            case 1:
                switch (r[1]) {
                case 0:
                    if (h[port].c3) {
                        h[port].p4 = msg;
                        func_00078114(scr);
                        busy = 1;
                    }
                    break;
                case 3:
                    h[port].p4 = msg;
                    func_00078114(scr);
                    busy = 1;
                    break;
                default:
                    func_000E8DD4(port);
                    break;
                }
                break;
            case 2:
                st = r[1];
                switch (st) {
                case 0:
                    busy = 1;
                    func_000E7B2C(port);
                    break;
                case 3:
                    busy = 1;
                    func_000E7B2C(port);
                    break;
                case 4:
                    if (retry) {
                        func_00078114(scr);
                        retry--;
                    } else {
                        func_000E8DD4(port);
                        h[port].p4 = D_000E66C4;
                        h[port].s = st;
                    }
                    break;
                default:
                    func_000E8DD4(port);
                    break;
                }
                break;
            }
            break;
        }
        if (!busy) {
            func_000514E0();
            switch (D_000EF144) {
            case 0:
            case 1:
            case 2:
                done = func_000E96F8(D_00097538, buf, size);
                break;
            case 3:
            case 4:
            case 5:
            case 6:
                done = func_000E9CE0(D_00097538, buf, size);
                break;
            }
        }
        func_000E950C();
        func_000E91B8();
        func_000E90D8();
        D_000EFC00++;
    } while (!done);
    }
    DrawSync(0);
    VSync(0);
    func_00078E4C(0, &r[0], &r[1]);
    func_00077E98();
    func_0007457C(current->b, 0x1080);
    func_0007457C(D_00097524->b, 0x1080);
    DrawSync(0);
    func_000E90D8();
    func_000E90D8();
}
#undef RECT
#undef DB
#undef E
#undef H
#undef current
#undef D_000EE930
#undef D_00097524

/* ======== func_000E91B8 - MCARD (mcard.c); draft src/pilot/mcard/func_000E91B8.wip.c (PS1 match 89.93%) ========
 * Objdiff: same instructions except that ours cross-jumps the then-arm into the else-arm's identical func_0005D164 call.
 * Source: own decompilation (draft), verified as above; confidence: medium-high. */
#define E E_000E91B8
#define H H_000E91B8
#define P_TAG P_TAG_000E91B8
#define POLY_FT4 POLY_FT4_000E91B8
#define DB DB_000E91B8
#define current current_000E91B8
#define D_000EE930 D_000EE930_000E91B8
#define D_000EF8D0 D_000EF8D0_000E91B8
typedef struct { int a; short b; char pad[0x12]; char name[0x28]; unsigned char idx; unsigned char n; unsigned char f42; unsigned char f43; } E;
typedef struct { unsigned short s; char c2; char c3; void *p4; E e[15]; } H;
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
typedef struct { P_TAG tag; int w[9]; } POLY_FT4;
typedef struct { char *primptr; P_TAG **ot; int f8; int id; } DB;
extern DB *current __asm__("current");
extern H D_000EE930[] __asm__("D_000EE930");
extern POLY_FT4 D_000EF8D0[] __asm__("D_000EF8D0");
extern int D_000ED7CC;
extern int D_000ED7C8;
extern int D_000ED7C4;
extern int D_000EF13C;
extern int D_000EF140;
extern int D_000EF144;
extern char D_000ED784[];
extern char D_000E6864[];
extern char D_000E686C[];
extern char D_000E687C[];
extern void func_000EABBC(), func_0005C8F4(), func_000EB104(), func_0005D164();
extern int PrintString();

void func_000E91B8(void)
{
    int i;
    int x;

    D_000EF8D0[current->id].tag.addr = current->ot[12]->addr;
    current->ot[12]->addr = (unsigned int)&D_000EF8D0[current->id];
    (D_000EF8D0 + 2)[current->id].tag.addr = current->ot[12]->addr;
    current->ot[12]->addr = (unsigned int)&(D_000EF8D0 + 2)[current->id];
    if (D_000ED7CC == 0) {
        if (--D_000ED7C8 == 0) {
            D_000ED7C8 = 12;
            D_000ED7C4++;
        }
    }
    for (i = 0; i < 2; i++) {
        if ((D_000EE930[i].s & 2) || (D_000EE930[i].s & 4))
            func_000EABBC(i);
    }
    func_0005C8F4(D_000ED784, 0xBA);
    func_000EB104(0);
    func_000EB104(1);
    if ((D_000EE930[D_000EF13C].s & 2) && (D_000EE930[D_000EF13C].e[D_000EF140].f43 & 1)) {
        x = PrintString(D_000E6864, 0x18, 0xCE);
        if ((unsigned int)D_000EF144 < 3) {
            int f = D_000EF140 * 0x44;
            func_0005D164(((E *)((char *)D_000EE930[D_000EF13C].e + f))->name, x, 0xCE, 0x1000, 0xE00, 1);
        } else
            func_0005D164(D_000EE930[D_000EF13C].e[D_000EF140].name, x, 0xCE, 0x1000, 0xE00, 1);
    } else if (D_000EE930[D_000EF13C].s != 4 && D_000EE930[D_000EF13C].s != 1) {
        if (D_000EF144 < 3)
            PrintString(D_000E686C, 0x1E, 0xCE);
        else
            PrintString(D_000E687C, 0x1E, 0xCE);
    }
}
#undef E
#undef H
#undef P_TAG
#undef POLY_FT4
#undef DB
#undef current
#undef D_000EE930
#undef D_000EF8D0

/* ======== func_000E96F8 - MCARD (mcard.c); draft src/pilot/mcard/func_000E96F8.wip.c (PS1 match 99.76%) ========
 * Objdiff: same instructions modulo registers (D_000EE930+8 spelled D_000EE938 in the target).
 * Source: own decompilation (draft), verified as above; confidence: high. */
#define E E_000E96F8
#define H H_000E96F8
#define D_000EE930 D_000EE930_000E96F8
typedef struct { char fname[0x18]; char title[0x24]; unsigned int size; unsigned char idx; unsigned char n; unsigned char f42; unsigned char f43; } E;
typedef struct { unsigned short s; unsigned char c2; char c3; void *p4; E e[15]; } H;
typedef struct { unsigned char data[0x5E0]; int sum; } SAVE;
typedef struct { int a, b; } S8;
extern H D_000EE930[] __asm__("D_000EE930");
extern int D_000EF13C;
extern int D_000EF140;
extern int D_000EF144;
extern unsigned char *D_000ED86C;
extern int D_000967F0;
extern S8 D_00097640;
extern char D_000E676C[];
extern char D_000E6894[], D_000E68A8[], D_000E68E0[], D_000E68F4[], D_000E6930[], D_000E6944[];
extern char D_000E697C[], D_000E69B0[], D_000E69E0[], D_000E69F4[];
extern void VSync();
extern int func_00078E4C();
extern int func_00078798();
extern int func_0007B218(); /* strncmp */
extern void func_000EAE3C();
extern void func_000EC02C();
extern int func_000EC4DC();

static inline int load(int port, void *b, void *c, int d)
{
    int r[2];
    int n;
    int ret;

    r[0] = 0;
    r[1] = 0;
    n = 999;
    do {
        VSync(0);
        if (func_00078E4C(0, &r[0], &r[1]))
            break;
    } while (--n != -1);
    if (func_00078798(port << 4, b, c, 0, d) == 1) {
        n = 999;
        r[0] = 0;
        r[1] = 0;
        do {
            VSync(0);
            ret = func_00078E4C(0, &r[0], &r[1]);
            if (ret)
                break;
        } while (--n != -1);
        if (ret == 1)
            return 1;
    }
    func_000EAE3C(D_000E676C, 2);
    return 0;
}

int func_000E96F8(int pad, unsigned char *dst, unsigned int size)
{
    int ok;
    int i;
    unsigned char f;

    func_000EC02C(pad);
    if (pad & 0x10)
        return 1;
    if (pad & 0x40) {
        f = D_000EE930[D_000EF13C].e[D_000EF140].f43;
        if (!(f & 1))
            return 0;
        if (f & 2) {
            ok = 1;
            switch (D_000EF144) {
            case 0:
                if (func_0007B218(D_000EE930[D_000EF13C].e[D_000EF140].title, D_000E6894, 0x12)) {
                    func_000EAE3C(D_000E68A8, 2);
                    ok = 0;
                }
                break;
            case 2:
                if (func_0007B218(D_000EE930[D_000EF13C].e[D_000EF140].title, D_000E68E0, 0x12)) {
                    func_000EAE3C(D_000E68F4, 2);
                    ok = 0;
                }
                break;
            case 1:
                if (func_0007B218(D_000EE930[D_000EF13C].e[D_000EF140].title, D_000E6930, 0x12)) {
                    func_000EAE3C(D_000E6944, 2);
                    ok = 0;
                }
                break;
            }
            if (ok) {
                func_000EAE3C(D_000E697C, 0);
                if (D_000EE930[D_000EF13C].e[D_000EF140].size < size)
                    size = D_000EE930[D_000EF13C].e[D_000EF140].size;
                load(D_000EF13C, &D_000EE930[D_000EF13C].e[D_000EF140], D_000ED86C, size);
                if (D_000EF144 == 0) {
                    if (func_0007B218(D_000EE930[D_000EF13C].e[D_000EF140].title, D_000E6894, 0x12) == 0) {
                        D_000967F0 = 1;
                        D_000ED86C += 0x200;
                        if (func_000EC4DC(D_000ED86C, 0x5E0) != ((SAVE *)D_000ED86C)->sum) {
                            func_000EAE3C(D_000E676C, 2);
                            D_000967F0 = 0;
                            return 0;
                        }
                        *(SAVE *)dst = *(SAVE *)D_000ED86C;
                        func_000EAE3C(D_000E69B0, 1);
                        return 1;
                    } else {
                        func_000EAE3C(D_000E68A8, 2);
                        return 0;
                    }
                } else if (D_000EF144 == 2) {
                    if (func_0007B218(D_000EE930[D_000EF13C].e[D_000EF140].title, D_000E68E0, 0x12) == 0) {
                        D_000967F0 = 1;
                        D_000ED86C += 0x200;
                        if (func_000EC4DC(D_000ED86C, 0x5E0) != ((SAVE *)D_000ED86C)->sum) {
                            func_000EAE3C(D_000E676C, 2);
                            D_000967F0 = 0;
                            return 0;
                        }
                        *(SAVE *)dst = *(SAVE *)D_000ED86C;
                        func_000EAE3C(D_000E69B0, 1);
                        return 1;
                    } else {
                        func_000EAE3C(D_000E68F4, 2);
                        return 0;
                    }
                } else if (D_000EF144 == 1) {
                    if (func_0007B218(D_000EE930[D_000EF13C].e[D_000EF140].title, D_000E6930, 0x12) == 0) {
                        D_000967F0 = 1;
                        D_000ED86C += 0x200;
                        D_00097640 = *(S8 *)D_000ED86C;
                        D_000ED86C += 8;
                        for (i = 0; i < 0x3444; i++) {
                            *dst++ = *D_000ED86C;
                            D_000ED86C++;
                        }
                        func_000EAE3C(D_000E69B0, 1);
                        return 1;
                    } else {
                        func_000EAE3C(D_000E6944, 2);
                        return 0;
                    }
                } else {
                    func_0007B218(D_000EE930[D_000EF13C].e[D_000EF140].title, D_000E69E0, 0x12);
                    return 0;
                }
            }
        } else {
            func_000EAE3C(D_000E69F4, 2);
        }
    }
    return 0;
}
#undef E
#undef H
#undef D_000EE930

/* ======== func_001E06B8 - CRED (cred.c); draft src/pilot/cred/func_001E06B8.wip.c (PS1 match 94.16%) ========
 * Objdiff: same instructions modulo registers/order.
 * Source: own decompilation (draft), verified as above; confidence: medium-high. */
#define POLY_FT4 POLY_FT4_001E06B8
#define D_001E2790 D_001E2790_001E06B8
typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short x1, y1;
    unsigned char u1, v1;
    unsigned short tpage;
    short x2, y2;
    unsigned char u2, v2;
    unsigned short pad1;
    short x3, y3;
    unsigned char u3, v3;
    unsigned short pad2;
} POLY_FT4;

typedef struct {
    unsigned short type;
    unsigned char a, b;
    int p;
} CREDLINE;

extern int D_00097074;
extern int D_00097098;
extern char D_001E06A8[];
extern POLY_FT4 D_001E2790[] __asm__("D_001E2790");
extern int D_001E27F0, D_001F3870, D_001F3818;
extern int D_001E27F8[];
extern int D_001E1AB8, D_001F3808, D_001E1F88, D_001F3800, D_001E1AC4, D_001F3848, D_001F3820;
extern int D_001E27E8, D_001E1ABC, D_001E27E0, D_001F3860;
extern int D_001F3810[];
extern CREDLINE D_001E1ADC[];
extern int D_001E1AC8[];
extern int D_001E1F90[];
extern int Loadfile();
extern void func_00073AC4();
extern void DecDCTvlcBuild();

void func_001E06B8(void)
{
    POLY_FT4 *p;
    CREDLINE *l;
    int *y;
    int pos;
    int i;

    D_00097074 = 0;
    p = D_001E2790;
    p->clut = 0x703B;
    p->tpage = 0x3E;
    p->u0 = 0x80; p->v0 = 0x80; p->u1 = 0xD0; p->v1 = 0x80; p->u2 = 0x80; p->v2 = 0xBF; p->u3 = 0xD0; p->v3 = 0xBF;
    D_001E2790[1].tpage = 0x3E;
    D_001E2790[1].clut = 0x707B;
    p++;
    p->u0 = 0; p->v0 = 0xC0; p->u1 = 0x50; p->v1 = 0xC0; p->u2 = 0; p->v2 = 0xFF; p->u3 = 0x50; p->v3 = 0xFF;
    D_001E27F0 = D_00097098;
    D_001F3870 = D_00097098 += 0x2800;
    D_001F3818 = D_00097098 += 0x3000;
    D_00097098 += Loadfile(D_001E06A8, D_00097098);
    func_00073AC4(0);
    DecDCTvlcBuild(D_001E27F8);

    D_001E1AB8 = 0;
    D_001F3808 = 0;
    D_001E1F88 = 0;
    D_001F3800 = -1;
    D_001E1AC4 = 0;
    D_001F3848 = 0x200;
    D_001F3820 = -0x100;
    D_001E27E8 = 0;
    D_001E1ABC = 0;
    D_001F3810[1] = -1;
    D_001F3810[0] = -1;
    D_001E27E0 = 0;
    D_001F3860 = -2;

    pos = 0x80;
    i = 0;
    while (D_001E1ADC[i].type != 10) {
        D_001E1F90[i] = pos;
        pos += D_001E1AC8[D_001E1ADC[i].type];
        i++;
    }
}
#undef POLY_FT4
#undef D_001E2790

/* ======== func_001E0F1C - CRED (cred.c); draft src/pilot/cred/func_001E0F1C.wip.c (PS1 match 97.07%) ========
 * Objdiff: same instructions modulo order (the target materialises y - 0x80 earlier).
 * Source: own decompilation (draft), verified as above; confidence: high. */
#define P_TAG P_TAG_001E0F1C
#define DB DB_001E0F1C
#define POLY_FT4 POLY_FT4_001E0F1C
#define current current_001E0F1C
#define D_001E2790 D_001E2790_001E0F1C
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
typedef struct { char *primptr; unsigned long *ot; } DB;
typedef struct {
    unsigned long tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short x1, y1;
    unsigned char u1, v1;
    unsigned short tpage;
    short x2, y2;
    unsigned char u2, v2;
    unsigned short pad1;
    short x3, y3;
    unsigned char u3, v3;
    unsigned short pad2;
} POLY_FT4;

extern DB *current __asm__("current");
extern POLY_FT4 D_001E2790[] __asm__("D_001E2790");
extern int D_001F3810[];
extern void func_001E0BE8(int x, int y, int face, int shade);
extern void func_001E0B0C(int x, int y);

int func_001E0F1C(int face, int y)
{
    POLY_FT4 *p;
    int ty;
    int shade;
    int x;
    int c;
    int r;

    if (y + 0x48 < 0x30)
        return 0;
    ty = y - 0x80;
    shade = 0x30 - ty;
    if (shade < 0)
        shade = 0;
    else if (shade > 0x28)
        shade = 0x28;
    if (D_001F3810[face] == 9)
        func_001E0BE8(0x15E, y, face, shade);

    x = 0x15E;
    r = 0x1AE;
    c = 0x80;
    p = (POLY_FT4 *)current->primptr;
    *p = D_001E2790[face];
    ((P_TAG *)p)->len = 9;
    p->code = 0x2E;
    p->y3 = p->y2 = y + 0x40;
    p->r0 = c;
    p->g0 = c;
    p->b0 = c;
    p->x0 = x;
    p->y0 = y;
    p->x1 = r;
    p->y1 = y;
    p->x2 = x;
    p->x3 = r;
    ((P_TAG *)p)->addr = ((P_TAG *)&current->ot[3])->addr;
    ((P_TAG *)&current->ot[3])->addr = (unsigned long)p;
    current->primptr += sizeof(POLY_FT4);
    func_001E0B0C(0x15E, y);
    return 1;
}
#undef P_TAG
#undef DB
#undef POLY_FT4
#undef current
#undef D_001E2790

