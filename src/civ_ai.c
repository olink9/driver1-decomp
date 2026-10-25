/* civ_ai.c: 58 functions func_0001250C..func_0001AC18 (0x1250C-0x1AE7C)
 * Merged from src/pilot by build/tu-civ_ai/nsgen.py (on top of tools/mktu.py's parser); byte-identical.
 *
 * File boundary (the plan's row 2 "ai.c", 0x1250C-0x1B380, split in two):
 *  - This object owns .sdata 0x96770-0x96783 and .sbss 0x96FBC-0x96FF7 (the first slice of each: .sbss
 *    starts at 0x96FBC, right after the libraries' last .sdata word 0x96FB8, not at 0x97000) and .data
 *    0x85314-0x853D7 (D_0008531C start table, D_0008539C state-handler table). Every function here that
 *    reaches them does so through $gp, every other file absolutely; their users run from func_000143CC to
 *    func_0001AC18, and the address order of first use interleaves (0x96FD4 by func_000143CC sits after
 *    0x96FC0 by func_000154A8; 0x96FE8 by func_0001A9BC sits before 0x96FEC by func_0001A30C), so
 *    0x143CC-0x1AE7C cannot be cut. No small global is reached both ways inside 0x1250C-0x1AE7C.
 *  - func_0001250C..func_000142CC own no data; they are AI helpers reached only from this cluster
 *    (func_00013D88 is called by func_0001A30C, func_0001250C by func_00016CE4, func_00013A90 by
 *    func_00016F0C), so they stay at the head of this file.
 *  - 0x1AE7C-0x1B380 is ai.c: its first function is D2 AI.C's StoreGameFlags (D2 line 275).
 *  - Name: civilian-car AI (road/junction following, civ slots D_00098950, spawn/remove), the D1
 *    counterpart of D2 CIV_AI.C. D2 COP_AI-like code is not identified here (cop logic seems to live
 *    in the felony area; D_0008539C also points at func_00065424 there).
 *
 * Step 4: the file defines its own data, each section in address order and ONE block (.data 0x85314-0x853D7,
 *    .sdata 0x96770-0x96783, .sbss 0x96FBC-0x96FF7). D_00096778 (glaunch.c), D_00096FC8 (felony/mission) and
 *    D_00096FE4 (main.c) are only used elsewhere; D_000853C8, D_00096FDC/E0 are unreferenced fillers.
 *    D_00085314 (int[2]) needs __attribute__((section(".data"))): at 8 bytes cc1 puts it in .sdata.
 *    D_0008530C stays an extern label (= D_00085314 - 8, indexed with f9 & 3 = 2..3).
 *    .sbss starts at 0x96FBC in slus_008.42.yaml too (inside the file's zero padding up to 0x87800; objcopy --pad-to).
 *    maspsx abs= lists every other file's small global the original reaches with %hi/%lo (was `long long`).
 *
 * Conflicts (mktu listed 47) were first resolved by build/tu-civ_ai/nsgen2.py + cfg2.py with ~50 per-function
 * `__asm__` views; Step 3 removed every one of them (byte-identical at each step):
 *  - Records: CIV_SLOT (D_00098900/D_00098950, 0x50; D_00098950 is D_00098900[1]), CAR_DATA (car_data and
 *    D_0009B230 = car_data[1], 0x18C) and PURSUER (D_000C4690/D_000C4E38, 0x7A8) come from include/structs.h.
 *    CAR_DATA spellings: 0xC8 = pos (old x/y/z, t[3], XZ), where.t[0]/t[2] = 0x18/0x20, model = 0x2,
 *    angle.vy = 0xEA, direction = 0xF8, wheelspin = 0x100, wheel_speed = 0x104, speed = 0x108,
 *    damage[0..1] = 0x114/0x118, totalDamage = 0x17C. D_0009B2F8 = &D_0009B230[i].pos; car_data int[]
 *    offsets 0x245C/0x2464 = car_data[23].pos.vx/vz. PURSUER (old PED): active = 0x78C, car = 0x7A4.
 *  - File-local records: ROAD_INFO (D_0009E744, 0x14: lanes[4] + pos[4][2]), CELL_OBJ (D_00099180, 0xC) and
 *    REC_99120 (D_00099120, 0x38, a civ group: x/y/z, f10[4], counts at 0x24-0x26, dir 0x27, slot-id list
 *    at 0x29; func_0001943C/19314/19AF4/19DAC/1A024 take it). Other local types are per-function shapes
 *    of tables reached through pointers (D_00097040/44/4C) and stay under per-section names.
 *  - Functions: one unprototyped `RET f();` per callee. Char parameters: func_00014FC4, func_000154A8 and
 *    func_000197E0 have K&R definitions (unprototyped callers pass ints; same callee bytes). Narrow results:
 *    a cast at the use, `(unsigned char)func_00017A50()`, `(unsigned char)func_000192D0()`,
 *    `(short)func_00021558()`, `(unsigned short)func_00021820()`; func_00017DE8 calls the uchar-returning
 *    func_00017780 through `((int (*)(CIV_SLOT *, VECTOR *, int *, unsigned char))func_00017780)` because
 *    its original keeps the full v0 (a plain call adds an andi).
 *  - Globals: D_00096898 is read as `*(unsigned char *)&`, D_000988C8 is short[], D_00099B08 SXY[],
 *    camera_position VECTOR; func_000147C4 reads rcossin_tbl as 4-byte-aligned pairs `((E *)rcossin_tbl)[i]`
 *    (one lw per entry).
 *  - The static inline ClearMem copies (4 functions) and other inline helpers keep per-section names.
 *  - #include "common.h" (func_000148C4) is dropped: nothing from it was used.
 */
/* maspsx: abs=D_00097044,D_00097040,D_00097050,D_00097054,D_00097038,D_000967A8,D_00097374,D_00096804,D_00096834,D_000973B4,D_000973B8,D_00096990,current,D_0009704C,D_0009D500,D_0009D508,D_0009B16C,D_0009B174,gLightsOn,D_00097764,D_00096884,D_00096870,gNight,D_00097058,CameraCnt,D_00096898 */
#include "structs.h"

/* ---- file-local records (fields this file touches) ---- */
typedef struct {
    unsigned char lanes[4]; /* 0x0 lane count per direction */
    short pos[4][2]; /* 0x4 per-direction x, z (cells) */
} ROAD_INFO;
typedef struct {
    short model; /* 0x0 */
    char flag; /* 0x2 */
    char pad3[0x4 - 0x3];
    int x; /* 0x4 */
    int z; /* 0x8 */
} CELL_OBJ;

typedef struct {
    int x, y, z; /* 0x0 */
    int fC; /* 0xC */
    short f10[4]; /* 0x10 */
    unsigned char f18; /* 0x18 */
    char pad19;
    short f1A, f1C, f1E; /* 0x1A */
    char pad20[0x24 - 0x20];
    unsigned char f24; /* 0x24 */
    unsigned char f25; /* 0x25 */
    unsigned char f26; /* 0x26 entries in list */
    unsigned char f27; /* 0x27 dir */
    unsigned char f28; /* 0x28 */
    unsigned char list[0x38 - 0x29]; /* 0x29 civ slot ids */
} REC_99120;

/* ---- shared declarations (identical in every pilot) ---- */
extern short D_000978B0[];
typedef struct { int nx, ny, nz, d; } PLANE;
extern unsigned char D_000988B0[];
typedef struct {
    int pad0[2];
    short s8;
    short padA;
    int padC;
} Node;
extern int D_00097044;
typedef struct {
    short a;
    short h;
    int b;
} REC;
typedef struct {
    int v;
    unsigned char b;
    char pad[3];
} FOUND;
extern int D_00097040;
typedef struct {
    char p0[3];
    char f3;
    char p4[4];
    REC rec[1];
} PATH;
extern int D_00099AE0[];
extern char D_000C71E7[];
extern void func_0004C8F8();
extern void func_000650E4();
typedef struct {
    char pad[3];
    unsigned char x3;
    char pad4[0x2C - 4];
} NODE_b095;
typedef struct {
    int a, b, c, d, e, f;
} SEG_b095;
extern int D_00097050;
extern int D_00097054;
extern int D_00097038;
extern int D_000852FC[];
extern int D_0008530C[];
typedef struct {
    int x;
    int z;
} CELLPOS;
extern int D_000967A8;
extern int D_00097374;
extern int D_0009E714[];
extern short *D_0009E724[];
extern int *D_0009D8F4[];
extern CELLPOS D_00099720[];
typedef struct { short x, y; } E __attribute__((aligned(4)));
typedef struct { char p0[0x12C]; int f12c; char p1[0x2A4 - 0x130]; int a4, a8, ac; char p3[0x2DE - 0x2B0]; unsigned char b2de; char p4[0x100]; } B_t;
typedef struct { int x, y, z, w; } V3;
typedef struct { int x, y, z; } V3s;
extern B_t D_000C6F04;
extern int D_00096804;
extern int D_00096834;
extern int MapHeight();
extern int D_0009784C[];
extern int D_000973B4;
extern int D_000973B8;
extern int D_00096990;
extern int current;
extern int func_000398B0();
extern void func_0003A698();
extern void func_00043E8C();
extern void func_00046000();
extern void func_000439E4();
extern void func_00043D0C();
typedef struct { unsigned char a, b, c, d, e, f; char pad[74]; } T;
extern void func_0004AD94();
typedef struct {
    char pad[10];
    unsigned char flags;
    char pad2[5];
} ROADINFO;
typedef struct {
    char p0[0xA];
    unsigned char xA;
    unsigned char xB;
    int pC;
} ROAD2;
typedef struct {
    int x;
    int z;
    int p8;
    short sC;
    short pE;
    short s10;
    short p12;
} INFO;
extern int D_0009704C;
extern int D_0009D500;
extern int D_0009D508;
extern int D_0009B16C;
extern int D_0009B174;
extern int gLightsOn;
extern int D_00097764;
extern int func_00063AA8();
extern int func_000635B0();
extern int func_00063B14();
typedef struct {
    int x, y, z, pad;
} VEC;
extern int func_00058A00();
extern void func_000589B4();
extern char D_00099158[];
extern unsigned char D_00099AF0[];
extern void func_00021D54();
extern void func_00021DE8();
extern void func_00021F5C();
typedef struct {
    short a;
    short road;
    unsigned short flags;
    short b;
} JEXIT;
typedef struct {
    char pad[11];
    unsigned char dir;
    char padc[4];
} ROAD16;
extern int D_00096884;
extern int func_000612FC();
extern void func_00063AB4();
typedef struct {
    int a;
    int b;
} PAIR;
extern void func_0004D740();
typedef struct {
    int a;
    int b;
    int pad8;
    short c;
    short padE;
    short d;
    short pad12;
    int pad14;
} SEG_b062;
typedef struct {
    int x;
    int z;
} XZ_b062;
typedef struct { int x, z; } POS2;
extern int D_00096870;
extern short func_00020DB4();
typedef struct {
    int x;
    int z;
} POS;
extern int func_0004A9F4();
extern short D_0008EAAA[];
extern void func_00064FE0();
extern int gNight;
extern unsigned char D_000C6F02[];
extern int func_00020AE4();
extern int sprintf();
typedef struct {
    short s0; short a; short b; char pad[10];
} RD;
extern int D_00097058;
extern CIV_SLOT D_00098950[];
extern CIV_SLOT D_00098900[];
extern CAR_DATA D_0009B230[];
extern CAR_DATA car_data[];
extern PURSUER D_000C4690[];
extern PURSUER D_000C4E38[];
extern ROAD_INFO D_0009E744[];
extern CELL_OBJ D_00099180[];
extern SXY D_00099B08[];
extern VECTOR camera_position;
extern short D_000988C8[];
extern volatile int CameraCnt;
extern int D_00096898;   /* majority spelling; other pilots keep a view */
extern REC_99120 D_00099120;
extern int func_00021558();   /* majority spelling; other pilots keep a view */
extern int func_00021820();
extern int func_00063B74();
extern void ClearMem();
extern short rcossin_tbl[];   /* majority spelling; other pilots keep a view */

/* ---- the file's own data (Step 4), each section in address order ---- */
/* .data 0x85314-0x853D7 */
/* 8 bytes: cc1 -G8 would put it in .sdata; the original has it in .data */
int D_00085314[2] __attribute__((section(".data"))) = { 0xE74, 0xED8 };   /* read as D_0008530C[f9 & 3] (f9 & 3 = 2..3) */
V3 D_0008531C[8] = {
    { 0x20B70, 0xBB8, 0x66B84, 0 }, { 0xFFFE7381, 0x3E7, 0x20617, 0 },
    { 0xFFFFD6FC, 0xBB8, 0xFFFFA980, 0 }, { 0xFFF83163, 0, 0xFFFCF1DF, 0 },
    { 0x24548, 0x1813, 0xFFFE4A80, 0 }, { 0xFFFD67F0, 0x1813, 0x58228, 0 },
    { 0xFFFFD6FC, 0xFFFFE7ED, 0xFFFFA980, 0 }, { 0xFFFFDCDD, 0xFFFFE7ED, 0xF8A7, 0 },
};
void func_00017D6C(); void func_00017D74(); void func_00017DE8(); void func_000181B4(); void func_00065424();
void func_00018478(); void func_000184DC(); void func_00018540(); void func_00018548(); void func_00018550();
void (*D_0008539C[11])() = {
    func_00017D6C, func_00017D74, func_00017DE8, func_000181B4, func_00065424, func_00018478,
    func_000184DC, func_00018540, func_00018548, func_00018550, func_00017D6C,
};
int D_000853C8[4] = { 0, 0x400, 0x800, 0xC00 };   /* unreferenced */

/* .sdata 0x96770-0x96783 */
int D_00096770 = 0;
int D_00096774 = 0;
int D_00096778 = 1;   /* used only by glaunch.c */
int D_0009677C = 0;
int D_00096780 = 0;

/* .sbss 0x96FBC-0x96FF7 */
char D_00096FBC;
CAR_DATA *D_00096FC0;
unsigned char D_00096FC4;
int D_00096FC8;   /* used by felony.c, mission.c */
int D_00096FCC;
int D_00096FD0;
int D_00096FD4;
int D_00096FD8;
int D_00096FDC;   /* unreferenced */
int D_00096FE0;   /* unreferenced */
int D_00096FE4;   /* used only by main.c */
short D_00096FE8;
int D_00096FEC;
int D_00096FF0;
int D_00096FF4;

