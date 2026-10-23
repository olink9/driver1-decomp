/* 94.13 with fnscore defaults; MATCH 100.0 with --aspsx 2.69 (gp_allow_offset=False: the .comm RECTs
   D_00097550/D_00097574/D_000975A0 are $gp-relative at +0 but lui/%lo at +2..+6, as in the target). Same case as func_0002A5CC. */
/* no Driver 2 counterpart (texture page / CLUT loader, LoadPermanentTPages-like) */
typedef struct { short x, y, w, h; } RECT;
typedef struct { int id; int size; } TPENT;
typedef struct { short x, y; } XY;
typedef struct { int a; int b; } PAIR;

RECT D_00097550;
RECT D_00097574;
RECT D_000975A0;
extern int D_00097580;
extern long long D_00097584;

extern RECT D_000974E0;
extern RECT D_000976A4;
extern RECT D_000975F0;
extern RECT D_00097598;
extern long long D_00096804;
extern long long D_0009683C;
extern long long D_00097098;
extern long long D_000970F0;
extern long long D_000974B4;
extern TPENT *D_00097548;
extern int D_0009754C;
extern int D_00097560;
extern int D_0009757C;
extern int D_00097588;
extern int D_0009758C;
extern int D_00097590;
extern TPENT *D_00097594;
extern unsigned short texture_pages[];
extern unsigned short texture_cluts[][32];
extern unsigned char D_000BE278[];
extern unsigned char tpageslots[];
extern XY D_000BE708[];
extern XY D_000BE1BC[];

extern unsigned short GetTPage();
extern unsigned short func_0007A7F4();
extern void memset();
extern void loadsectors();
extern void func_000516E4();
extern void func_00056610();
extern void func_0005674C();

static inline void RegisterTPage(int id)
{
    int k = D_00097560;
    tpageslots[k] = id;
    D_000BE278[id] = k;
    D_000BE708[k].x = D_00097574.x;
    D_000BE708[k].y = D_00097574.y;
}


static inline void AdvanceClutRect(RECT *r)
{
    r->x += 16;
    if (r->x == 0x400) {
        r->x = 0x3C0;
        r->y++;
    }
}

void func_00051ADC(int *sector)
{
    int i, j;
    int n;
    int id;
    int k;
    RECT *tp;
    char *addr;

    for (i = 0; i < 128; i++)
        texture_pages[i] = GetTPage(0, 0, 0x3C0, 0);
    for (i = 0; i < 128; i++)
        for (j = 0; j < 32; j++)
            texture_cluts[i][j] = func_0007A7F4(0x3C0, 0x10);
    D_00097560 = 0;
    memset(D_000BE278, 0, 0x80);
    D_00097550.x = 0x3C0;
    D_00097550.y = 0x100;
    D_00097550.w = 0x10;
    D_00097550.h = 1;
    AdvanceClutRect(&D_00097550);
    D_000974E0 = D_00097550;
    D_00097574.x = 0x200;
    D_00097574.y = 0;
    D_00097574.w = 0x40;
    D_00097574.h = 0x100;
    D_000975A0.x = 0x3C0;
    D_000975A0.y = 0x100;
    D_000975A0.w = 0x10;
    D_000975A0.h = 1;
    D_00097550.y += 3;
    D_000976A4 = D_00097550;
    AdvanceClutRect(&D_00097550);
    if (*(int *)&D_00096804 == 0) {
        func_00056610(*(int *)&D_000970F0, 0);
        func_0005674C(&D_00097550);
    }
    n = 0;
    addr = *(char **)&D_00097098;
    for (i = 0; i < D_0009758C; i++)
        n += (D_00097594[i].size + 2047) / 2048;
    loadsectors(addr, *sector, n);
    *sector += n;
    for (i = 0; i < D_0009758C; i++) {
        id = D_00097594[i].id;
        n = (D_00097594[i].size + 2047) & ~2047;
        RegisterTPage(id);
        func_000516E4(&D_00097574, &D_00097550, id, addr);
        addr += n;
        D_00097560++;
    }
    addr = *(char **)&D_00097098;
    if (*(int *)&D_00096804 == 0) {
        for (i = 0; i < *(int *)&D_0009683C * 2; i++) {
            n = (D_00097548[i].size + 2047) / 2048;
            *sector += n;
        }
        for (; i < (*(int *)&D_0009683C + 1) * 2; i++) {
            id = D_00097548[i].id;
            n = (D_00097548[i].size + 2047) / 2048;
            if (i == *(int *)&D_0009683C * 2) {
                D_000975F0 = D_00097574;
                D_00097598 = D_00097550;
                D_0009754C = id;
                *(int *)&D_000974B4 = id;
                D_00097588 = id;
                D_00097590 = D_00097560;
                D_00097580 = *sector;
                *(int *)&D_00097584 = n;
            }
            loadsectors(addr, *sector, n);
            RegisterTPage(id);
            func_000516E4(&D_00097574, &D_00097550, id, addr);
            D_00097560++;
            *sector += n;
        }
    }
    if (D_00097550.x != 0x3C0) {
        D_00097550.x = 0x3C0;
        D_00097550.y++;
    }
    for (i = D_00097560; i < 15; i++) {
        tpageslots[i] = 0xFF;
        tp = &D_00097574;
        D_000BE1BC[i].x = D_00097550.x;
        D_000BE1BC[i].y = D_00097550.y;
        D_000BE708[i].x = D_00097574.x;
        D_000BE708[i].y = D_00097574.y;
        D_00097574.x += 0x40;
        if (D_00097574.x == 0x400) {
            D_00097574.x = 0x200;
            D_00097574.y += 0x100;
            if (D_00097574.y == 0x200) {
                D_0009757C = 100;
                tp->y = 0x100;
            }
        }
        if (D_0009757C) {
            tp->x = 0x380;
            tp->y = 0x100;
        }
        D_00097550.y += 8;
    }
}