/* ---- functions (unprototyped: callers were compiled against several spellings) ---- */
void func_0001250C();
int func_00012E98();
int func_000130D4();
int func_000138A0();
int func_00013A90();
int func_00013D88();
int func_000142CC();
void func_000143CC();
unsigned char func_00015338();
int func_00016760();
void func_00016B24();
int func_00016F0C();
int func_00017A64();
void func_00017C50();
int func_00018100();
void func_00018618();
void func_00018900();
void func_00018C74();
int func_00018F44();
void func_0001943C();
int func_00019AF4();
int func_00019DAC();
int func_00019E88();
void func_0001A008();
void func_0001A024();
int func_0001A1BC();
int func_0001AC18();
extern void func_0001B4A4();
extern void func_0001B720();
extern void func_0001D608();
extern void func_00020A64();
extern void func_0003EB0C();
extern int func_0004FFE8();
extern void memset();

/* ---------------- func_0001250C (src/pilot/func_0001250C.c) ---------------- */
void func_0001250C(void)
{
	short *out = D_000978B0;
	int i = 0;
	short *tab = rcossin_tbl;
	do {
int s = tab[((i * 4) & 0xFFF) * 2]; int c = tab[((i * 4 + 0x400) & 0xFFF) * 2];
		int a = c*180, na = -a, b = s*450, a2 = s*180, na2 = -a2, c2 = c*450, nb = -b, nc2 = -c2;
		i++;
		out[0] = (na - b) >> 12;
		out[1] = (na2 + c2) >> 12;
		out += 2;
		out[0] = (a - b) >> 12;
		out[1] = (a2 + c2) >> 12;
		out += 2;
		out[0] = (na - nb) >> 12;
		out[1] = (na2 + nc2) >> 12;
		out += 2;
		out[0] = (a - nb) >> 12;
		out[1] = (a2 + nc2) >> 12;
		out += 2;
	} while (i < 256);
}

/* ---------------- func_0001262C (src/pilot/func_0001262C.c) ---------------- */
/* func_0001262C: does any other car (or the player) overlap the 4-plane box ahead of civ car cp; result cached in D_000988B0 (no Driver 2 counterpart) */
int func_0001262C(CIV_SLOT *cp)
{
    PLANE pl[4];
    int id;
    int done;
    int range;
    int slot;
    int dist;
    int i;
    int px, pz;
    int x, z;
    int ang;
    short *box;
    int x0, z0, x1, z1, x2, z2, x3, z3;
    int s, c;
    int d0, d1, d2, d3;
    int dd;
    int ox, oz;
    int other;
    unsigned char gm;

    done = 0;
    slot = cp->id;
    id = slot;
    if ((*(int *)&CameraCnt ^ slot) & 1)
        return D_000988B0[slot];

    if (cp->f1 == 3 && cp->f34 != 0) {
        range = 0xB4;
        dist = cp->f38 + 400;
    } else {
        range = 300;
        dist = (cp->f38 << 4) + 0x6D6;
    }

    cp = D_00098900;
    px = D_0009B230[id].pos.vx;
    pz = D_0009B230[id].pos.vz;

    for (i = 0; i < 23; i++, cp++) {
        if (i == 0) {
            gm = *(unsigned char *)&D_00096898;
            if (gm == 3 || gm == 6 || (unsigned char)(gm - 1) < 2)
                continue;
        }
        if (cp->f1 == 0 && i != 0)
            continue;
        other = cp->id;
        if (other == id && i != 0)
            continue;
        if (i != 0) {
            x = D_0009B230[other].pos.vx;
            z = D_0009B230[other].pos.vz;
        } else {
            x = car_data[0].pos.vx;
            z = car_data[0].pos.vz;
        }
        if (x - px >= 0) {
            if (x - px < dist) ; else continue;
        } else if (px - x >= dist)
            continue;
        if (z - pz >= 0) {
            if (z - pz < dist) ; else continue;
        } else if (pz - z >= dist)
            continue;

        if (!done) {
            ang = -D_0009B230[id].direction & 0xFFF;
            c = rcossin_tbl[ang * 2 + 1];
            s = rcossin_tbl[ang * 2];
            ox = (-range * c) >> 12;
            oz = (-range * s) >> 12;
            ox += px;
            oz += pz;
            pl[0].d = -c * ox + -s * oz;
            ox = (range * c) >> 12;
            oz = (range * s) >> 12;
            pl[1].d = c * (ox + px) + s * (oz + pz);
            ox = (dist * -s) >> 12;
            oz = (dist * c) >> 12;
            pl[2].d = -s * (ox + px) + c * (oz + pz);
            pl[3].d = s * px + -c * pz;
            pl[0].nx = -c;
            pl[0].nz = -s;
            pl[1].nx = c;
            pl[1].nz = s;
            pl[2].nx = -s;
            pl[2].nz = c;
            pl[3].nx = s;
            pl[3].nz = -c;
            done = 1;
        }

        ang = (i != 0 ? -D_0009B230[other].direction : -car_data[0].direction) & 0xFFF;
        box = &D_000978B0[((ang & 0x3FF) / 4) * 8];

        switch (ang & 0xC00) {
        case 0:
            x0 = box[0]; z0 = box[1];
            x1 = box[2]; z1 = box[3];
            x2 = box[4]; z2 = box[5];
            x3 = box[6]; z3 = box[7];
            break;
        case 0x400:
            z0 = box[0]; x0 = -box[1];
            z1 = box[2]; x1 = -box[3];
            z2 = box[4]; x2 = -box[5];
            z3 = box[6]; x3 = -box[7];
            break;
        case 0x800:
            x0 = -box[0]; z0 = -box[1];
            x1 = -box[2]; z1 = -box[3];
            x2 = -box[4]; z2 = -box[5];
            x3 = -box[6]; z3 = -box[7];
            break;
        default:
            x0 = box[1]; z0 = -box[0];
            x1 = box[3]; z1 = -box[2];
            x2 = box[5]; z2 = -box[4];
            x3 = box[7]; z3 = -box[6];
            break;
        }
        x0 += x; z0 += z;
        x1 += x; z1 += z;
        x2 += x; z2 += z;
        x3 += x; z3 += z;

        dd = pl[0].d;
        d0 = pl[0].nx * x0 + pl[0].nz * z0;
        d1 = pl[0].nx * x1 + pl[0].nz * z1;
        d2 = pl[0].nx * x2 + pl[0].nz * z2;
        d3 = pl[0].nx * x3 + pl[0].nz * z3;
        if (dd < d0 && dd < d1 && dd < d2 && dd < d3)
            continue;
        dd = pl[1].d;
        d0 = pl[1].nx * x0 + pl[1].nz * z0;
        d1 = pl[1].nx * x1 + pl[1].nz * z1;
        d2 = pl[1].nx * x2 + pl[1].nz * z2;
        d3 = pl[1].nx * x3 + pl[1].nz * z3;
        if (dd < d0 && dd < d1 && dd < d2 && dd < d3)
            continue;
        dd = pl[2].d;
        d0 = pl[2].nx * x0 + pl[2].nz * z0;
        d1 = pl[2].nx * x1 + pl[2].nz * z1;
        d2 = pl[2].nx * x2 + pl[2].nz * z2;
        d3 = pl[2].nx * x3 + pl[2].nz * z3;
        if (dd < d0 && dd < d1 && dd < d2 && dd < d3)
            continue;
        dd = pl[3].d;
        d0 = pl[3].nx * x0 + pl[3].nz * z0;
        d1 = pl[3].nx * x1 + pl[3].nz * z1;
        d2 = pl[3].nx * x2 + pl[3].nz * z2;
        d3 = pl[3].nx * x3 + pl[3].nz * z3;
        if (dd < d0 && dd < d1 && dd < d2 && dd < d3)
            continue;
        D_000988B0[slot] = 1;
        return 1;
    }
    D_000988B0[slot] = 0;
    return 0;
}

/* ---------------- func_00012E98 (src/pilot/func_00012E98.c) ---------------- */
#define check1 check1__func_00012E98
#define check2 check2__func_00012E98
#define check3 check3__func_00012E98
/* no Driver 2 counterpart */
#define nodes (*(Node **)&D_00097044)
static inline int check1(CIV_SLOT *a)
{
    int i;
    CIV_SLOT *p;
    unsigned char id = a->id;
    int road = a->f20;

    for (p = D_00098950, i = 0; i < 22; i++, p++) {
        if (p->f8 && p->id != id && p->f3 && p->f20 == road)
            return 0;
    }
    return 1;
}
static inline int check2(CIV_SLOT *a, int idx)
{
    int i;
    CIV_SLOT *p;
    int id = a->id;
    int road = a->f20;
    unsigned char lane = a->f30;

    for (p = D_00098950, i = 0; i < 22; i++, p++) {
        if (!p->f8 || p->id == id)
            continue;
        if (p->f28 == idx) {
            if (p->f24 == lane)
                return 1;
        } else if (p->f3 && p->f20 == road && p->f2C == idx) {
            return 1;
        }
    }
    return 0;
}
static inline int check3(CIV_SLOT *a)
{
    int i;
    CIV_SLOT *p;
    int id = a->id;
    int idx = a->f2C;
    unsigned char lane = a->f30;
    int lim;

    p = D_00098950;
    if (p[id].f34)
        lim = 6000;
    else
        lim = 3000;
    for (i = 0; i < 22; i++, p++) {
        if (p->f1 >= 2 && p->id != id
            && p->f28 == idx && p->f24 == lane
            && p->f1C < lim && p->f38 < 30)
            return 1;
    }
    return 0;
}
int func_00012E98(CIV_SLOT *a, int mode)
{
    if (mode == 1 && !check1(a))
        return 1;
    if (nodes[a->f2C].s8 >= 4 || !check2(a, a->f2C))
        return check3(a);
    return 1;
}
#undef nodes
#undef check1
#undef check2
#undef check3

/* ---------------- func_000130D4 (src/pilot/func_000130D4.c) ---------------- */
#define collect collect__func_000130D4
/* func_000130D4 - no Driver 2 counterpart (car AI: junction clear check) */
static inline int collect(CIV_SLOT *c, FOUND *f, int road, int lane, int jn)
{
    CIV_SLOT *e;
    int n = 0;
    int id = c->id;

    for (e = D_00098950; e < D_00098950 + 22; e++) {
        if (e->f8 && e->id != id) {
            if (e->f28 == road) {
                if (e->f24 == lane || e->f30 == lane) {
                    f->v = e->f18;
                    f->b = e->id;
                    n++;
                    f++;
                }
            } else if (e->f3 && e->f20 == jn) {
                f->v = -1;
                f->b = e->id | 0x80;
                n++;
                f++;
            }
        }
    }
    return n;
}
int func_000130D4(CIV_SLOT *c)
{
    FOUND l1[22];
    FOUND l2[22];
    REC *r;
    int n;
    int b;
    int jn;
    int prev;
    int next;
    int road1;
    int road2;

    jn = c->f20;
    r = (REC *)(*(char **)&D_00097040 + jn * 44 + 8);
    prev = (c->f24 - 1) & 3;
    next = (c->f24 + 1) & 3;
    road1 = r[prev].h;
    road2 = r[next].h;

    if (c->f34 == 0) {
        if (road1 != -1) {
            FOUND *f;
            FOUND *g;
            FOUND *end;
            n = collect(c, l1, road1, next, jn);
            g = l1 + n;
            f = l1;
            end = g;
            for (; f < end; f++) {
                if (f->v < 0x4074) {
                    b = f->b;
                    if (b & 0x80)
                        goto fail;
                    if (D_00098950[b].f38 && D_00098950[b].f2C != c->f28)
                        return 0;
                }
            }
        }
        if (road2 != -1) {
            FOUND *f;
            FOUND *g;
            FOUND *end;
            n = collect(c, l2, road2, prev, jn);
            g = l2 + n;
            f = l2;
            end = g;
            for (; f < end; f++) {
                if (f->v < 0x4074) {
                    if (D_00098950[f->b].f38)
                        return 0;
                }
            }
        }
        goto tail;
    } else if (c->f34 > 0) {
        if (road1 != -1) {
            FOUND *f;
            FOUND *g;
            FOUND *end;
            n = collect(c, l1, road1, next, jn);
            g = l1 + n;
            f = l1;
            end = g;
            for (; f < end; f++) {
                if (f->v < 0x4074) {
                    b = f->b;
                    if (b & 0x80)
                        goto fail;
                    if (D_00098950[b].f2C == road2 && D_00098950[b].f38)
                        return 0;
                }
            }
        }
    tail:
        if (func_00012E98(c, 1) == 0)
            goto ret1;
    fail:
        return 0;
    } else if (c->f34 < 0) {
        if (road1 != -1) {
            FOUND *f;
            FOUND *g;
            FOUND *end;
            n = collect(c, l1, road1, next, jn);
            g = l1 + n;
            f = l1;
            end = g;
            for (; f < end; f++) {
                if (f->v < 0x4074) {
                    b = f->b;
                    if (b & 0x80)
                        goto fail;
                    if (D_00098950[b].f38 && D_00098950[b].f2C != c->f28)
                        return 0;
                }
            }
        }
        if (road2 != -1) {
            FOUND *f;
            FOUND *g;
            FOUND *end;
            n = collect(c, l2, road2, prev, jn);
            g = l2 + n;
            f = l2;
            end = g;
            for (; f < end; f++) {
                if (f->v < 0x4074) {
                    b = f->b;
                    if (b & 0x80)
                        goto fail;
                    if (D_00098950[b].f2C != r[c->f24].h && D_00098950[b].f38)
                        return 0;
                }
            }
        }
        if (func_00012E98(c, 1))
            return 0;
    }
ret1:
    return 1;
}
#undef collect

/* ---------------- func_000138A0 (src/pilot/func_000138A0.c) ---------------- */
#define CAR CAR__func_000138A0
typedef struct {
    unsigned char id;
    char p1[0x1F];
    int f20;
    int f24;
} CAR;
int func_000138A0(CAR *c)
{
    FOUND list[22];
    CIV_SLOT *e;
    FOUND *f;
    FOUND *end;
    FOUND *g;
    FOUND *h;
    PATH *path;
    REC *r;
    int n;
    int t0;
    int t1;
    int t2;
    int t3;

    t1 = c->f20;
    path = (PATH *)(*(char **)&D_00097040 + t1 * 44);
    r = (REC *)((char *)path + 8);
    if (path->f3 != 0)
        return 1;
    t2 = r[c->f24].h;
    t0 = (c->f24 + 2) & 3;
    if (t2 != -1) {
        f = list;
        t3 = c->id;
        n = 0;
        for (e = D_00098950; e < D_00098950 + 22; e++) {
            if (e->f8 && e->id != t3) {
                if (e->f28 == t2) {
                    if (e->f24 == t0 || e->f30 == t0) {
                        f->v = e->f18;
                        f->b = e->id;
                        n++;
                        f++;
                    }
                } else if (e->f3 && e->f20 == t1) {
                    f->v = -1;
                    f->b = e->id | 0x80;
                    n++;
                    f++;
                }
            }
        }
        g = &list[0];
        end = g + n;
        for (h = g; h < end; h++) {
            int v = h->v;
            int b;
            if (v < 0x4074) {
                b = h->b;
                if (b & 0x80)
                    return 0;
                if (D_00098950[b].f34 != -1) {
                    if (D_00098950[b].f34 == 0)
                        return 0;
                    if (v < 0x2904)
                        return 0;
                }
            }
        }
    }
    return func_00012E98(c, 1) == 0;
}
#undef CAR

/* ---------------- func_00013A90 (src/pilot/func_00013A90.c) ---------------- */
int func_00013A90(int a, int b, int c, int d)
{
    int i;
    CIV_SLOT *e;
    D_00099AE0[3] = 0;
    D_00099AE0[2] = 0;
    D_00099AE0[1] = 0;
    D_00099AE0[0] = 0;
    e = D_00098950;
    for (i = 0; i < 22; i++, e++) {
        if (e->f1) {
            if (e->f28 == a)
                D_00099AE0[0]++;
            else if (e->f28 == b)
                D_00099AE0[1]++;
            else if (e->f28 == c)
                D_00099AE0[2]++;
            else if (e->f28 == d)
                D_00099AE0[3]++;
        }
    }
    return D_00099AE0[0] + D_00099AE0[1] + D_00099AE0[2] + D_00099AE0[3];
}

/* ---------------- func_00013B80 (src/pilot/func_00013B80.c) ---------------- */
void func_00013B80(int a, int b, int c)
{
    CIV_SLOT *p = &D_00098900[a];
    CIV_SLOT *q = &D_00098900[b];

    if ((*(unsigned char *)&D_00096898) == 3 && ((a == 0 && b == 23) || (b == 0 && a == 23))) {
        D_000C71E7[0] = 15;
        return;
    }
    if (b == 0 || a == 0) {
        if (p->f7) func_0004C8F8();
        p->f2 = 1;
        p->f8 = 0;
        if (p->f5) {
            if (p->f1 == 4) return;
            if (c >= 16) func_000650E4(D_000988C8, 5, 0x1000);
            else func_000650E4(D_000988C8, 5, (c << 24) >> 16);
        } else {
            if (c >= 32) func_000650E4(D_000988C8, 4, 0x1000);
            else func_000650E4(D_000988C8, 4, (c << 23) >> 16);
        }
    } else {
        if (p->f2 == 1 || q->f2 == 1 || p->f5 || q->f5) {
            if (p->f7) func_0004C8F8();
            if (q->f7) func_0004C8F8(b);
            p->f2 = 1; p->f8 = 0;
            q->f2 = 1; q->f8 = 0;
        }
        if (p->f1 == 1) { p->f2 = 1; p->f8 = 0; }
        if (q->f1 == 1) { q->f2 = 1; q->f8 = 0; }
    }
}

/* ---------------- func_00013D88 (src/pilot/func_00013D88.c) ---------------- */
#define reroute reroute__func_00013D88
/* func_00013D88: car AI per-frame state update (no Driver 2 counterpart) */
static inline void reroute(CIV_SLOT *p)
{
    SEG_b095 seg;
    VECTOR *px = &D_0009B230[p->id].pos;
    func_00021820(px->vx, px->vz, p->f24, &seg);
    func_00016F0C(p, &seg);
}
int func_00013D88(CIV_SLOT *p, CAR_DATA *c)
{
    int mode;
    SEG_b095 seg;
    VECTOR *px;
    int d;
    unsigned char v;
    int r;

    c->fF0 = 0;
    p->f15 = 0;
    if (p->f2 == 1 || p->f1 == 1)
        return 0;
    if (p->f1 == 9)
        return 0;
    if (p->f3 == 0)
        p->f3C = func_000142CC(p);
    if ((p->f17 & 8) && c->speed == 0 && p->f3C == 0) {
        p->f17 &= ~8;
        p->f1 = 1;
        p->f8 = 0;
        p->fB = 0xFF;
        return 0;
    }
    if (p->f18 < 3000 && (p->f17 & 2)) {
        if (!(p->f17 & 0x20))
            p->f40 = p->f44 = 0x7FFFFFFF;
        px = &D_0009B230[p->id].pos;
        p->f17 &= ~2;
        func_00021820(px->vx, px->vz, p->f24, &seg);
        func_00016F0C(p, &seg);
    }

    if ((*(NODE_b095 **)&D_00097040)[p->f20].x3 == 0) {
        if (p->f34 == 1) { mode = 1; goto sw; }
        if (p->f12 & 1) { mode = 1; goto sw; }
        if (p->f12 & 4) {
            if (p->f24 & 1)
                v = *(unsigned char *)&D_00097050;
            else
                v = *(unsigned char *)&D_00097054;
            if (v == 3) { mode = 3; goto sw; }
            if (p->f3 == 0) { mode = 2; goto sw; }
        }
    }
    mode = 0;
sw:
    switch (mode) {
    case 1:
        if (p->f3 == 0 && p->f18 < 650 && func_000130D4(p)) {
            p->f3 = mode;
            if (p->f34)
                p->f3C = 60;
            else
                p->f3C = 130;
        }
        break;
    case 0:
    case 3:
        if (p->f3 == 0 && p->f18 < 650 && (*(int *)&CameraCnt & 3) == p->f24) {
            mode = p->f34;
            switch (mode) {
            case -1:
                r = func_000138A0(p);
                if (r == 1) {
                    p->f3C = 60;
                    p->f3 = r;
                    break;
                }
                goto retry;
            case 1:
                if (func_00012E98(p, 0) == 0) {
                    p->f3C = 60;
                    p->f3 = mode;
                    break;
                }
                goto retry;
            case 0:
                if (func_00012E98(p, 0) == 0) {
                    p->f3C = 130;
                    p->f3 = 1;
                    break;
                }
            retry:
                if ((p->f12 & 4) && p->f4 == *(unsigned char *)&D_00097038 + 1) {
                    px = &D_0009B230[p->id].pos;
                    func_00021820(px->vx, px->vz, p->f24, &seg);
                    func_00016F0C(p, &seg);
                }
                break;
            }
        }
        break;
    case 2:
        if (p->f1 != 3 && (p->f34 != 0 || p->f3 == 0)) {
            p->f3 = 0;
            if (p->f3C > 90)
                p->f3C = 90;
            if (p->f18 < 650) {
                p->f3C = 0;
                p->f17 &= 0xDE;
            }
        }
        break;
    }

    mode = p->f3C;
    if ((p->id & 3) == (*(int *)&CameraCnt & 3))
        p->f16 = func_0001262C(p);
    if (p->f16)
        mode = 0;
    d = mode - p->f38;
    if (d > 0) {
        if (d > 32)
            d = 32;
        c->fF0 = (d * 5000) / 32;
    } else if (d < 0) {
        if (d < -32)
            d = -32;
        if (d < -20)
            p->f15 = 1;
        c->fF0 = (d * 7000) / 32;
    }
    return 0;
}
#undef reroute

/* ---------------- func_000142CC (src/pilot/func_000142CC.c) ---------------- */
int func_000142CC(CIV_SLOT *o)
{
    if (o->f17 & 8) {
        o->f3C -= 16;
        if (o->f3C < 0) o->f3C = 0;
    } else {
        if (o->f18 < 0x28A || func_00018F44(o, 1, 0) != -1) {
            o->f3C = 0;
            return 0;
        }
        if (o->f18 >= D_0008530C[o->f9 & 3])
            goto tab;
        o->f3C = o->f18 / 24;
        if (o->f3C < 5) o->f3C = 5;
    }
    return o->f3C;
tab:
    return D_000852FC[o->f9 & 3];
}

/* ---------------- func_000143CC (src/pilot/func_000143CC.c) ---------------- */
#define cellmid cellmid__func_000143CC
/* func_000143CC - no Driver 2 counterpart: gather the objects of the cells around the player (offset table D_00099B08) */
static inline int cellmid(int c)
{
    return c * 1500 + 750;
}
void func_000143CC(void)
{
    short *ofs;
    CELL_OBJ *obj;
    CELLPOS *pos;
    int cx, cz;
    int i;
    int x, z;
    int idx;
    int val;
    int cell;
    int tx, tz;
    int qx, qz;
    unsigned short uval;
    int wx;

    ofs = (short *)D_00099B08;
    obj = D_00099180;
    pos = D_00099720;
    D_00096FD4 = 0;
    D_00096FD8 = 0;
    if (D_00096780 != 0 || D_0009677C != 0) {
        cx = (car_data[23].pos.vx + D_0009E714[0] * 750) / 1500;
        cz = (D_0009E714[1] * 750 - car_data[23].pos.vz) / 1500;
    } else {
        cx = (car_data[0].pos.vx + D_0009E714[0] * 750) / 1500;
        cz = (D_0009E714[1] * 750 - car_data[0].pos.vz) / 1500;
    }
    for (i = 0; i < 120; i++, ofs += 2) {
        x = cx + ofs[0];
        z = cz + ofs[1];
        if (x < 0 || x >= D_0009E714[0] || z < 0 || z >= D_0009E714[1])
            continue;
        tz = z;
        tx = x;
        qz = tz / 60;
        qx = tx / 60;
        x = tx - qx * 60;
        z = tz - qz * 60;
        qz = *(int *)&D_00097374 - qz;
        idx = (qx & 1) + ((qz & 1) << 1);
        val = ((short (*)[60])D_0009E724[idx])[z][x];
        uval = ((unsigned short (*)[60])D_0009E724[idx])[z][x];
        if (val == -0x8000) {
            pos->x = cellmid(tx) - D_0009E714[2];
            D_00096FD8++;
            pos->z = D_0009E714[3] - tz * 1500;
            pos++;
            continue;
        }
        cell = D_0009D8F4[idx][z * 60 + x];
        obj->flag = cell >> 30;
        if (cell & 2) {
            obj->model = -1;
        } else {
            if (val < 0)
                continue;
            if (*(int *)&D_000967A8 < val)
                continue;
            if (*(short *)(*(char **)&D_00097044 + val * 16 + 8) < 4)
                continue;
        }
        if (!(cell & 2))
            obj->model = uval;
        obj->x = cellmid(tx) - D_0009E714[2];
        D_00096FD4++;
        obj->z = D_0009E714[3] - tz * 1500;
        obj++;
    }
}
#undef cellmid

/* ---------------- func_000147C4 (src/pilot/func_000147C4.c) ---------------- */
void func_000147C4(void)
{
    SXY last, v;
    E e;
    SXY *out = D_00099B08;
    int i;
    memset(&last, 0, 4);
    for (i = 0; i < 0x1000; i += 0x20) {
        e = ((E *)rcossin_tbl)[i & 0xFFF];
        v.x = e.x;
        v.y = e.y;
        v.x = (v.x * 19 + 0x800) >> 12;
        v.y = (v.y * 19 + 0x800) >> 12;
        if (last.y != v.y || last.x != v.x) {
            out->x = v.x;
            out->y = v.y;
            out++;
            last.x = v.x;
            last.y = v.y;
        }
    }
}

/* ---------------- func_000148C4 (src/pilot/func_000148C4.c) ---------------- */
#define G04 (*(int *)&D_00096804)
#define G34 (*(int *)&D_00096834)
/* Driver 2 name: unknown */
void func_000148C4(void)
{
    char pad[16];
    V3s v;
    CAR_DATA *p;
    memset(&car_data[0], 0, 0x18C);
    if (D_000C6F04.b2de != 0 && G04 == 0) {
        int a = D_000C6F04.a4, b = D_000C6F04.a8, c = D_000C6F04.ac;
        car_data[0].model = 0;
        car_data[0].pos.vx = a;
        car_data[0].pos.vy = b;
        car_data[0].pos.vz = c;
        func_00020A64(&car_data[0]);
        v.x = car_data[0].pos.vx; v.y = car_data[0].pos.vy; v.z = car_data[0].pos.vz;
        func_0001B4A4(&car_data[0], &v);
        car_data[0].direction = D_000C6F04.f12c;
        func_0001B720(&car_data[0], 0);
    } else {
        v.x = D_0008531C[G34].x; v.y = D_0008531C[G34].y; v.z = D_0008531C[G34].z;
        p = &car_data[0];
        if (G34 == 2) p->direction = 0; else p->direction = 0x400;
        p->pos.vx = D_0008531C[G34].x;
        p->pos.vy = D_0008531C[G34].y;
        p->pos.vz = D_0008531C[G34].z;
        p->pos.vy = MapHeight(&p->pos.vx) + 0xA4;
        v.y = p->pos.vy;
        func_0001B4A4(p, &v);
        func_0001B720(&car_data[0], 0);
        car_data[0].model = 0;
    }
    camera_position.vx = car_data[0].pos.vx;
    camera_position.vy = -car_data[0].pos.vy - 200;
    camera_position.vz = car_data[0].pos.vz;
}
#undef G04
#undef G34

/* ---------------- func_00014A8C (src/pilot/func_00014A8C.c) ---------------- */
/* func_00014A8C - DrawAllTheCars-like (sort visible cars by distance, then draw)
 * run3-r4: MATCH. Final fix: the car pointer `c` of the sort loop is REUSED for the per-entry car in the
 * draw loop (`c = &D_0009B230[e->f0]`, no separate `car`) - gives the split `lui v0; addiu a0,v0` base
 * (lesson r4-02), and `c = D_0009B230;` written before `e = D_00098950;` fixes the prologue order.
 * Earlier: car loop biv c++ with call arg D_0009B230[i].t, ENT biv e++, separate draw counter m.
 */
#define current (*(DB **)&current)
void func_00014A8C(void)
{
    unsigned char order[32];
    unsigned char n;
    int i, j, k, m;
    int camx, camz;
    int d, dx, dz;
    int t;
    unsigned char o;
    int idx;
    CIV_SLOT *e;
    int *cur;
    char *p;
    CAR_DATA *c;
    int r;
    int a1;

    c = D_0009B230;
    e = D_00098950;
    *(int *)&D_000973B4 = 0;
    camx = camera_position.vx;
    camz = camera_position.vz;
    n = 0;
    for (i = 0; i < 25; i++, e++, c++) {
        if (e->f1 != 0 && func_000398B0(&D_0009B230[i].pos) == 1) {
            dx = camx - c->pos.vx;
            if (dx < 0) dx = c->pos.vx - camx;
            dz = camz - c->pos.vz;
            if (dz < 0) dz = c->pos.vz - camz;
            d = dx + dz;
            if (d < 27000) {
                D_0009784C[n] = d;
                order[n++] = i;
            }
        }
    }
    if (n == 0)
        return;
    *(int *)&D_000973B8 = 0;
    for (j = 1; j < n; j++) {
        o = order[j];
        t = D_0009784C[j];
        for (k = j; k != 0 && t < D_0009784C[k - 1]; k--) {
            order[k] = order[k - 1];
            D_0009784C[k] = D_0009784C[k - 1];
        }
        order[k] = o;
        D_0009784C[k] = t;
    }
    for (m = 0; m < n; m++) {
        idx = order[m];
        e = &D_00098950[idx];
        cur = *(int **)&current;
        p = current->primptr - 0x1E000; r = current->primtab - p - 3000;
        *(int *)&D_00096990 = e->f11;
        if (r < 0)
            return;
        r -= (n - m - 1) << 11;
        if (r < 4000)
            *(int *)&D_000973B8 = 1;
        idx++;
        func_0003A698(idx, e->f10);
        *(int *)&D_00096990 = 0;
        func_00043E8C(idx);
        if (e->f10 == 1 && ((e->f5 != 0 && e->f1 == 4) || e->f1 == 9))
            func_00046000(idx);
        if (e->f17 & 1) {
            c = &D_0009B230[e->id];
            a1 = e->f24;
            if (a1 & 1) {
                if (e->f44 != 0x7FFFFFFF) {
                    if (e->f44 < c->where.t[2]) {
                        if (a1 == 3) func_000439E4(idx, 0);
                        else func_000439E4(idx, 1);
                    } else if (c->where.t[2] < e->f44) {
                        if (a1 == 1) func_000439E4(idx, 0);
                        else func_000439E4(idx, 1);
                    }
                }
            } else {
                if (e->f40 != 0x7FFFFFFF) {
                    if (e->f40 < c->where.t[0]) {
                        if (a1 == 0) func_000439E4(idx, 0);
                        else func_000439E4(idx, 1);
                    } else if (c->where.t[0] < e->f40) {
                        if (a1 == 2) func_000439E4(idx, 0);
                        else func_000439E4(idx, 1);
                    }
                }
            }
        } else if (e->f1 != 1 && ((e->f28 & 0x4000) || e->f1 == 3 || e->f18 < 5000)) {
            switch (e->f34) {
            case -1: func_000439E4(idx, 0); break;
            case 1: func_000439E4(idx, 1); break;
            }
        }
        if (e->f17 & 0x10)
            func_000439E4(idx, 0);
        if (e->f17 & 0x20)
            func_000439E4(idx, 1);
        if (e->f15)
            func_00043D0C(idx);
    }
}
#undef current

/* ---------------- func_00014F28 (src/pilot/func_00014F28.c) ---------------- */
void func_00014F28(void) {
    int i;
    CIV_SLOT *p = D_00098950;
    for (i = 0; i < 22; i++, p++) {
        if (p->f1 != 0) {
            if (p->f2 == 1 || p->f5 != 0)
                func_0004AD94(p->id + 1);
        }
    }
}

/* ---------------- func_00014FC4 (src/pilot/func_00014FC4.c) ---------------- */
void func_00014FC4(j, dir, flip, mode, lane, out)
    int j, dir, flip, mode;
    unsigned char lane;
    int *out;
{
    int n;
    int max;
    int li;
    int off;
    int x;
    int z;
    int r;

    n = D_0009E744[j].lanes[dir];
    max = n - 1;
    if (mode == 1 || lane == 0xfe) {
        li = max;
    } else if (lane == 0xff && max != -1) {
        li = func_0004FFE8(j) % n;
    } else {
        if (max < lane)
            lane = max;
        if (flip)
            lane = max - lane;
        li = lane;
    }
    off = li * 1500;

    switch (dir) {
    case 0:
    default:
        x = D_0009E744[j].pos[0][0] * 1500 + off;
        z = D_0009E744[j].pos[0][1] * 1500;
        break;
    case 2:
        x = D_0009E744[j].pos[2][0] * 1500 - off;
        z = D_0009E744[j].pos[2][1] * 1500;
        break;
    case 1:
        x = D_0009E744[j].pos[1][0] * 1500;
        z = D_0009E744[j].pos[1][1] * 1500 - off;
        break;
    case 3:
        x = D_0009E744[j].pos[3][0] * 1500;
        z = D_0009E744[j].pos[3][1] * 1500 + off;
        break;
    }

    if (mode != 1) {
        if (lane == 0xfe)
            r = 300;
        else if ((*(ROADINFO **)&D_00097044)[j].flags & 1)
            r = (unsigned char)func_0004FFE8(j);
        else
            r = (func_0004FFE8(j) & 0x1ff) - 256;

        switch (dir) {
        case 0:
            x -= r;
            break;
        case 2:
            x += r;
            break;
        case 1:
            z += r;
            break;
        case 3:
            z -= r;
            break;
        }
    }
    out[0] = x;
    out[1] = z;
}

/* ---------------- func_00015338 (src/pilot/func_00015338.c) ---------------- */
unsigned char func_00015338(CIV_SLOT *p)
{
    CAR_DATA *c = &D_0009B230[p->id];
    int w = p->f28;
    ROAD_INFO *r;
    int d, a3, q;
    unsigned char t0;

    a3 = 0;
    if (w & 0xC000) return 0;
    { ROAD_INFO *b = D_0009E744; r = &b[w]; }
    d = p->f24;
    t0 = r->lanes[d];
    if (t0 == 1) return 0;
    switch (d) {
    case 0: q = (c->pos.vx + 750) / 1500; a3 = q - r->pos[0][0]; break;
    case 1: q = (c->pos.vz - 750) / 1500; a3 = r->pos[1][1] - q; break;
    case 2: q = (c->pos.vx - 750) / 1500; a3 = r->pos[2][0] - q; break;
    case 3: q = (c->pos.vz + 750) / 1500; a3 = q - r->pos[3][1]; break;
    }
    return t0 - a3 - 1;
}

/* ---------------- func_0001548C (src/pilot/func_0001548C.c) ---------------- */
void func_0001548C(int a, char *p)
{
    *(int *)(p + 0x78C) = 1;
    *(int *)(p + 0x790) = 0;
    *(int *)(p + 0x794) = 1;
    *(int *)(p + 0x40) = 0;
    *(int *)(p + 0x7A4) = a;
}

/* ---------------- func_000154A8 (src/pilot/func_000154A8.c) ---------------- */
#define ClearMem ClearMem__func_000154A8
#define Nearest Nearest__func_000154A8
#define ROAD ROAD__func_000154A8
/* no Driver 2 counterpart (civ car creation) */
typedef struct {
    int x0;
    int x4;
    int p8, pC;
    unsigned char x10;
    char p11[3];
} ROAD;
#define roads (*(ROAD **)&D_0009704C)
#define roads2 (*(ROAD2 **)&D_00097044)
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
static inline int Nearest(int x, int z)
{
    CIV_SLOT *o = D_00098950;
    int tx, tz, i, n, min, dx, dz, d;

    tx = x / 1500;
    tz = z / 1500;
    if (D_00096780 || D_0009677C)
        n = 24;
    else
        n = 22;
    min = 1024;
    for (i = 0; i < n; i++, o++) {
        if (o->f1) {
            d = o->x - tx;
            if (d < 0)
                d = tx - o->x;
            dz = o->z - tz;
            if (dz < 0)
                dz = tz - o->z;
            d += dz;
            if (d < min)
                min = d;
        }
    }
    return min;
}
int func_000154A8(id, road, flag, x, z)
    int id, road;
    unsigned char flag;
    int x, z;
{
    CIV_SLOT *cp;
    CAR_DATA *car;
    int ty, mode;
    int dir, odir;
    int pos[2];
    INFO info;
    int pos2[3];
    CIV_SLOT *o;
    int tx, tz, i, cnt, min, dx, dz;
    int one;

    ty = 0;
    mode = ty;
    cp = &D_00098950[id];
    car = &D_0009B230[id];
    ClearMem((unsigned char *)cp, 0x50);
    cp->id = id;
    cp->f1 = 0;
    if (road != -1)
        ty = roads[road].x10;
    ClearMem((unsigned char *)car, 0x18C);
    car->id = id + 1;

    if (road != -1) {
        if (ty == 1) {
            unsigned char odir;
            if (D_00096780 || D_0009677C) {
                dir = 3;
                if (roads[road].x0 * 1500 < *(int *)&D_0009D500)
                    dir = 1;
            } else {
                dir = 3;
                if (roads[road].x0 * 1500 < *(int *)&D_0009B16C)
                    dir = 1;
            }
            if (dir == 1)
                odir = 3;
            else
                odir = 1;
            switch (roads2[road].xB) {
            case 0x81:
                flag = 1;
                odir = flag;
                break;
            case 0x83:
                flag = 1;
                odir = 3;
                break;
            }
            if (flag == 1)
                cp->f24 = odir;
            else
                cp->f24 = dir;
        } else {
            if (D_00096780 || D_0009677C)
                dir = (-roads[road].x4 * 1500 < *(int *)&D_0009D508) << 1;
            else
                dir = (-roads[road].x4 * 1500 < *(int *)&D_0009B174) << 1;
            odir = (dir == 0) << 1;
            switch (roads2[road].xB) {
            case 0x80:
                flag = 1;
                odir = 0;
                break;
            case 0x82:
                flag = 1;
                odir = 2;
                break;
            }
            if (flag == 1)
                cp->f24 = odir;
            else
                cp->f24 = dir;
        }
    }

    cp->f7 = 0;
    cp->f8 = 0;
    cp->f2 = 0;
    cp->f28 = road;
    if (road == -1)
        mode = 1;
    else if (roads2[road].xA & 1) {
        if (!(func_0004FFE8(id) & 7))
            mode = 2;
    }

    if (mode) {
        if (road != -1) {
            func_00014FC4(road, cp->f24, cp->f34, 1, 0xFF, pos);
            if (cp->f24 & 1)
                z = pos[1];
            else
                x = pos[0];
            switch (cp->f24) {
            case 0:
                x += 470;
                break;
            case 2:
                x -= 470;
                break;
            case 1:
                z -= 470;
                break;
            case 3:
                z += 470;
                break;
            }
            if (Nearest(x, z) < 2) {
                func_00017C50(id);
                return 0;
            }
            func_00021820(x, z, cp->f24, &info);
            if (info.z < 4500) {
                func_00017C50(id);
                return 0;
            }
            if (info.x < 3000) {
                func_00017C50(id);
                return 0;
            }
        } else {
            if (Nearest(x, z) < 2) {
                func_00017C50(id);
                return 0;
            }
        }
        if (func_0004FFE8(id) & 3)
            cp->f7 = 1;
        cp->f1 = 1;
        cp->fA = 0;
        cp->fB = 0xFF;
        cp->x = D_0009B230[cp->id].pos.vx / 1500;
        cp->z = D_0009B230[cp->id].pos.vz / 1500;
    } else {
        cp->f1 = 2;
        cp->f8 = 1;
        if (func_00021820(x, z, cp->f24, &info)) {
            func_00017C50(id);
            return 0;
        }
        if (!func_00016F0C(cp, &info)) {
            func_00017C50(cp->id);
            return 0;
        }
        func_00014FC4(cp->f28, cp->f24, cp->f34, 0, 0xFF, pos2);
        if (cp->f24 & 1)
            z = pos2[1];
        else
            x = pos2[0];
        o = D_00098950;
        tx = x / 1500;
        tz = z / 1500;
        cnt = 0;
        min = 1024;
        for (i = 0; i < 22; i++, o++) {
            if (o->f1) {
                dx = o->x - tx >= 0 ? o->x - tx : tx - o->x;
                dz = o->z - tz >= 0 ? o->z - tz : tz - o->z;
                if (dx < 5 && dz < 5) {
                    dx += dz;
                    if (dx < min)
                        min = dx;
                    cnt++;
                }
            }
        }
        pos2[2] = min;
        if (cnt >= 11 || (*(unsigned char *)&D_00096898 != 3 && min < 6)) {
            func_00017C50(id);
            return 0;
        }
        cp->f18 = info.x;
        cp->f1C = info.z;
        cp->f20 = info.sC;
        cp->f12 = info.s10;
    }

    car->pos.vx = x;
    car->pos.vz = z;
    func_00020A64(car);
    if (road != -1)
        car->direction = cp->f24 << 10;
    else
        car->direction = flag << 10;
    cp->f40 = cp->f44 = 0x7FFFFFFF;
    cp->f38 = 0;
    cp->f3C = 0x82;
    if (mode)
        cp->f10 = *(int *)&CameraCnt % 2 + 2;
    else
        cp->f10 = func_0004FFE8(id) % 2 + 2;
    if (cp->f10 == 3 && *(int *)&D_00096834 == 5)
        cp->f11 = 5;
    else
        cp->f11 = *(int *)&CameraCnt % 4 + 1;
    cp->f5 = 0;
    if (D_00096FF4 == 1 && !mode && func_00063B74(cp, car)) {
        D_00096FF4 = 0;
        D_00096FC0 = car;
    }
    one = 1;
    if (cp->f5) {
        if (D_00096774 == one) {
            if (D_00096770 == one && (func_0004FFE8(id) & 1)) {
                cp->f10 = one;
                cp->f11 = one;
            } else {
                cp->f10 = 2;
                cp->f11 = 1;
            }
        } else {
            cp->f10 = one;
            cp->f11 = one;
        }
    }
    car->model = cp->f10;
    func_0003EB0C(id + 1, cp->f10);
    func_0001B720(car, 1);
    cp->x = D_0009B230[cp->id].pos.vx / 1500;
    cp->z = D_0009B230[cp->id].pos.vz / 1500;
    cp->f9 = func_00015338(cp);
    cp->f17 = 2;
    if (*(int *)&gLightsOn)
        cp->f17 = 0x42;
    return 1;
}
#undef roads
#undef roads2
#undef ClearMem
#undef Nearest
#undef ROAD

/* ---------------- func_00016094 (src/pilot/func_00016094.c) ---------------- */
#define ClearMem ClearMem__func_00016094
#define VECTOR VECTOR__func_00016094
typedef struct { int vx, vy, vz, pad; } VECTOR;
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
CAR_DATA *func_00016094(int model, int a1, int a2, int x, int z)
{
    VECTOR v2;
    VECTOR v;
    int slot;
    CIV_SLOT *e;
    CAR_DATA *cp = 0;

    slot = func_00017A64();
    if (slot != -1) {
        cp = &D_0009B230[slot];
        e = &D_00098950[slot];
        ClearMem((unsigned char *)cp, sizeof(CAR_DATA));
        ClearMem((unsigned char *)e, sizeof(CIV_SLOT));
        e->id = slot;
        cp->id = slot + 1;
        e->f1 = 1;
        e->f7 = 1;
        e->f28 = -1;
        e->f8 = 0;
        e->f2 = 0;
        cp->pos.vx = x;
        cp->pos.vy = 200;
        cp->pos.vz = z;
        func_00020A64(cp);
        memset(&v, 0, 16);
        v.vx = cp->pos.vx;
        v.vy = cp->pos.vy;
        v.vz = cp->pos.vz;
        v2 = v;
        func_0001B4A4(cp, &v2);
        cp->direction = a2;
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
        e->x = D_0009B230[e->id].pos.vx / 1500;
        e->z = D_0009B230[e->id].pos.vz / 1500;
        e->f17 = 0;
    }
    return cp;
}
#undef ClearMem
#undef VECTOR

/* ---------------- func_0001638C (src/pilot/func_0001638C.c) ---------------- */
#define ClearMem ClearMem__func_0001638C
#define SEG SEG__func_0001638C
/* func_0001638C - no Driver 2 counterpart: spawn a car */
typedef struct {
    int a[6];
} SEG;
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
CAR_DATA *func_0001638C(int dir, int x, int z)
{
    SEG seg;
    CAR_DATA *cp = 0;
    CIV_SLOT *e;
    PURSUER *pd;
    int slot;
    int r;
    int n;
    unsigned char one;

    if ((unsigned char)func_00017A50() == 0 || func_00063AA8() == 0)
        goto end;
    slot = func_00017A64();
    cp = &D_0009B230[slot];
    e = &D_00098950[slot];
    ClearMem((unsigned char *)cp, sizeof(CAR_DATA));
    ClearMem((unsigned char *)e, sizeof(CIV_SLOT));
    e->id = slot;
    cp->id = slot + 1;
    e->f7 = 0;
    e->f8 = 0;
    e->f2 = 0;
    e->f28 = -1;
    one = 1;
    e->f8 = one;
    func_00021820(x, z, func_000635B0(dir), &seg);
    func_00016F0C(e, &seg);
    cp->pos.vx = x;
    cp->pos.vz = z;
    func_00020A64(cp);
    cp->direction = dir;
    e->f44 = 0x7FFFFFFF;
    e->f40 = 0x7FFFFFFF;
    e->f38 = 0;
    e->f3C = 0x82;
    if (D_00096774 != 0 && (D_00096770 & func_0004FFE8(x)) == 0) {
        r = func_0004FFE8(slot);
        e->f10 = r % 2 + 2;
        e->f11 = r % 4 + 1;
    } else {
        e->f10 = one;
    }
    n = func_00063B14();
    e->f5 = n + 1;
    pd = &D_000C4E38[n];
    pd->active = 1;
    pd->f790 = 0;
    pd->f794 = 1;
    pd->f40 = 0;
    pd->car = cp;
    pd->f794 = 0;
    (*(int *)&D_00097764)++;
    e->f1 = 4;
    cp->model = e->f10;
    func_0003EB0C(slot + 1, e->f10);
    func_0001B720(cp, 1);
    e->x = D_0009B230[e->id].pos.vx / 1500;
    e->z = D_0009B230[e->id].pos.vz / 1500;
    e->f9 = func_00015338(e);
    e->f17 = 2;
    if (*(int *)&gLightsOn != 0)
        e->f17 = 0x42;
end:
    return cp;
}
#undef ClearMem
#undef SEG

/* ---------------- func_00016760 (src/pilot/func_00016760.c) ---------------- */
#define ClearMem ClearMem__func_00016760
/* func_00016760 - no Driver 2 counterpart: create a car at a position (sibling of func_0001638C) */
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
int func_00016760(int x, int y, int z, int dir, int flag)
{
    VEC v;
    CAR_DATA *cp;
    CIV_SLOT *e;
    int slot;
    int r;

    memset(&v, 0, 16);
    v.x = x;
    v.y = y;
    v.z = z;
    slot = func_00017A64();
    if (slot == -1)
        return slot;
    e = &D_00098950[slot];
    cp = &D_0009B230[slot];
    ClearMem((unsigned char *)e, sizeof(CIV_SLOT));
    ClearMem((unsigned char *)cp, sizeof(CAR_DATA));
    cp->id = slot + 1;
    e->id = slot;
    e->f7 = 0;
    e->f8 = 0;
    e->f2 = 0;
    e->f28 = -1;
    if ((func_0004FFE8(slot) & 1) && flag == 0)
        e->f7 = 1;
    e->fA = 0;
    e->fB = 0xFF;
    cp->pos.vx = x;
    cp->pos.vy = y;
    cp->pos.vz = z;
    cp->direction = dir;
    func_00020A64(cp);
    v.y = cp->pos.vy;
    e->f44 = 0x7FFFFFFF;
    e->f40 = 0x7FFFFFFF;
    e->f38 = 0;
    if (flag) {
        if (D_00096770) {
            e->f10 = 1;
            e->f11 = 1;
        } else {
            r = func_0004FFE8(slot);
            e->f10 = r % 2 + 2;
            e->f11 = r % 4 + 1;
        }
        e->f1 = 9;
        e->f2 = 1;
        e->f3C = 0;
        cp->fFC = 1;
    } else {
        r = func_0004FFE8(slot);
        e->f10 = r % 2 + 2;
        e->f11 = r % 4 + 1;
        e->f3C = 0x82;
        e->f1 = 1;
    }
    e->f5 = 0;
    cp->model = e->f10;
    func_0003EB0C(slot + 1, e->f10);
    func_0001B4A4(cp, &v);
    func_0001B720(cp, 0);
    e->x = D_0009B230[e->id].pos.vx / 1500;
    e->z = D_0009B230[e->id].pos.vz / 1500;
    return slot;
}
#undef ClearMem

/* ---------------- func_00016B24 (src/pilot/func_00016B24.c) ---------------- */
/* Driver 2 name: unknown */
void func_00016B24(void)
{
    int local;
    unsigned char idx;
    CELL_OBJ *e;
    int a0;

    if (D_00096FD4 == 0)
        return;
    if (D_00096780 == 0) {
        func_0001943C(&D_00099120);
        local = func_00017A64();
        if (local == -1)
            return;
    }
    idx = func_0004FFE8(local) % D_00096FD4;
    if (D_00096780 != 0) {
        idx = func_00058A00(idx, &local);
        if (idx == 0xFF)
            return;
        if (idx == 0xFE) {
            CIV_SLOT *sl = &D_00098950[local];
            sl->f1 = 0;
            sl->f8 = 0;
            return;
        }
    }
    e = &D_00099180[idx];
    a0 = 0;
    if (*(int *)&D_00096834 != 2 && *(int *)&D_00096834 != 6) {
        if (e->model != -1)
            a0 = func_000154A8(local, e->model, func_0004FFE8(local) & 1, e->x, e->z);
        else
            a0 = func_000154A8(local, -1, e->flag & 3, e->x, e->z);
    }
    if (D_0009677C && a0)
        func_000589B4(idx, (unsigned char)local);
}

/* ---------------- func_00016CE4 (src/pilot/func_00016CE4.c) ---------------- */
#define ClearMemInline ClearMemInline__func_00016CE4
static inline void ClearMemInline(unsigned char *p, int n)
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
void func_00016CE4(void)
{
    int i;
    REC_99120 *b;
    CIV_SLOT *e;
    unsigned char *q;

    ClearMemInline((unsigned char *)D_00098950, 2000);
    ClearMemInline((unsigned char *)D_000C4E38, 0x1EA0);
    ClearMem(D_00099158, 0x28);
    if (*(int *)&D_00096834 != 2) {
        b = &D_00099120;
        ClearMemInline((unsigned char *)b, 0x38);
        b->f1E = 450;
        b->f1A = -1;
        b->f1C = -1;
        D_00096FE8 = -1;
        D_00096FF4 = 0;
        func_00021DE8();
        func_00021F5C();
        func_00021D54();
    }
    func_0001250C();
    D_00096FD0 = 0x10101010;
    D_00096FF0 = 99;
    D_00096FC4 = 0;
    i = 0;
    q = D_00099AF0;
    e = D_00098950;
    do {
        *q = i;
        e->f8 = 0;
        e->id = i;
        i++;
        q++;
        e->f1 = 0;
        e->f5 = 0;
        e++;
    } while (i < 22);
    D_00096FBC = 0;
}
#undef ClearMemInline

/* ---------------- func_00016F0C (src/pilot/func_00016F0C.c) ---------------- */
#define CAR CAR__func_00016F0C
#define JUNC JUNC__func_00016F0C
#define NODE NODE__func_00016F0C
/* func_00016F0C - cop/civ car: choose exit at a junction (no Driver 2 match) */
typedef struct {
    char pad0[3];
    unsigned char flag;
    char pad4[4];
    JEXIT e[4];
    char pad28[4];
} JUNC;
/* 44 bytes */
typedef struct {
    char pad0[3];
    unsigned char b3;
    unsigned char b4;
    char pad5[4];
    unsigned char lane;
    char padA[13];
    unsigned char b17;
    char pad18[12];
    int dir;
    int road;
    int target;
    int exit;
    int turn;
    char pad38[4];
    int x3C;
} CAR;
typedef struct {
    char pad[12];
    short junc;
} NODE;
#define junctions (*(JUNC **)&D_00097040)
#define roads16 (*(ROAD16 **)&D_00097044)
#define SETEXIT() \
    c = choices[r]; \
    car->exit = c; \
    car->target = roads[c]; \
    if (car->exit == car->dir) { \
        car->turn = 0; \
    } else { \
        car->turn = (((car->exit - car->dir) << 10) + 0x800 & 0xFFF) - 0x800 >= 0 ? 1 : -1; \
    }
int func_00016F0C(CAR *car, NODE *node)
{
    unsigned char count;
    unsigned char lane;
    JEXIT *e;
    short roads[4];
    unsigned char choices[4];
    unsigned char allowed[4];
    unsigned char t;
    unsigned char i;
    unsigned char r;
    int dir;
    int d;
    short rd;
    int sgn;
    JUNC *jn;
    unsigned char c;
    unsigned char n;

    count = 0;
    lane = 0;

    if (node->junc == -1 || node->junc == 0x3FFF)
        return 0;

    if (junctions[node->junc].flag) {
        allowed[3] = 1;
        allowed[2] = 1;
        allowed[1] = 1;
        allowed[0] = 1;
    } else {
        allowed[3] = 0;
        allowed[2] = 0;
        allowed[1] = 0;
        allowed[0] = 0;
    }

    if ((unsigned int)car->road < 0x4000) {
        lane = car->lane;
        dir = car->dir;
        t = D_0009E744[car->road].lanes[dir] - 1;
        switch (dir) {
        case 0:
            if (t == 0) {
                allowed[0] = 1;
                allowed[1] = 1;
                allowed[3] = 1;
            } else if (lane == 0) {
                allowed[0] = 1;
                allowed[1] = 1;
            } else if (lane == t) {
                allowed[0] = 1;
                allowed[3] = 1;
            } else {
                allowed[0] = 1;
            }
            break;
        case 2:
            if (t == 0) {
                allowed[2] = 1;
                allowed[1] = 1;
                allowed[3] = 1;
            } else if (lane == 0) {
                allowed[2] = 1;
                allowed[3] = 1;
            } else if (lane == t) {
                allowed[2] = 1;
                allowed[1] = 1;
            } else {
                allowed[2] = 1;
            }
            break;
        case 1:
            if (t == 0) {
                allowed[0] = 1;
                allowed[1] = 1;
                allowed[2] = 1;
            } else if (lane == 0) {
                allowed[2] = 1;
                allowed[1] = 1;
            } else if (lane == t) {
                allowed[1] = 1;
                allowed[0] = 1;
            } else {
                allowed[1] = 1;
            }
            break;
        case 3:
            if (t == 0) {
                allowed[0] = 1;
                allowed[3] = 1;
                allowed[2] = 1;
            } else if (lane == 0) {
                allowed[3] = 1;
                allowed[0] = 1;
            } else if (lane == t) {
                allowed[2] = 1;
                allowed[3] = 1;
            } else {
                allowed[3] = 1;
            }
            break;
        }
    } else {
        allowed[3] = 1;
        allowed[1] = 1;
        allowed[2] = 1;
        allowed[0] = 1;
    }

    car->x3C = 0x82;
    car->b4 = *(unsigned char *)&D_00097038 + 1;
    car->b3 = 0;

    e = junctions[node->junc].e;
    roads[0] = e[0].road;
    roads[1] = e[1].road;
    roads[2] = e[2].road;
    roads[3] = e[3].road;

    rd = roads[0];
    if (!(e[0].flags & 2) && rd != -1 && rd != car->road
        && roads16[rd].dir != 0x82 && allowed[0]) {
        choices[count++] = 0;
        t = D_0009E744[rd].lanes[0] - 1;
        if (car->dir == 0 && t < car->lane)
            count--;
    }
    rd = roads[2];
    if (!(e[2].flags & 2) && rd != -1 && rd != car->road
        && roads16[rd].dir != 0x80 && allowed[2]) {
        choices[count++] = 2;
        t = D_0009E744[rd].lanes[2] - 1;
        if (car->dir == 2 && t < car->lane)
            count--;
    }
    rd = roads[1];
    if (!(e[1].flags & 2) && rd != -1 && rd != car->road
        && roads16[rd].dir != 0x83 && allowed[1]) {
        choices[count++] = 1;
        t = D_0009E744[rd].lanes[1] - 1;
        if (car->dir == 1 && t < car->lane)
            count--;
    }
    rd = roads[3];
    if (!(e[3].flags & 2) && rd != -1 && rd != car->road
        && roads16[rd].dir != 0x81 && allowed[3]) {
        choices[count++] = 3;
        t = D_0009E744[rd].lanes[3] - 1;
        if (car->dir == 3 && t < car->lane)
            count--;
    }

    if (count == 0) {
        for (i = 0; i < 4; i++) {
            rd = roads[i];
            if (!(e[i].flags & 2) && rd != -1 && rd != car->road
                && roads16[rd].dir != ((i + 2) & 3) + 0x80)
                choices[count++] = i;
        }
        if (count == 0)
            return 0;
    }

    if (count == 1)
        car->b17 |= 4;

    jn = &junctions[node->junc];
    func_00013A90(jn->e[0].road, jn->e[1].road, jn->e[2].road, jn->e[3].road);

    r = func_0004FFE8(lane) % count;
    SETEXIT();

    if (count == 3 && car->turn < 0) {
        if (node->junc & 1) {
            if ((unsigned int)(car->exit - 2) < 2) {
                if ((n = r + 1) >= 3)
                    n -= count;
                r = n;
                SETEXIT();
            }
        } else {
            if ((unsigned int)car->exit < 2) {
                if ((n = r + 1) >= 3)
                    n -= count;
                r = n;
                SETEXIT();
            }
        }
    }
    return 1;
}
#undef junctions
#undef roads16
#undef SETEXIT
#undef CAR
#undef JUNC
#undef NODE

/* ---------------- func_00017780 (src/pilot/func_00017780.c) ---------------- */
unsigned char func_00017780(CIV_SLOT *p, VECTOR *a1, int *a2, unsigned char mode)
{
    unsigned char res = 1;
    CAR_DATA *e = &D_0009B230[p->id];
    int d;

    if (p->f40 != 0x7FFFFFFF && p->f38 != 0 && (mode == 0 || mode == 2))
    {
        d = p->f40 - e->where.t[0];
        if (abs(d) <= 32)
        {
            if (p->f17 & 1)
            {
                e->where.t[0] = p->f40;
                p->f40 = 0x7FFFFFFF;
                p->f17 &= 0xDE;
                e->fF4 = 0;
                e->angle.vy = 0;
                if (p->f17 & 0x10)
                    p->f17 = (p->f17 & 0xEF) | 8;
            }
        }
        else if (p->f38 >= 11)
        {
            if (p->f17 & 1)
            {
                if (p->f24 == 2)
                {
                    if (p->f17 & 0x30)
                        e->fF4 = -(d / 8);
                    else
                        e->fF4 = -(d / 32);
                }
                else
                {
                    if (p->f17 & 0x30)
                        e->fF4 = d / 8;
                    else
                        e->fF4 = d / 32;
                }
            }
            res = 0;
        }
    }

    if (p->f44 != 0x7FFFFFFF && p->f38 != 0 && (mode == 1 || mode == 3))
    {
        d = p->f44 - e->where.t[2];
        if (abs(d) <= 32)
        {
            if (p->f17 & 1)
            {
                e->where.t[2] = p->f44;
                p->f44 = 0x7FFFFFFF;
                p->f17 &= 0xDE;
                e->fF4 = 0;
                e->angle.vy = 0;
                if (p->f17 & 0x10)
                    p->f17 = (p->f17 & 0xEF) | 8;
            }
        }
        else if (p->f38 >= 11)
        {
            if (p->f17 & 1)
            {
                if (p->f24 == 1)
                {
                    if (p->f17 & 0x30)
                        e->fF4 = -(d / 8);
                    else
                        e->fF4 = -(d / 32);
                }
                else
                {
                    if (p->f17 & 0x30)
                        e->fF4 = d / 8;
                    else
                        e->fF4 = d / 32;
                }
            }
            res = 0;
        }
    }

    if (res)
    {
        p->f40 = 0x7FFFFFFF;
        p->f44 = 0x7FFFFFFF;
    }
    return res;
}

/* ---------------- func_00017A50 (src/pilot/func_00017A50.c) ---------------- */
int func_00017A50(void)
{
    return D_00096FC4 != 0x16;
}

/* ---------------- func_00017A64 (src/pilot/func_00017A64.c) ---------------- */
int func_00017A64(void) {
    unsigned char i = D_00096FC4;
    if (i == 0x16)
        return -1;
    D_00096FC4 = i + 1;
    return D_00099AF0[i];
}

/* ---------------- func_00017AA0 (src/pilot/func_00017AA0.c) ---------------- */
int func_00017AA0(unsigned char type)
{
    int best = -1;
    int bestidx = -1;
    CAR_DATA *base;
    CAR_DATA *car;
    CAR_DATA *cur;
    CIV_SLOT *e;
    int i;
    unsigned char ok;
    int r;

    base = car_data;
    car = base + 1;
    cur = base + *(int *)&D_00096884;
    e = D_00098950;
    i = 0;
    for (; car < &car_data[23]; car++, e++, i++) {
        ok = 0;
        switch (type) {
        case 0:
            ok = 1;
            break;
        case 1:
            if (e->f5 != 0)
                ok = 1;
            break;
        case 2:
            if (e->f5 == 0)
                ok = 1;
            break;
        }
        if (ok) {
            if (e->f1 == 0) {
                if ((unsigned char)func_000192D0(&D_00099120, e->id) == 0)
                    return 1;
            } else {
                r = func_000612FC((char *)car + 0xC8, (char *)cur + 0xC8);
                if (best < r) {
                    best = r;
                    bestidx = i;
                }
            }
        }
    }
    if (bestidx == -1)
        return 0;
    func_00017C50(bestidx);
    return 1;
}

/* ---------------- func_00017C50 (src/pilot/func_00017C50.c) ---------------- */
void func_00017C50(int a)
{
    CIV_SLOT *p = &D_00098950[a];
    unsigned char x = p->f5;

    if (p->f1 != 0 && D_0009677C != 0) {
        if (*(volatile int *)&CameraCnt != D_00096FD0 || a != D_00096FF0) {
            D_00096FF0 = a;
            D_00096FD0 = *(volatile int *)&CameraCnt;
        }
    }
    if (x != 0) {
        func_00063AB4(&D_000C4690[p->f5]);
    }
    if (p->f1 == 9) {
        func_0001A008(&D_00099120);
    }
    p->f1 = 0;
    p->f8 = 0;
    D_00099AF0[--D_00096FC4] = a;
}

/* ---------------- func_00017D6C (src/pilot/func_00017D6C.c) ---------------- */
void func_00017D6C(void)
{
}

/* ---------------- func_00017D74 (src/pilot/func_00017D74.c) ---------------- */
void func_00017D74(CIV_SLOT *o) {
    o->f8 = 0;
    if (o->fB != 0) {
        o->fB = o->fB - 1;
    } else if (o->f28 != -1) {
        if ((func_0004FFE8(o->id) & 0x1F) == o->id) func_00018900(o);
    }
}

/* ---------------- func_00017DE8 (src/pilot/func_00017DE8.c) ---------------- */
#define SEG SEG__func_00017DE8
/* func_00017DE8 - no Driver 2 counterpart */
typedef struct {
    int x;
    int z;
    int pad8;
    short y;
    short padE;
    short dir;
    short pad12;
    int pad14;
} SEG;
void func_00017DE8(CIV_SLOT *c)
{
    SEG s1;
    SEG s2;
    PAIR pr;
    VECTOR *p;
    int *pz;
    int ok;
    int r;
    int t;

    if (c->f38 == 0)
        return;
    if (c->f2 == 1) {
        p = &car_data[c->id + 1].pos;
        if (func_00021820(p->vx, p->vz, c->f24, &s1) == 0) {
            c->f18 = s1.x;
            c->f1C = s1.z;
            c->f20 = s1.y;
            c->f12 = s1.dir;
        }
        if ((unsigned)(c->fD - 1) < 0x7C)
            c->fD++;
        return;
    }
    p = &car_data[c->id + 1].pos;
    pz = &p->vz;
    if (func_00018100(c, c->f24 << 10) < 64)
        ok = ((int (*)(CIV_SLOT *, VECTOR *, int *, unsigned char))func_00017780)(c, p, pz, c->f24);
    else
        ok = 0;
    r = func_00021820(p->vx, *pz, c->f24, &s2);
    if ((ok != 0 && s2.y == -1) || s2.y == 0x3FFF) {
        c->f2 = 1;
        return;
    }
    if (r == 1 && c->f3 != 0) {
        func_00014FC4(c->f2C, c->f30, c->f34, 2, c->f9, &pr);
        c->f17 &= ~4;
        c->f40 = pr.a;
        c->f44 = pr.b;
        c->f1 = 3;
        c->f14 = 0;
        return;
    }
    if (r != 0)
        return;
    c->f18 = s2.x;
    c->f1C = s2.z;
    c->f20 = s2.y;
    c->f12 = s2.dir;
    c->f9 = func_00015338(c);
    if (c->fC != 0)
        c->fC--;
    if (car_data[0].speed < 31)
        return;
    if (c->f38 < 11)
        return;
    if (func_0001A1BC(&car_data[c->id + 1]) == 0)
        return;
    if (c->fC != 0)
        return;
    t = c->id & 3;
    if (t == 3)
        t = 2;
    c->fC = 13;
    func_0004D740(-1, 0, t + 6, &car_data[c->id + 1].pos, &car_data[c->id + 1].vel);
}
#undef SEG

/* ---------------- func_00018100 (src/pilot/func_00018100.c) ---------------- */
int func_00018100(unsigned char *p, int target) {
    int cur, diff, a3;
    a3 = 0;
    cur = D_0009B230[*p].direction & 0xFFF;
    diff = ((target - cur + 0x800) & 0xFFF) - 0x800;
    if (cur != target) {
        if (abs(diff) < 0x80) a3 = diff / 2;
        else if (diff >= 0) a3 = 0x200;
        else a3 = -0x200;
    }
    D_0009B230[*p].fF4 = a3;
    return abs(diff);
}

/* ---------------- func_000181B4 (src/pilot/func_000181B4.c) ---------------- */
/* func_000181B4: car AI - turn toward the target heading, then pick the next road segment */
#define ABSDIFF(a, b) (((a) - (b)) < 0 ? (b) - (a) : (a) - (b))
void func_000181B4(CIV_SLOT *p)
{
    VECTOR *px;
    int *pz;
    int r;
    int ang, dir, d, steer;
    SEG_b062 seg;
    XZ_b062 v;

    if (p->f2 == 1 || p->f38 == 0)
        return;

    px = &D_0009B230[p->id].pos;
    pz = &px->vz;
    r = (short)func_00021558(px->vx, px->vz);
    p->f28 = r;

    if (p->f14 == 0) {
        switch (p->f24) {
        case 0:
        case 2:
            if (ABSDIFF(*pz, p->f44) < p->f38 + 700)
                p->f14 = 1;
            else
                return;
            break;
        case 1:
        case 3:
            if (ABSDIFF(px->vx, p->f40) < p->f38 + 700)
                p->f14 = 1;
            else
                return;
            break;
        }
    }

    steer = 0;
    ang = p->f30 << 10;
    dir = D_0009B230[p->id].direction & 0xFFF;
    d = ((ang - dir + 0x800) & 0xFFF) - 0x800;
    if (dir != ang) {
        if (abs(d) < 128)
            steer = d / 2;
        else if (d >= 0)
            steer = 512;
        else
            steer = -512;
    }
    D_0009B230[p->id].fF4 = steer;

    if (abs(d) < 64) {
        if (!(r & 0x4000)) {
        p->f1 = 2;
        p->f24 = p->f30;
        p->f28 = p->f2C;
        func_00021820(px->vx, *pz, p->f24, &seg);
        p->f18 = seg.a;
        p->f1C = seg.b;
        p->f20 = seg.c;
        p->f12 = seg.d;
        if (func_00016F0C(p, &seg)) {
            func_00014FC4(p->f28, p->f24, p->f34, 2, p->f9, &v);
            p->f44 = v.z;
            p->f40 = v.x;
        }
        } else {
            func_00017780(p, px, pz, p->f30);
        }
    }
}
#undef ABSDIFF

/* ---------------- func_00018478 (src/pilot/func_00018478.c) ---------------- */
void func_00018478(unsigned char *p)
{
    CAR_DATA *s = &D_0009B230[p[0]];
    if (p[0xB] != 0) {
        s->fF4 = -0x23F;
        p[0xB]--;
    } else {
        p[1] = 2;
        p[2] = 1;
    }
}

/* ---------------- func_000184DC (src/pilot/func_000184DC.c) ---------------- */
void func_000184DC(unsigned char *p)
{
    CAR_DATA *s = &D_0009B230[p[0]];
    if (p[0xB] != 0) {
        s->fF4 = 0x23F;
        p[0xB]--;
    } else {
        p[1] = 2;
        p[2] = 1;
    }
}

/* ---------------- func_00018540 (src/pilot/func_00018540.c) ---------------- */
void func_00018540(void)
{
}

/* ---------------- func_00018548 (src/pilot/func_00018548.c) ---------------- */
void func_00018548(void)
{
}

/* ---------------- func_00018550 (src/pilot/func_00018550.c) ---------------- */
void func_00018550(CIV_SLOT *a)
{
    REC_99120 *c;
    CAR_DATA *s;
    PURSUER *b;

    s = &D_0009B230[a->id];
    if (s->totalDamage == 0) {
        a->f3C = 0;
        a->f38 = 0;
        a->f15 = 1;
        a->f16 = 1;
    } else {
        b = (PURSUER *)func_00063B74(a, s);
        if (b != 0) {
            a->f1 = 2;
            c = &D_00099120;
            s->fFC = 0;
            b->f794 = 0;
            c->f26--;
            if (c->f26 == 0)
                func_0001A008(c);
        }
    }
}

/* ---------------- func_00018618 (src/pilot/func_00018618.c) ---------------- */
#define ROAD ROAD__func_00018618
/* civ car: try to start a parked car / pick spawn spot (unnamed) */
typedef struct { char pad[0xA]; unsigned char cA; char padB[5]; } ROAD;
#define DIFF(a, b) ((a) >= (b) ? ABS((a) - (b)) : ABS((b) - (a)))
#define ABS(x) ((x) < 0 ? -(x) : (x))
void func_00018618(CIV_SLOT *cp)
{
    CIV_SLOT *c;
    POS2 pos;
    int i;
    int cx, cz;

    c = D_00098950;
    if (cp->f17 & 9)
        return;
    if ((func_0004FFE8(cp->id) & 0x1F) != cp->id)
        return;
    if ((*(ROAD **)&D_00097044)[cp->f28].cA == 0)
        return;
    if (cp->f9 != 0)
        return;
    if (cp->f18 < 0x4650)
        return;
    if (cp->f1C < 0xBB8)
        return;
    func_00014FC4(cp->f28, cp->f24, 0, 1, 0, &pos);
    cx = pos.x / 1500;
    cz = pos.z / 1500;
    if (cp->f24 & 1) {
        for (i = 0; i < 22; i++, c++) {
            if (c->f1 && cp->id != c->id && c->z == cz) {
                if (DIFF(c->x, cp->x) < 8)
                    return;
            }
        }
    } else {
        for (i = 0; i < 22; i++, c++) {
            if (c->f1 && cp->id != c->id && c->x == cx) {
                if (DIFF(c->z, cp->z) < 8)
                    return;
            }
        }
    }
    switch (cp->f24) {
    case 0:
        cp->f40 = pos.x + 0x1EE;
        break;
    case 2:
        cp->f40 = pos.x - 0x1EE;
        break;
    case 1:
        cp->f44 = pos.z - 0x1BE;
        break;
    case 3:
        cp->f44 = pos.z + 0x1BE;
        break;
    }
    cp->f17 |= 0x11;
}
#undef DIFF
#undef ABS
#undef ROAD

/* ---------------- func_00018900 (src/pilot/func_00018900.c) ---------------- */
/* func_00018900 - matched run3-r4.
 * The mfhi t0 vs v0 leftover (spilled HI retried into the first free GPR) was fixed by declaring the
 * tail callee with its real return type: func_00016F0C returns int (tested in func_000181B4/000154A8).
 * The value-returning call_value form changes v0's liveness, so reload no longer picks v0 for HI. */
#define ABSD(a, b) ((a) - (b) < 0 ? (b) - (a) : (a) - (b))
void func_00018900(CIV_SLOT *cp)
{
    int pos[2];
    int tmp[6];
    int i;
    int px;
    CIV_SLOT *c;

    c = D_00098950;

    func_00014FC4(cp->f28, cp->f24, 0, 2, 0xfe, pos);
    cp->x = D_0009B230[cp->id].pos.vx / 1500;
    px = pos[0] / 1500;
    cp->z = D_0009B230[cp->id].pos.vz / 1500;

    if (cp->f24 & 1) {
        for (i = 0; i < 22; i++, c++) {
            if (cp->id != c->id && c->f1) {
                if (cp->f24 == 1) {
                    if (cp->f28 == c->f28 && c->z == cp->z
                        && c->x >= cp->x && ABSD(c->x, cp->x) < 5)
                        return;
                } else {
                    if (cp->f28 == c->f28 && c->z == cp->z
                        && cp->x >= c->x && ABSD(cp->x, c->x) < 5)
                        return;
                }
            }
        }
    } else {
        for (i = 0; i < 22; i++, c++) {
            if (cp->id != c->id && c->f1) {
                if (cp->f24 == 0) {
                    if (cp->f28 == c->f28 && c->x == px
                        && c->z >= cp->z && ABSD(c->z, cp->z) < 5)
                        return;
                } else {
                    if (cp->f28 == c->f28 && c->x == px
                        && cp->z >= c->z && ABSD(cp->z, c->z) < 5)
                        return;
                }
            }
        }
    }

    if (cp->f24 & 1) {
        cp->f40 = pos[0];
        cp->f44 = 0x7fffffff;
    } else {
        cp->f40 = 0x7fffffff;
        cp->f44 = pos[1];
    }
    cp->f17 = 0x23;
    cp->f1 = 2;
    cp->f8 = 1;
    cp->f3C = 0x82;
    func_00021820(D_0009B230[cp->id].pos.vx, D_0009B230[cp->id].pos.vz, cp->f24, tmp);
    cp->f18 = tmp[0];
    func_00016F0C(cp, tmp);
}
#undef ABSD

/* ---------------- func_00018C74 (src/pilot/func_00018C74.c) ---------------- */
void func_00018C74(CIV_SLOT *p)
{
    CIV_SLOT *o = D_00098950;
    int pos[2];
    int i, d, tx, tz;

    if (p->f17 & 9)
        return;
    if ((func_0004FFE8(p->id) - p->id) & 7)
        return;

    func_00014FC4(p->f28, p->f24, 0, 2, 0xFF, pos);
    tx = pos[0] / 1500;
    tz = pos[1] / 1500;
    p->x = D_0009B230[p->id].pos.vx / 1500;
    p->z = D_0009B230[p->id].pos.vz / 1500;

    if (p->f24 & 1)
    {
        d = p->z - tz;
        if (d >= 0)
        {
            if (d != 1)
                return;
        }
        else if (p->z != tz - 1)
            return;

        for (i = 0; i < 22; i++, o++)
        {
            if (p->id != o->id && p->f28 == o->f28 && o->z == tz && (p->x - o->x >= 0 ? p->x - o->x : o->x - p->x) < 5)
                return;
        }
        p->f17 |= 1;
        p->f44 = pos[1];
    }
    else
    {
        d = p->x - tx;
        if (d >= 0)
        {
            if (d != 1)
                return;
        }
        else if (p->x != tx - 1)
            return;

        for (i = 0; i < 22; i++, o++)
        {
            if (p->id != o->id && p->f28 == o->f28 && o->x == tx && (p->z - o->z >= 0 ? p->z - o->z : o->z - p->z) < 5)
                return;
        }
        p->f17 |= 1;
        p->f40 = pos[0];
    }
}

/* ---------------- func_00018F44 (src/pilot/func_00018F44.c) ---------------- */
/* find the car ahead on the same road/lane within maxdist */
int func_00018F44(CIV_SLOT *cp, int maxdist, int mode)
{
    CIV_SLOT *c;
    int dist;
    int i;

    dist = 1000;
    c = D_00098950;
    if (cp->f14 != 0)
        return -1;
    cp->x = D_0009B230[cp->id].pos.vx / 1500;
    cp->z = D_0009B230[cp->id].pos.vz / 1500;
    if (mode) {
        for (i = 0; i < 22; i++, c++) {
            if (cp->id != c->id && c->f1 != 0 && cp->f28 == c->f28 && cp->f24 == c->f24 && cp->f9 == c->f9) {
                switch (cp->f24) {
                case 0:
                    if (cp->z > c->z)
                        dist = cp->z - c->z;
                    break;
                case 2:
                    if (c->z > cp->z)
                        dist = c->z - cp->z;
                    break;
                case 1:
                    if (c->x > cp->x)
                        dist = c->x - cp->x;
                    break;
                case 3:
                    if (cp->x > c->x)
                        dist = cp->x - c->x;
                    break;
                }
                if (dist >= 0 && dist <= maxdist)
                    return i;
            }
        }
        return -1;
    }
    for (i = 0; i < 22; i++, c++) {
        if (cp->id != c->id && c->f1 != 1 && c->f1 != 0 && cp->f28 == c->f28 && cp->f24 == c->f24 && cp->f9 == c->f9) {
            c->x = D_0009B230[c->id].pos.vx / 1500;
            c->z = D_0009B230[c->id].pos.vz / 1500;
            switch (cp->f24) {
            case 0:
                if (c->z > cp->z)
                    dist = c->z - cp->z;
                break;
            case 2:
                if (cp->z > c->z)
                    dist = cp->z - c->z;
                break;
            case 1:
                if (c->x > cp->x)
                    dist = c->x - cp->x;
                break;
            case 3:
                if (cp->x > c->x)
                    dist = cp->x - c->x;
                break;
            }
            if (dist >= 0 && dist <= maxdist)
                return i;
        }
    }
    return -1;
}

/* ---------------- func_000192D0 (src/pilot/func_000192D0.c) ---------------- */
int func_000192D0(unsigned char *p, int c) {
    unsigned char *q = p + 0x29;
    unsigned char *e = p + (p[0x26] + 0x29);
    while (q < e) {
        if (*q == c) return 1;
        q++;
    }
    return 0;
}

/* ---------------- func_00019314 (src/pilot/func_00019314.c) ---------------- */
/* func_00019314 */
int func_00019314(REC_99120 *p, int dir)
{
	int r, idx;
	unsigned char *t;
	unsigned char v = dir;
	if (p->f1E != 0 || p->f1A != -1) {
	fail:
		return 0;
	}
	r = func_00021558(car_data[0].pos.vx, car_data[0].pos.vz);
	if (r & 0xC000)
		goto fail;
	idx = r & 0xFFFF;
	if (*(short *)(*(int *)&D_00097044 + idx * 16 + 8) < 20)
		goto fail;
	if ((dir & 1) != *(unsigned char *)(*(int *)&D_0009704C + idx * 20 + 0x10))
		goto fail;
	t = (unsigned char *)&D_0009E744[idx];
	p->f24 = t[v] + t[(v + 2) & 3];
	if (p->f24 == 0)
		goto fail;
	p->f26 = 0;
	p->f1A = r;
	p->f27 = dir;
	p->f28 = 1;
	return 1;
}

/* ---------------- func_0001943C (src/pilot/func_0001943C.c) ---------------- */
#define NODE NODE__func_0001943C
typedef struct { int a; int b; int c; int d; int e; } NODE;
void func_0001943C(REC_99120 *p)
{
    int dir;
    int off;
    NODE *q;
    int n;
    int r;
    int id;
    unsigned char c;
    CIV_SLOT *cp;
    int s;
    int f5;

    if (p->f28 != 1)
        return;
    dir = p->f27;
    q = &(*(NODE **)&D_0009704C)[p->f1A];
    off = -30000;
    if (dir & 1) {
        p->z = -q->d * 1500;
        if (dir == 1)
            off = 30000;
        p->x = *(int *)&D_0009B16C + off;
    } else {
        p->x = q->a * 1500;
        if (dir == 0)
            off = 30000;
        p->z = *(int *)&D_0009B174 + off;
    }
    r = p->f1C = func_00020DB4(p->x, p->z, dir, p->f10);
    if (r != -1) {
        ROAD_INFO *j = &D_0009E744[r];
        p->f25 = j->lanes[dir] + j->lanes[(dir + 2) & 3];
    } else {
        p->f25 = 0;
        p->f18 = 0;
    }
    n = p->f24 + p->f25 + p->f18;
    if (*(int *)&D_00096870 != 0) if (*(int *)&D_00096870 >= 0) if (*(int *)&D_00096870 < 3) {
        if (p->f18 == 0 && r != -1)
            n += n - 2;
        else
            n += n - 1;
    }
    if (p->f26 < n) {
        c = D_00096FC4;
        if (c == 22) {
            id = -1;
        } else {
            D_00096FC4 = c + 1;
            id = D_00099AF0[c];
        }
        if (id != -1) {
            p->list[p->f26++] = id;
        }
    } else if (n < p->f26) {
        do {
            s = p->list[--p->f26];
            cp = &D_00098950[s];
            f5 = cp->f5;
            if (cp->f1 && D_0009677C) {
                if (*(volatile int *)&CameraCnt != D_00096FD0 || s != D_00096FF0) {
                    D_00096FF0 = s;
                    D_00096FD0 = *(volatile int *)&CameraCnt;
                }
            }
            if (f5)
                func_00063AB4(&D_000C4690[cp->f5]);
            if (cp->f1 == 9)
                func_0001A008(&D_00099120);
            cp->f1 = 0;
            cp->f8 = 0;
            D_00099AF0[--D_00096FC4] = s;
        } while (n < p->f26);
    }
    if (p->f26 == n) {
        if (!func_00019AF4(p))
            func_0001A024(p);
    }
}
#undef NODE

/* ---------------- func_000197E0 (src/pilot/func_000197E0.c) ---------------- */
#define PlaceOne PlaceOne__func_000197E0
/* func_000197E0 - no Driver 2 counterpart */
static inline void PlaceOne(int x, int z, int dir, int seed, int val)
{
    int r;

    r = (func_0004FFE8(seed) & 0x3FF) - 0x600;
    D_00098950[func_00016760(x, -250, z, ((dir + 2) & 3) * 1024 - r, 1)].f28 = val;
}
void func_000197E0(pos, val, dir, count, flag)
    POS pos;
    int val, dir, count;
    unsigned char flag;
{
    int *p[2];
    POS tmp;
    int off;
    int i;

    off = 0;
    if (dir & 1) {
        p[0] = &pos.z;
        p[1] = &pos.x;
    } else {
        p[0] = &pos.x;
        p[1] = &pos.z;
    }
    switch (*(int *)&D_00096870) {
    case 0:
        break;
    case 1:
        off = 1500;
        break;
    case 2:
        off = 1750;
        break;
    }
    if (dir == 2 || dir == 3)
        off = -off;
    for (i = 0; i < count; i++) {
        PlaceOne(pos.x, pos.z, dir, i, val);
        if (flag || i < count - 1) {
            switch (*(int *)&D_00096870) {
            case 0:
                break;
            case 1:
                tmp.x = pos.x;
                tmp.z = pos.z;
                *p[0] += 750;
                *p[1] += off;
                PlaceOne(pos.x, pos.z, (dir - 1) & 3, i, val);
                pos.x = tmp.x;
                pos.z = tmp.z;
                break;
            case 2:
                tmp.x = pos.x;
                tmp.z = pos.z;
                *p[0] += 750;
                *p[1] += off;
                PlaceOne(pos.x, pos.z, dir, i, val);
                pos.x = tmp.x;
                pos.z = tmp.z;
                break;
            }
        }
        *p[0] += 1500;
        *p[1] += 0x80 - (func_0004FFE8(i) & 0xFF);
    }
}
#undef PlaceOne

/* ---------------- func_00019AF4 (src/pilot/func_00019AF4.c) ---------------- */
#define NODE NODE__func_00019AF4
typedef struct { int a; int b; int c; int d; int e; } NODE;
int func_00019AF4(REC_99120 *p)
{
    int x;
    int z;
    int node;
    unsigned char dir;
    unsigned char *ptr;
    unsigned char *end;
    int id;
    CIV_SLOT *e;
    int snd;
    NODE *n;

    x = p->x;
    z = p->z;
    node = p->f1C;
    dir = p->f27;
    if (func_00019DAC(p, x, z) == 0)
        return 0;
    ptr = p->list;
    end = p->list + p->f26;
    while (ptr < end) {
        id = *ptr;
        e = &D_00098950[id];
        snd = e->f5;
        if (e->f1 != 0 && D_0009677C != 0) {
            if (*(volatile int *)&CameraCnt != D_00096FD0 || id != D_00096FF0) {
                D_00096FF0 = id;
                D_00096FD0 = *(volatile int *)&CameraCnt;
            }
        }
        if (snd != 0)
            func_00063AB4(&D_000C4690[e->f5]);
        if (e->f1 == 9)
            func_0001A008(&D_00099120);
        e->f1 = 0;
        e->f8 = 0;
        ptr++;
        D_00099AF0[--D_00096FC4] = id;
    }
    func_000197E0(x, z, p->f1A, dir, p->f24, 0);
    p->f28 = 2;
    p->f1E = 450;
    if (node != -1) {
        n = &(*(NODE **)&D_0009704C)[node];
        switch (dir) {
        case 0:
        case 2:
            x = n->a * 1500;
            break;
        case 1:
        case 3:
            z = -(n->d * 1500);
            break;
        }
        func_000197E0(x, z, node, dir, p->f25 + p->f18, p->f18);
    }
    return 1;
}
#undef NODE

/* ---------------- func_00019DAC (src/pilot/func_00019DAC.c) ---------------- */
int func_00019DAC(REC_99120 *o, int x, int y)
{
    int pt[4];

    if (o->f1A == -1)
        return 0;
    if (!func_00019E88(o, x, y, o->f1A))
        return 0;
    if (o->f1C == -1)
        return 1;
    if (o->f18) {
        pt[0] = x + o->f10[2];
        pt[2] = y + o->f10[3];
        if (!func_0004A9F4(pt))
            return 0;
    }
    return func_00019E88(o, x + o->f10[0], y + o->f10[1], o->f1C) != 0;
}

/* ---------------- func_00019E88 (src/pilot/func_00019E88.c) ---------------- */
int func_00019E88(char *p, int x, int z, int id)
{
    int pos[6];
    int i;
    CIV_SLOT *e = D_00098950;
    int ix, iz;

    if ((unsigned short)func_00021820(x, z, *(unsigned char *)(p + 0x27), pos) != 0) return 0;
    if (*(short *)((char *)pos + 0xE) != id) return 0;
    if (pos[0] < 30000) return 0;
    ix = x / 1500;
    iz = z / 1500;
    for (i = 0; i < 22; i++, e++) {
        if (e->f28 == id) {
            if (*(unsigned char *)(p + 0x27) & 1) {
                if ((ix - e->x < 0 ? e->x - ix : ix - e->x) >= 3) continue;
                return 0;
            } else {
                if ((iz - e->z < 0 ? e->z - iz : iz - e->z) < 3) return 0;
            }
        }
    }
    return 1;
}

/* ---------------- func_00019FEC (src/pilot/func_00019FEC.c) ---------------- */
void func_00019FEC(char *p)
{
    if (*(short *)(p + 0x1E) != 0)
        *(short *)(p + 0x1E) -= 1;
}

/* ---------------- func_0001A008 (src/pilot/func_0001A008.c) ---------------- */
void func_0001A008(char *p)
{
    p[0x28] = 0;
    *(short *)(p + 0x1A) = -1;
    p[0x24] = 0;
    p[0x25] = 0;
    p[0x26] = 0;
}

/* ---------------- func_0001A024 (src/pilot/func_0001A024.c) ---------------- */
void func_0001A024(REC_99120 *p)
{
    unsigned char *q;
    unsigned char *end = (unsigned char *)p + (p->f26 + 0x29);
    for (q = p->list; q < end;) {
        int i = *q;
        int t;
        CIV_SLOT *s = &D_00098950[i];
        int a = s->f5;
        if (s->f1 != 0 && D_0009677C != 0) {
            if (*(int *)&CameraCnt != D_00096FD0 || i != D_00096FF0) {
                D_00096FF0 = i;
                D_00096FD0 = *(volatile int *)&CameraCnt;
            }
        }
        if (a != 0)
            func_00063AB4(&D_000C4690[s->f5]);
        if (s->f1 == 9)
            func_0001A008(&D_00099120);
        s->f1 = 0;
        s->f8 = 0;
        t = D_00096FC4 - 1;
        D_00096FC4 = t;
        q++;
        D_00099AF0[(unsigned char)t] = i;
    }
    p->f1E = 0;
    p->f28 = 0;
    p->f1A = -1;
    p->f24 = 0;
    p->f25 = 0;
    p->f26 = 0;
}

/* ---------------- func_0001A1BC (src/pilot/func_0001A1BC.c) ---------------- */
/* func_0001A1BC */
int func_0001A1BC(CAR_DATA *p)
{
    int dx, dz, r, c, s, a, b;
    r = car_data[0].direction;
    if ((unsigned)(((r - p->direction) & 0xfff) - 0x69c) >= 0x2c9) return 0;
    dx = p->pos.vx - car_data[0].pos.vx;
    dz = p->pos.vz - car_data[0].pos.vz;
    if ((unsigned)(dx + 0x2328) >= 0x4651) return 0;
    if (dz < -0x2328) return 0;
    if (dz >= 0x2329) return 0;
    c = rcossin_tbl[(r & 0xfff) * 2];
    s = rcossin_tbl[(r & 0xfff) * 2 + 1];
    if (dx * c + dz * s < -0x384000) return 0;
    c = rcossin_tbl[((r + 0x3ce) & 0xfff) * 2];
    s = rcossin_tbl[((r + 0x3ce) & 0xfff) * 2 + 1];
    if (dx * c + dz * s < -0x258000) return 0;
    c = rcossin_tbl[((r - 0x3ce) & 0xfff) * 2];
    s = rcossin_tbl[((r - 0x3ce) & 0xfff) * 2 + 1];
    return !(dx * c + dz * s < -0x258000);
}

/* ---------------- func_0001A30C (src/pilot/func_0001A30C.c) ---------------- */
#define remove_civ remove_civ__func_0001A30C
#define step_car step_car__func_0001A30C
/* MATCHED run3 (coordinator, 2026-10-22): func_00013D88 really returns int - declaring it void left v0 free after the call, so reload retried the spilled LO into v0. Old notes: (run2-r3): 'civ->t != 1' swap fixed by moving the rcossin step into static inline step_car().
 * Remaining: dz*dz product lands in v0 (target v1). Cause (.greg): dz*dz pseudo wins LO, dx*dx gets a2, then
 * reload spills LO for the dx*dx mult and retries dz*dz into the first free GPR (v0). In the target dx*dx must win
 * LO instead. Tried: operand/statement order, sq() inline, dist2() inline, in-place *=, 730 <= ..., !(<).
 * Plus reloc rcossin_tbl+2 vs D_0008EAAA (same bytes). */
/* Driver 2 name: unknown (per-frame civ car update / cull loop) */
static inline void remove_civ(int i)
{
    CIV_SLOT *s = &D_00098950[i];
    int a = s->f5;
    if (s->f1 != 0 && D_0009677C != 0) {
        if (*(int *)&CameraCnt != D_00096FD0 || i != D_00096FF0) {
            D_00096FF0 = i;
            D_00096FD0 = *(volatile int *)&CameraCnt;
        }
    }
    if (a != 0)
        func_00063AB4(&D_000C4690[s->f5]);
    if (s->f1 == 9)
        func_0001A008(&D_00099120);
    s->f1 = 0;
    s->f8 = 0;
    {
        int t = D_00096FC4 - 1;
        D_00096FC4 = t;
        D_00099AF0[(unsigned char)t] = i;
    }
}
static inline void step_car(CAR_DATA *car, int idx)
{
    idx &= 0xFFF;
    car->pos.vx += (rcossin_tbl[idx * 2] * 500 + 0x800) >> 12;
    car->pos.vz += (rcossin_tbl[idx * 2 + 1] * 500 + 0x800) >> 12;
}
void func_0001A30C(void)
{
    char pad[0x50];
    struct { int x, z; } cell;
    int count;
    int px, pz;
    int i;
    CAR_DATA *car = D_0009B230;
    CIV_SLOT *civ = D_00098950;
    int limit;
    int n;

    if (D_0009677C || D_00096780) {
        CAR_DATA *base = D_0009B230 - 1;
        px = base[23].pos.vx;
        pz = base[23].pos.vz;
    } else {
        CAR_DATA *base = D_0009B230 - 1;
        px = base[0].pos.vx;
        pz = base[0].pos.vz;
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
        int idx = car->direction;
        if (civ->f1 != 1)
            step_car(car, idx);
        D_0008539C[civ->f1](civ);
        if (civ->f1 != 0) {
            int dx, dz;
            dx = D_0009B230[i].pos.vx - px;
            if (dx < 0)
                dx = px - D_0009B230[i].pos.vx;
            dz = D_0009B230[i].pos.vz - pz;
            if (dz < 0)
                dz = pz - D_0009B230[i].pos.vz;
            dx /= 1500;
            dz /= 1500;
            if (dx * dx + dz * dz >= 730) {
                if (i < 22)
                    remove_civ(i);
                continue;
            }
            civ->x = D_0009B230[civ->id].pos.vx / 1500;
            civ->z = D_0009B230[civ->id].pos.vz / 1500;
            if ((*(int *)civ & 0xFFFF00) == 0x100 && car->f12C == 0) {
                if (civ->fA >= 20)
                    continue;
                civ->fA++;
            }
            if (civ->f1 != 4) {
                if (car->wheel_speed > 0)
                    civ->f38 = car->speed;
                else
                    civ->f38 = -car->speed;
                if (i < 22) {
                    func_00013D88(civ, car);
                    func_0001D608(car, 0);
                }
                if (civ->f1 == 2 && (*(int *)&CameraCnt & 1)) {
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
        if (!civ->f1)
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
#undef remove_civ
#undef step_car

/* ---------------- func_0001A94C (src/pilot/func_0001A94C.c) ---------------- */
void func_0001A94C(void) {
    int i;
    CAR_DATA *q = D_0009B230;
    CIV_SLOT *p = D_00098950;
    for (i = 0; i < 22; i++, q++, p++) {
        if (p->f1 != 0) func_0001D608(q, 0);
    }
}

/* ---------------- func_0001A9BC (src/pilot/func_0001A9BC.c) ---------------- */
/* per-frame bonus/score condition checks (no Driver 2 counterpart) */
void func_0001A9BC(void)
{
    char buf[80];
    unsigned char dir;
    int r;
    unsigned short v;
    int t;

    dir = func_000635B0(car_data[0].direction);
    r = func_00021558(car_data[0].pos.vx, car_data[0].pos.vz);
    v = r;
    if (v == 0xFFFF) {
        D_00096FE8 = -1;
    } else if (v != 0x8000) {
        if (v < 0x4000) {
            sprintf(buf, "Driving on road %d.\n", v);
            if (v != D_00096FE8) {
                if (func_0001AC18(v))
                    func_000650E4(D_000988C8, 3, 0x1000);
                D_00096FE8 = r;
            }
        } else {
            sprintf(buf, "Driving on junction %d.\n", r & 0x3FFF);
        }
    }

    t = func_00020AE4(car_data[0].pos.vx, car_data[0].pos.vz);
    if (t != -1 && (t & 0xC000) == 0 && t == ((dir + 2) & 3)) {
        D_000988C8[16]++;
        if (car_data[0].speed > 120)
            D_000C6F02[0] = 1;
    } else {
        D_000988C8[16] = 0;
    }
    func_000650E4(D_000988C8, 8, 0x1000);

    if (*(int *)&gNight != 0 && car_data[0].damage[0] > 1000 && car_data[0].damage[1] > 1000)
        func_000650E4(D_000988C8, 9, 0x1000);

    if (car_data[0].wheelspin != 0 || abs(car_data[0].f110) > 16000)
        D_000988C8[20]++;
    else
        D_000988C8[20] = 0;
    func_000650E4(D_000988C8, 10, 0x1000);

    if (car_data[0].speed > 180)
        func_000650E4(D_000988C8, 2, 0x1000);
}

/* ---------------- func_0001AC18 (src/pilot/func_0001AC18.c) ---------------- */
#define JN JN__func_0001AC18
typedef struct {
    char pad0[2]; unsigned char flag; char pad3[7];
    struct { short id; char pad[6]; } exits[4];
    char pad2a[2];
} JN;
#define junctions (*(JN **)&D_00097040)
#define roads (*(RD **)&D_00097044)
int func_0001AC18(int road)
{
    short found = -1;
    int i;
    int j;
    short cur;
    short other;

    if (D_00096FE8 == -1)
        return 0;
    i = 0;
    if (D_00096FE8 == road)
        return 0;
    cur = D_00096FE8;
    other = roads[road].b;
    for (i = 0; i < 4; i++) {
        if (junctions[roads[road].a].exits[i].id == cur) {
            found = roads[road].a;
            break;
        }
    }
    if (found == -1) {
        i = 0;
        if (other == -1)
            return 0;
        for (i = 0; i < 4; i++) {
            if (junctions[other].exits[i].id == D_00096FE8) {
                found = other;
                break;
            }
        }
        if (found == -1)
            return 0;
    }
    
    if (junctions[found].flag != 1)
        return 0;
    for (j = 0; j < 4; j++) {
        if (junctions[found].exits[j].id == road)
            break;
    }
    switch (i) {
    case 0: if (j == 3) return 0; break;
    case 1: if (j == 0) return 0; break;
    case 2: if (j == 1) return 0; break;
    case 3: if (j == 2) return 0; break;
    }
    if (i & 1) {
        if (*(unsigned char *)&D_00097050 != 1)
            return 0;
        if (*(unsigned int *)&D_00097058 < 0x127)
            return 1;
    } else if (*(unsigned char *)&D_00097054 == 1) {
        if (*(unsigned int *)&D_00097058 < 0x127)
            return 1;
    }
    return 0;
}
#undef junctions
#undef roads
#undef JN

