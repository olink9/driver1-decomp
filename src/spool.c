/* spool.c - Driver 1 CD streaming: spool queue, region/texture/model/soundbank loading.
 * Functions 216-247 of main (test_changemode..func_0003575C, 0x32DEC-0x357B4); D2 SPOOL.C counterpart.
 * Built from src/pilot by tools/mktu.py; conflicts unified to the Driver 2 types (SPOOLQ, Spool, SPL_REGIONINFO).
 *
 * The file defines its own data (.data 0x89118, .sdata 0x969E8, .sbss 0x971E0-0x972DC, each in address order;
 * D_00097214/40/6C/78/84/88/B8/C4 are unreferenced variables that fill the .sbss gaps).
 * maspsx settings for this file (tools/fnscore.py file_maspsx_env):
 *   gp_allow_offset=1  D_00097218+2 (tpage.y) is $gp-relative although D_00097218 is .comm here
 *   sbss_align8=0      D_0009728C (short[4], 8 bytes) sits at 0x9728C, not 8-aligned
 *   abs=               reached with %hi/%lo by the original although small: two of the file's own
 *                      variables (D_0009725C, D_000972CC) and the other files' small globals
 *                      (what the pilot files spelled `extern long long`)
 */
/* maspsx: gp_allow_offset=1 sbss_align8=0 abs=D_0009725C,D_000972CC */
/* maspsx: abs=D_00097320,D_0009731C,D_0009730C,D_00097314,current_region,D_0009733C,D_00096A24,D_00096834,D_00097524,current,D_00097560,D_00096C88,D_00097324,D_00096884,D_00097344,D_000970E0,D_000970D4,D_00097108,D_00097350,D_00097098,FastForward,D_00097534 */
#include "common.h"

/* ---- types ---- */
typedef struct {
    int x;
    int z;
} OFS;
/* Driver 2 SPOOLQ: one queued CD read */
typedef struct {
    unsigned char type;
    unsigned char data;
    unsigned short nsectors;
    unsigned int sector;
    char *addr;
    void (*func)();
} SPOOLQ;
typedef struct {
    int tpage;
    int slot;
} TSETINFO;
typedef struct {
    short vx, vy;
} SXYPAIR;
typedef struct {
    short x, y;
} RECTXY;
typedef struct {
    short v;
} SHORT1;
typedef struct {
    unsigned short gfx_offset;
    unsigned short model_offset;
    char pad4[4];
    unsigned char model_size;
    char pad9;
    unsigned char num_tpages;
    char padb[5];
} AREA_DATA;
typedef struct { int x; int y; } XYPAIR;
typedef struct OBJ {
    int pad0;
    short pad4;
    short ref;
    char pad8[8];
    int f10;
    int f14;
    int f18;
    int f1c;
    int f20;
} OBJ;
/* Driver 2 Spool (connected_areas is indexed past [2] by CheckLoadAreaData, as in D2) */
typedef struct {
    unsigned short offset;
    unsigned char connected_areas[2];
    unsigned char pvs_size;
    unsigned char cell_data_size[3];
    unsigned char super_region;
    unsigned char num_connected_areas;
    unsigned char roadm_size;
    unsigned char roadh_size;
} Spool;
typedef struct {
    unsigned short region_to_unpack;
    unsigned short target_barrel_region;
    int nsectors;
    char *cell_addr;
    char *roadm_addr;
    char *roadh_addr;
} SPL_REGIONINFO;
typedef struct { short x, y, w, h; } RECT;
/* the region being unpacked: copied from D_000A7EA4[] by func_0003575C */
typedef struct { int a; int idx; int *d; } S35558;
typedef struct { unsigned short v; unsigned short w; } P35558;

/* ---- .data 0x89118-0x89384 ---- */
/* CLUT loaded at (0x3C0,0x1AA) by func_000334B4/func_00034808, which rotate entries 1-13; entries 14-15 are the first word of D_00089134 */
unsigned short D_00089118[14] = {
    0x741F, 0x28A5, 0x2D09, 0x350E, 0x496D, 0x5A23, 0x1E8E, 0x2ED6, 0x5E52, 0x55EF, 0x498C, 0x3D4A, 0x30E7, 0x28A5
};
/* image data uploaded by func_0003336C (0x250 bytes) */
unsigned long D_00089134[148] = {
    0x3DEF294A, 0x0000020C, 0x00000000, 0x00200008, 0x00000000, 0x7777FE00,
    0x000EF777, 0x00000000, 0x00000000, 0x7777666E, 0x0E888777, 0x00000000,
    0x5E000000, 0x77766666, 0x98888877, 0x0000000E, 0x55E00000, 0x77766666,
    0x99888877, 0x000000E9, 0x555E0000, 0x77766665, 0x99888877, 0x00000E99,
    0x5554E000, 0x77766665, 0x99988877, 0x0000EAA9, 0x55444E00, 0x77666655,
    0x99988877, 0x000EAAAA, 0x54444400, 0x77666655, 0x99998877, 0x000BAAAA,
    0x444443E0, 0x77666555, 0xA9998887, 0x00EBBAAA, 0x44433330, 0x77666555,
    0xAA998887, 0x00BBBBAA, 0x4433333E, 0x77665554, 0xAAA99887, 0x0EBBBBBB,
    0x4333332E, 0xFFF65544, 0xBAAA99FF, 0x0ECBBBBB, 0x33322222, 0xFFFF5443,
    0xBBAAAFFF, 0x0CCCCBBB, 0x22222222, 0xEEFFF433, 0xCBBAFFFE, 0x0CCCCCCC,
    0x22222111, 0x00EFF222, 0xCCCCFFE0, 0x0DDDCCCC, 0x11111111, 0x00EFF111,
    0xDDDDFFE0, 0x0DDDDDDD, 0xDDDDDDDD, 0x00EFFDDD, 0x1111FFE0, 0x01111111,
    0xCCCCCDDD, 0x00EFFCCC, 0x2222FFE0, 0x01112222, 0xCCCCCCCC, 0xEEFFFABB,
    0x2334FFFE, 0x02222222, 0xBBBBCCCC, 0xFFFFAAAB, 0x33444FFF, 0x02222333,
    0xBBBBBBCE, 0xFFF99AAA, 0x344455FF, 0x0E333333, 0xABBBBBBE, 0x778899AA,
    0x44455667, 0x0E333333, 0xAAABBBB0, 0x7788899A, 0x44455667, 0x00333344,
    0xAAAABBE0, 0x77888999, 0x44556667, 0x00E33444, 0x9AAAAB00, 0x77788999,
    0x45556677, 0x00034444, 0x9AAAAE00, 0x77788899, 0x45556677, 0x000E4444,
    0x99AAE000, 0x77788899, 0x55566677, 0x0000E444, 0x999E0000, 0x77788889,
    0x55566677, 0x00000E45, 0x99E00000, 0x77788889, 0x55666677, 0x000000E5,
    0x9E000000, 0x77788888, 0x55666677, 0x0000000E, 0x00000000, 0x7777888E,
    0x0E666777, 0x00000000, 0x00000000, 0x7777FE00, 0x000EF777, 0x00000000,
    0x00000133, 0x04CC0000, 0x00000000, 0x00000C00, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x10001000, 0x00001000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
};

/* ---- .sdata 0x969E8-0x969F4 ---- */
int D_000969E8 = 0;
volatile int D_000969EC = 0;
volatile int spoolpos_writing = 0;

/* ---- .sbss 0x971E0-0x972DC: spool.c's own uninitialised globals, in address order ---- */
unsigned char D_000971E0[8];
unsigned char D_000971E8[8];
int D_000971F0;
int D_000971F4;
int D_000971F8;
int D_000971FC;
int D_00097200;
int D_00097204;
int D_00097208;
int D_0009720C;
int D_00097210;
int D_00097214;
RECTXY D_00097218;
SHORT1 D_0009721C;
SHORT1 D_0009721E;
int D_00097220;
int D_00097224;
int D_00097228;
int D_0009722C;
int D_00097230;
volatile int D_00097234;
char *D_00097238;
int D_0009723C;
int D_00097240;
int D_00097244;
int D_00097248;
char *D_0009724C;
int D_00097250;
char *D_00097254;
short *D_00097258;
short *D_0009725C;
int D_00097260;
int D_00097264;
int D_00097268;
int D_0009726C;
unsigned char *AreaTPages;
int D_00097274;
int D_00097278;
int D_0009727C;
int D_00097280;
int D_00097284;
int D_00097288;
extern short D_0009728C[];
short D_0009728C[4];
AREA_DATA *D_00097294;
int D_00097298;
int D_0009729C;
int D_000972A0;
int D_000972A4;
int D_000972A8;
unsigned short *D_000972AC;
volatile int spoolcounter;
int D_000972B4;
int D_000972B8;
int D_000972BC;
int D_000972C0;
int D_000972C4;
int *D_000972C8;
int *D_000972CC;
unsigned short *D_000972D0;
volatile int D_000972D4;
int D_000972D8;

/* ---- externs ---- */
extern SPOOLQ D_000A7BA4[];
extern SPL_REGIONINFO D_000A7EA4[];
extern int func_0007A12C();
extern void func_0007A27C();
extern int D_00097320;
extern int D_0009731C;
extern int D_0009730C;
extern int D_00097314;
extern int current_region;
extern int D_0009733C;
extern int D_00096A24;
extern int D_00096834;
extern POLY_FT4 D_000A7B3C;
extern void func_00074404();
extern int func_0007A7F4();
extern int D_00097524;
extern int current;
extern int func_000359BC();
extern void func_0004DE58();
extern void func_0004E024();
extern void DrawPrim();
extern void func_0007A53C();
extern void func_0007A580();
extern int tsetinfo[];
extern unsigned char D_000BE278[];
extern unsigned char tpageslots[];
extern SXYPAIR D_000BE708[];
extern SXYPAIR D_000BE1BC[];
extern unsigned long texture_cluts[][16];
extern unsigned short texture_pages[];
extern int D_00097560;
extern XYPAIR D_000BE0A4[][4];
extern int D_000A7F54[];
extern int D_00096C88;
extern unsigned long D_000BCE74[];
extern unsigned long D_000BD484[];
extern void func_0004E40C();
extern void func_0004E348();
extern OBJ *modelpointers[];
extern int D_00097324;
extern int D_00096884;
extern int D_00097344;
extern int D_000970E0;
extern short *D_0009E724[];
extern int *D_0009D8F4[];
extern char *D_000AA7E8[];
extern int D_000AA7D4[];
extern int D_000AB040[];
extern int D_000970D4;
extern int D_00097108;
extern int D_00097350;
extern int D_000A8024[];
extern int RoadMapRegions[];
extern int D_00097098;
extern int D_000AAF90[];
extern int D_000AA7E4[];
extern int D_000AB050[];
extern int func_00079F70();
extern void func_0007A4FC();
extern int FastForward;
extern S35558 D_000A7F44;
extern int D_00097534;

extern int printf();

/* ---- functions defined in this file ---- */
void test_changemode();
int func_00032F98();
void func_0003336C();
void func_000334B4();
void UpdateSpool();
void InitSpooling();
void SendTPage();
void func_00033AF8();
void LoadInAreaTSets();
void SendSBK();
void func_00033E54();
void func_00033F5C();
void CheckLoadAreaData();
void ClearRegion();
int LoadRegionData();
void UnpackRegion();
void ProcessSpoolInfoLump();
void func_00034808();
void func_0003490C();
void data_cb_textures();
void ready_cb_textures(unsigned char intr, unsigned char *result);
void ready_cb_regions(unsigned char intr, unsigned char *result);
void data_cb_regions();
void func_00034EE4();
void ready_cb_soundbank(unsigned char intr, unsigned char *result);
void data_cb_misc();
void func_000352D4(unsigned char a, unsigned char *b);
void StartSpooling();
void func_0003542C();
void func_000354D8();
void func_00035558();
void func_0003575C();

/* ---------------- test_changemode (src/pilot/test_changemode.c) ---------------- */
/* test_changemode */
void test_changemode(void)
{
    SPOOLQ *current = &D_000A7BA4[D_000969EC];
    int ta;

    if (D_000969EC != spoolcounter) {
      if (D_000971F4 == current->sector) {
        ta = current->addr;
        D_000971F0 = ta;
        D_00097204 = 1;
        switch (current->type) {
        case 0:
            D_000971F8 = D_000A7EA4[D_000972BC + 1].nsectors;
            D_000971FC = current->nsectors;
            func_0007A12C(ready_cb_regions);
            break;
        case 1:
            D_0009720C = 0;
            D_00097210 = 0;
            D_000971F8 = 17;
            D_000971FC = 1;
            D_00097220 = D_000972B4;
            D_000971F0 = current->addr + 0x4000;
            func_0007A12C(ready_cb_textures);
            break;
        case 3:
            D_000971F8 = current->nsectors;
            func_0007A12C(func_000352D4);
            break;
        case 2:
            D_00097274 = 0;
            D_0009720C = 0;
            D_00097210 = 0;
            D_000971FC = 2;
            D_000971F8 = current->nsectors;
            D_00097280 = current->data;
            D_000971F0 = ta + ((D_00097268 & 1) << 12);
            func_0007A12C(ready_cb_soundbank);
            break;
        }
      } else {
        D_00097204 = 0;
        func_0007A12C(0);
      }
    } else {
        D_00097204 = 0;
        func_0007A12C(0);
        func_0007A27C(9, 0);
    }
}

/* ---------------- func_00032F98 (src/pilot/func_00032F98.c) ---------------- */
/* func_00032F98 - no Driver 2 counterpart: count loaded neighbouring regions */
#define cellX      (*(int *)&D_00097320)
#define cellZ      (*(int *)&D_0009731C)
#define regionX    (*(int *)&D_0009730C)
#define regionZ    (*(int *)&D_00097314)
#define regionIdx  (*(int *)&current_region)
#define regionSize (*(int *)&D_0009733C)
#define regionsAll (*(int *)&D_00096A24)
#define gameMode   (*(int *)&D_00096834)
int func_00032F98(void)
{
    OFS ofs[19];
    int xs = 0;
    int zs = 0;
    int n = 0;
    int i;
    int count;
    int idx;
    int r;

    if (cellX < 9) {
        if (regionX != 0) {
            ofs[n].x = -1;
            ofs[n].z = 0;
            n++;
            xs = 1;
        }
    } else if (regionSize - 9 < cellX && regionX < regionsAll / regionSize) {
        ofs[n].x = 1;
        ofs[n].z = 0;
        n++;
        xs = 2;
    }
    if (cellZ < 9) {
        if (regionZ != 0) {
            ofs[n].x = 0;
            ofs[n].z = -1;
            n++;
            zs = 1;
        }
    } else if (regionSize - 9 < cellZ && regionZ != 0) {
        ofs[n].x = 0;
        ofs[n].z = 1;
        n++;
        zs = 2;
    }
    if (n == 2) {
        if (zs == 1) {
            n = 3;
            if (xs == 1) {
                ofs[2].x = -1;
                ofs[2].z = -1;
            } else {
                ofs[2].x = 1;
                ofs[2].z = -1;
            }
        } else {
            n = 3;
            if (xs == 1) {
                ofs[2].x = -1;
                ofs[2].z = 1;
            } else {
                ofs[2].x = 1;
                ofs[2].z = 1;
            }
        }
    }
    count = 0;
    for (i = 0; i < n; i++) {
        idx = regionIdx + ofs[i].x + ofs[i].z * (regionsAll / regionSize);
        r = D_0009728C[((regionX + ofs[i].x) & 1) + (((regionZ + ofs[i].z) & 1) << 1)];
        if (r != -1 && D_000972AC[idx] != 0xFFFF && r == idx)
            count++;
    }
    if (D_000969E8 != 0) {
        switch (D_00097230) {
        case 0:
            if (cellZ >= 25)
                count++;
            break;
        case 2:
            if (gameMode == 1 && regionIdx == 0xDF && cellX < 10)
                break;
            if (cellZ < 6)
                count++;
            break;
        case 1:
            if (cellX >= 25)
                count++;
            break;
        case 3:
            if (gameMode == 5 && regionIdx == 0x48) {
                if (cellX < 5)
                    count++;
            } else if (cellX < 6)
                count++;
            break;
        }
    }
    return count;
}
#undef cellX
#undef cellZ
#undef gameMode
#undef regionIdx
#undef regionSize
#undef regionX
#undef regionZ
#undef regionsAll

/* ---------------- func_0003336C (src/pilot/func_0003336C.c) ---------------- */
void func_0003336C(void) {
    int i;
    unsigned short *pal = D_00089118;
    struct { short x, y, w, h; } r;
    POLY_FT4 *p;
    for (i = 0; i < 14; i++) pal[i] &= 0x7FFF;
    pal[0] = 0;
    r.x = 0x3C0;
    r.y = 0x1AB;
    r.w = 8;
    r.h = 0x20;
    pal[14] |= 0x8000;
    pal[15] |= 0x8000;
    func_00074404(&r, D_00089134);
    p = &D_000A7B3C;
    ((unsigned char *)p)[3] = 9;
    p->code = 0x2E;
    p->r0 = 0x80; p->g0 = 0x80; p->b0 = 0x80;
    p->v0 = 0xAC;
    p->v1 = 0xAC;
    p->u0 = 0;
    p->u1 = 0x20;
    p->u2 = 0;
    p->v2 = 0xCC;
    p->u3 = 0x20;
    p->v3 = 0xCC;
    p->clut = func_0007A7F4(0x3C0, 0x1AA);
    p->tpage = GetTPage(0, 0, 0x3C0, 0x100);
    p->x0 = 0x1B0; p->y0 = 0x80;
    p->x1 = 0x1D6; p->y1 = 0x80;
    p->x2 = 0x1B0; p->y2 = 0x95;
    p->x3 = 0x1D6; p->y3 = 0x95;
}

/* ---------------- func_000334B4 (src/pilot/func_000334B4.c) ---------------- */
void func_000334B4(void)
{
    char pad[8];
    short rect[4];
    short *rp;
    unsigned short *q;
    int i;
    unsigned short t;

    if (D_00097248) func_00033E54();
    if (D_000972D4 && func_00032F98()) {
        func_000359BC();
        func_0004DE58();
        DrawSync(0);
        PutDrawEnv((DRAWENV *)(*(int *)&D_00097524 + 0x10));
        while (D_000972D4) {
            rp = rect;
            t = D_00089118[1];
            i = 1;
            q = D_00089118 + 1;
            for (; i < 13; i++, q++)
                *q = q[1];
            D_00089118[i] = t;
            rp[0] = 0x3c0;
            rp[1] = 0x1aa;
            rp[2] = 0x10;
            rp[3] = 1;
            func_00074404(rect, D_00089118);
            DrawPrim(&D_000A7B3C);
            DrawSync(0);
            VSync(0);
        }
        PutDrawEnv((DRAWENV *)(*(int *)&current + 0x10));
        func_0004E024();
        PutDrawEnv((DRAWENV *)(*(int *)&current + 0x10));
        func_0004E024();
    }
}

/* ---------------- UpdateSpool (src/pilot/UpdateSpool.c) ---------------- */
/* UpdateSpool */
void UpdateSpool(void)
{
	char pos[4];
	SPOOLQ *current;
	int sec;
	int n;

	current = &D_000A7BA4[D_000969EC];
	D_000971F0 = current->addr;

	switch (current->type) {
	case 0:
		D_00097260 = 5;
		n = D_000A7EA4[D_000972BC].nsectors;
		D_000971FC = current->nsectors;
		D_000971F8 = n;
		func_0007A53C(data_cb_regions);
		func_0007A12C(ready_cb_regions);
		break;
	case 1:
		D_00097260 = 5;
		D_0009720C = 0;
		D_00097210 = 0;
		D_000971F8 = 17;
		D_000971FC = 1;
		D_00097220 = D_000972B4;
		func_0007A53C(data_cb_textures);
		func_0007A12C(ready_cb_textures);
		D_000971F0 += 0x4000;
		break;
	case 3:
		D_00097260 = 5;
		D_000971F8 = current->nsectors;
		func_0007A53C(data_cb_misc);
		func_0007A12C(func_000352D4);
		break;
	case 2:
		D_00097260 = 5;
		D_00097274 = 0;
		D_0009720C = 0;
		D_00097210 = 0;
		D_000971FC = 2;
		D_000971F8 = current->nsectors;
		D_00097280 = current->data;
		func_0007A53C(func_00034EE4);
		func_0007A12C(ready_cb_soundbank);
		D_000971F0 += (D_00097268 & 1) << 12;
		break;
	}
	sec = current->sector;
	D_00097224 = 0;
	D_00097204 = 0;
	D_000971F4 = sec;
	func_0007A580(sec, pos);
	func_0007A27C(0x1B, pos);
}

/* ---------------- InitSpooling (src/pilot/InitSpooling.c) ---------------- */
/* FAKEMATCH: empty __asm__ used as a compiler hint (no instruction emitted) */
/* Driver 2: InitSpooling */
void InitSpooling(void) {
    int i;
    int z;
    char pad[0x88];
    for (i = 0; i < 4; i++) {
        D_0009728C[i] = -1;
        ClearRegion(i);
    }
    D_000972D4 = 0;
    __asm__ volatile("");
    D_00097234 = 0;
    z = D_00097234;
    D_00097244 = 0;
    D_00097268 = 0;
    D_000972B4 = 0;
    D_00097250 = 0;
    D_0009727C = 0;
    D_000972BC = 0;
    D_000972A0 = 0;
    D_000972A8 = 0;
    D_000972A4 = 0;
    D_00097248 = 0;
    D_00097260 = 0;
    D_000972D8 = 0;
    spoolcounter = z;
}

/* ---------------- SendTPage (src/pilot/SendTPage.c) ---------------- */
/* SendTPage - Driver 2: SendTPage
 * Matched in run3-r4 (permuter + hand cleanup). What it took:
 * - texture_cluts[tpage2send] is built from a byte offset and a base pointer that are both assigned BEFORE the
 *   LoadImage call (clutofs = tpage2send << 6; clutbase = texture_cluts) and added after it.
 * - tpage.w / tpage.h (D_0009721C/E) are struct fields (tpage is a RECT16 in the original): declared as 2-byte
 *   structs so the stores get MEM_IN_STRUCT aliasing like the real tpage.w/h.
 * - else branch: the tpageloaded[tpage2send] address is a local pointer (`loaded`) used for test and store.
 */
/* model_spool_buffer */
/* tsetpos */
/* nTPchunks */
/* loadbank_write */
/* tsetcounter */
/* tpage.x/y */
/* tpage.w */
/* tpage.h */
/* tsetinfo */
/* tpageloaded */
/* tpageslots */
/* slot_tpagepos */
/* slot_clutpos */
/* texture_cluts */
/* texture_pages */
/* LoadImage */
#define getClut(x, y) (((y) << 6) | (((x) >> 4) & 0x3f))
#define getTPage(tp, abr, x, y) \
    ((((tp) & 0x3) << 7) | (((abr) & 0x3) << 5) | (((y) & 0x100) >> 4) | (((x) & 0x3ff) >> 6) | (((y) & 0x200) << 2))
void SendTPage(void)
{
    unsigned char old;
    unsigned long *clutptr;
    int slot;
    int npalettes;
    int tpage2send;
    int i;
    char *addr;
    RECT16 cluts;
    int clutofs;
    char *clutbase;
    unsigned char *loaded;

    addr = (char *)D_00097238 + 0xA000;
    tpage2send = tsetinfo[D_00097250 * 2];
    slot = tsetinfo[D_00097250 * 2 + 1];

    if (D_00097208 == 0) {
        addr += 0x4000;
        if (slot != D_000BE278[tpage2send] - 1) {
            npalettes = *(int *)addr;
            D_00097218.x = D_000BE708[slot].vx;
            addr += 4;
            clutofs = tpage2send << 6;
            clutbase = (char *)texture_cluts;
            D_00097218.y = D_000BE708[slot].vy;
            D_0009721C.v = 64;
            D_0009721E.v = 64;
            cluts.x = D_000BE1BC[slot].vx;
            cluts.y = D_000BE1BC[slot].vy;
            cluts.w = 64;
            cluts.h = npalettes / 4 + 1;
            func_00074404(&cluts, addr);

            clutptr = (unsigned long *)(clutbase + clutofs);
            i = 0;

            while (i < npalettes) {
                clutptr[0] = getClut(cluts.x + 16, cluts.y) << 0x10 | getClut(cluts.x, cluts.y);
                clutptr[1] = getClut(cluts.x + 48, cluts.y) << 0x10 | getClut(cluts.x + 32, cluts.y);
                clutptr += 2;
                cluts.y++;
                i += 4;
            }

            texture_pages[tpage2send] = getTPage(0, 0, D_00097218.x, D_00097218.y);
        }
    } else {
        addr += (D_00097244 & 1) << 13;
        loaded = &D_000BE278[tpage2send];
        if (slot != *loaded - 1) {
            func_00074404((RECT16 *)&D_00097218, addr);
            D_00097218.y += D_0009721E.v;
        }

        if (D_00097208 == 4) {
            old = tpageslots[slot];
            tpageslots[slot] = tpage2send;
            D_000BE278[old] = 0;
            *loaded = slot + 1;
            D_00097250++;
            if (D_00097250 == D_000972B4) {
                D_000972B4 = 0;
                D_00097250 = 0;
            }
        }
    }
}
#undef getClut
#undef getTPage

/* ---------------- func_00033AF8 (src/pilot/func_00033AF8.c) ---------------- */
void func_00033AF8(void)
{
    while (D_000972D4 != 0)
        ;
}

/* ---------------- LoadInAreaTSets (src/pilot/LoadInAreaTSets.c) ---------------- */
/* Driver 2: LoadInAreaTSets */
static inline void RequestSpool(int type, int data, int offset, int loadsize, char *address, void (*func)())
{
    SPOOLQ *next;
    int sector;

    next = &D_000A7BA4[spoolcounter];
    sector = D_000BE0A4[*(int *)&D_00096834][3].x / 2048 + offset;
    next->type = type;
    next->data = data;
    next->sector = sector;
    next->nsectors = loadsize;
    next->addr = address;
    next->func = func;
    spoolcounter++;
}
void LoadInAreaTSets(int area)
{
    unsigned char *tpages;
    int ntpages_to_load;
    int i;
    int slot;
    int j;
    int offset;
    int availableslots[16];
    int navailable;
    char *loadaddr;

    navailable = 0;
    tpages = AreaTPages + area * 16;
    loadaddr = D_00097238 + 0xA000;
    slot = *(int *)&D_00097560;
    ntpages_to_load = D_00097294[area].num_tpages;

    for (; slot < 15; slot++) {
        if (tpageslots[slot] == 0xFF) {
            availableslots[navailable++] = slot;
        } else {
            for (j = 0; j < ntpages_to_load; j++) {
                if (tpageslots[slot] == tpages[j])
                    break;
            }
            if (j == ntpages_to_load)
                availableslots[navailable++] = slot;
        }
    }

    offset = D_00097294[area].gfx_offset;

    for (i = 0; i < ntpages_to_load; i++) {
        if (D_000BE278[*tpages] == 0) {
            if (navailable <= 0)
                return;
            navailable--;
            D_000A7F54[D_000972B4 * 2] = availableslots[navailable];
        } else {
            D_000A7F54[D_000972B4 * 2] = D_000BE278[*tpages] - 1;
        }

        RequestSpool(1, 0, offset, 17, loadaddr, SendTPage);
        offset += 17;
        D_000A7F54[D_000972B4 * 2 - 1] = *tpages;
        D_000972B4++;
        tpages++;
    }
}

/* ---------------- SendSBK (src/pilot/SendSBK.c) ---------------- */
/* SendSBK */
/* Driver 2: SendSBK (SPOOL.C) */
void SendSBK(void)
{
    char *address;
    int slength;
    unsigned long i, n;
    unsigned long *source, *dest;

    address = (char *)(D_00097238 + 0x8000);
    address += (D_00097244 & 1) << 12;
    if (D_00097274 == 0) {
        address += 4;
        if (*(int *)&D_00096C88 != -1) {
            func_0004E40C();
            func_0004E348();
        }
        dest = D_000BCE74;
        D_00097228 = *(int *)address;
        n = D_00097228 << 2;
        source = (unsigned long *)(address + 4);
        for (i = 0; i < n; i++)
            dest[i] = source[i];
        address += D_00097228 * 16 + 4;
        slength = 0xFF8 - D_00097228 * 16;
        D_0009729C = D_000BD484[D_00097280];
    } else {
        slength = 0x1000;
    }
    SpuSetTransferMode(0);
    SpuSetTransferStartAddr(D_0009729C);
    SpuRead(address, (slength + 63) & -64);
    D_00097274++;
    D_0009729C += slength;
}

/* ---------------- func_00033E54 (src/pilot/func_00033E54.c) ---------------- */
void func_00033E54(void)
{
    OBJ *p;
    OBJ *q;
    OBJ *o;
    int i, n, hdr;

    D_00097248 = 0;
    n = *D_000972D0++;
    p = (OBJ *)D_00097238;
    for (i = 0; i < n; i++) {
        hdr = *(int *)p;
        p = (OBJ *)((int *)p + 1);
        modelpointers[D_000972D0[i]] = p;
        o = p;
        if (p->pad4 == -1) {
            if (p->f20)
                p->f20 += (int)p;
            p->f10 += (int)p;
            p->f18 += (int)p;
            p->f1c += (int)p;
        } else {
            q = modelpointers[p->pad4];
            if (q->f20)
                p->f20 = q->f20;
            p->f10 = q->f10;
            p->f18 = q->f18;
            p->f1c = q->f1c;
        }
        o->f14 += (int)o;
        p = (OBJ *)((char *)p + hdr);
    }
    D_000969E8 = 0;
}

/* ---------------- func_00033F5C (src/pilot/func_00033F5C.c) ---------------- */
/* SetupModels */
void func_00033F5C(void) {
    if (*(int *)&D_00097324 == -1)
        func_00033E54();
    else
        D_00097248 = 1;
}

/* ---------------- CheckLoadAreaData (src/pilot/CheckLoadAreaData.c) ---------------- */
/* Driver 2: CheckLoadAreaData (with LoadInAreaModels / RequestSpool inlined) */
extern int current_region;
#define current_region (*(int *)&current_region)
#define old_region (*(int *)&D_00097324)
/* mktu: RequestSpool - identical to the definition in src/pilot/LoadInAreaTSets.c */
static inline void LoadInAreaModels(int area)
{
    int offset;
    int length;

    offset = D_00097294[area].model_offset;
    length = D_00097294[area].model_size;
    D_000972D0 = (unsigned short *)(D_00097238 + (length - 1) * 2048);
    RequestSpool(3, 0, offset, length, D_00097238, func_00033F5C);
}
void CheckLoadAreaData(int cellx, int cellz)
{
    int nAreas;
    int load;
    int boundary;
    int none;
    int i;
    Spool *spoolptr;

    i = old_region;
    spoolptr = (Spool *)(D_00097254 + D_000972AC[current_region]);
    load = 0;
    none = -1;

    if (i == none && spoolptr->super_region != 0xFF)
        goto use_super;
    {
        nAreas = spoolptr->num_connected_areas;

        if (i == none)
            D_000972C0 = i;
        else if (spoolptr->super_region != 0xFF || nAreas == 0)
            return;

        boundary = 13;
        if (*(int *)&D_00096884 == 0)
            boundary = 15;

        for (i = 0; i < nAreas; i++) {
            if (D_000972C0 != (spoolptr->connected_areas[i] & 0x3f)) {
                D_00097230 = spoolptr->connected_areas[i] >> 6;
                switch (D_00097230) {
                case 0:
                    if (cellz > *(int *)&D_0009733C - boundary)
                        load = 1;
                    break;
                case 1:
                    if (cellx > *(int *)&D_0009733C - boundary)
                        load = 1;
                    break;
                case 2:
                    if (cellz < boundary)
                        load = 1;
                    break;
                case 3:
                    if (cellx < boundary)
                        load = 1;
                    break;
                }
            }
            if (load)
                break;
        }

        if (i == nAreas) {
            if (D_000972C0 != -1)
                return;
            if (*(int *)&D_00097344 == -1)
                return;
            D_000972C0 = *(int *)&D_00097344;
        } else {
            D_000972C0 = spoolptr->connected_areas[i] & 0x3f;
        }
        goto done;
    }
use_super:
    D_000972C0 = spoolptr->super_region;
done:

    D_000969E8 = 1;
    LoadInAreaTSets(D_000972C0);
    LoadInAreaModels(D_000972C0);
}
#undef current_region
#undef old_region

/* ---------------- ClearRegion (src/pilot/ClearRegion.c) ---------------- */
void ClearRegion(int a0)
{
    int s;
    int n;
    int i;
    unsigned short *p;
    short *q;
    int *r;
    int *z;

    s = *(int *)&D_0009733C;
    n = a0 * s * s;
    q = D_0009E724[a0];
    r = D_0009D8F4[a0];
    p = (unsigned short *)*(int *)&D_000970E0 + n;
    for (i = s * s; i != 0; i--)
        *p++ = 0xFFFF;
    for (i = 0xE10; i != 0; i--) {
        *q++ = -1;
        *r++ = 0x38000000;
    }
    z = (int *)D_000AA7E8[a0];
    for (i = 0x384; i != 0; i--)
        *z++ = 0;
}

/* ---------------- LoadRegionData (src/pilot/LoadRegionData.c) ---------------- */
/* Driver 2: LoadRegionData. RequestSpool is a static inline here. Key: ONE multi-set buffer variable for the cells / cell_objects targets, built in place (addr = base; addr += idx * size;) - a single-set temp gets sched1 birthing boost and the in-place += fixes the a0/a1 allocation. */
/* mktu: RequestSpool - identical to the definition in src/pilot/LoadInAreaTSets.c */
int LoadRegionData(int region, int target_region)
{
    int offset;
    char *addr;
    char *roadmap_buffer;
    char *roadh_buffer;
    char *cell_buffer;
    Spool *spoolptr;

    D_0009728C[target_region] = region;
    spoolptr = (Spool *)(D_00097254 + D_000972AC[region]);
    offset = spoolptr->offset;
    roadmap_buffer = D_000AA7E8[target_region];
    RequestSpool(0, 0, offset, spoolptr->roadm_size, roadmap_buffer, func_0003542C);
    offset += spoolptr->roadm_size;
    roadh_buffer = roadmap_buffer + 0x1000;
    RequestSpool(0, 0, offset, spoolptr->roadh_size, roadh_buffer, func_000354D8);
    offset += spoolptr->roadh_size;
    cell_buffer = D_0009724C;
    RequestSpool(0, 0, offset, spoolptr->cell_data_size[1], cell_buffer, 0);
    offset += spoolptr->cell_data_size[1];
    addr = (char *)*(int **)&D_000970D4;
    addr += D_000AA7D4[target_region] * 4;
    RequestSpool(0, 0, offset, spoolptr->cell_data_size[0], addr, 0);
    offset += spoolptr->cell_data_size[0];
    addr = *(char **)&D_00097108;
    addr += (*(int *)&D_00097350 + D_000AB040[target_region]) * 16;
    RequestSpool(0, 0, offset, spoolptr->cell_data_size[2], addr, 0);
    offset += spoolptr->cell_data_size[2];
    RequestSpool(0, 0, offset, spoolptr->pvs_size, D_000AA7E8[target_region], func_0003490C);
    offset += spoolptr->pvs_size;
    D_000A7EA4[D_0009727C].nsectors = offset - spoolptr->offset;
    D_000A7EA4[D_0009727C].region_to_unpack = region;
    D_000A7EA4[D_0009727C].target_barrel_region = target_region;
    D_000A7EA4[D_0009727C].cell_addr = cell_buffer;
    D_000A7EA4[D_0009727C].roadm_addr = roadmap_buffer;
    D_000A7EA4[D_0009727C].roadh_addr = roadh_buffer;
    return 1;
}

/* ---------------- UnpackRegion (src/pilot/UnpackRegion.c) ---------------- */
/* Driver 2: UnpackRegion */
void UnpackRegion(int a, int b) {
    if (D_0009728C[b] == -1) {
        if (LoadRegionData(a, b)) {
            D_0009727C++;
        }
        D_000A8024[b] = a;
        RoadMapRegions[b] = a;
    }
}

/* ---------------- ProcessSpoolInfoLump (src/pilot/ProcessSpoolInfoLump.c) ---------------- */
/* Driver 2: ProcessSpoolInfoLump */
void ProcessSpoolInfoLump(int *lump)
{
    int size, i, slots, objs, alloc, m, n, r;
    int *a2, *a1;
    char *p = (char *)lump;

    size = *(int *)p;
    size <<= 11;
    p += 4;
    D_00097238 = *(char **)&D_00097098;
    if (size <= 0xFFFF)
        size = 0x10000;
    *(int *)&D_00097098 += size;
    alloc = *(int *)&D_00097098;
    slots = 0;
    objs = 0;
    size = *(int *)p;
    D_0009723C = (int)(p + 4);
    p += size + 4;
    n = *(int *)p;
    D_00097294 = (AREA_DATA *)(p + 4);
    p += n * 16 + 4;
    AreaTPages = (unsigned char *)p;
    p += n * 16;
    a1 = (int *)p;
    D_00097264 = n;
    for (i = 0; i < 4; i++) {
        D_000AA7E8[i] = (char *)alloc;
        D_000AAF90[i] = -1;
        D_000AA7D4[i] = slots;
        D_000AB040[i] = objs;
        slots += a1[i];
        objs += a1[i + 4];
        alloc += (a1[i + 8] + 0x7FF) & -0x800;
    }
    p += 0x30;
    D_000AA7E4[0] = slots;
    D_000AB050[0] = objs;
    *(int *)&D_00097098 = alloc;
    r = *(int *)p;
    D_000972AC = (unsigned short *)(p + 4);
    p += r * 2 + 4;
    D_00097254 = (char *)(p + 4);
}

/* ---------------- func_00034808 (src/pilot/func_00034808.c) ---------------- */
void func_00034808(void)
{
    RECT rect;
    int s0;
    unsigned short first, *p, *a;
    int i;
    s0 = func_0007A12C(0);
    func_000359BC();
    func_0004DE58();
    DrawSync(0);
    PutDrawEnv((DRAWENV *)(*(int *)&D_00097524 + 0x10));
    while (func_00079F70(1) != 2) {
        a = D_00089118;
        first = a[1];
        i = 1;
        p = a + 1;
        for (; i < 13; i++, p++)
            *p = p[1];
        a[i] = first;
        rect.x = 0x3C0;
        rect.y = 0x1AA;
        rect.w = 16;
        rect.h = 1;
        func_00074404(&rect, a);
        DrawPrim(&D_000A7B3C);
        DrawSync(0);
        VSync(0);
    }
    PutDrawEnv((DRAWENV *)(*(int *)&current + 0x10));
    func_0004E024();
    func_0007A12C(s0);
}

/* ---------------- func_0003490C (src/pilot/func_0003490C.c) ---------------- */
void func_0003490C(void) {
    int i;
    func_0003575C();
    i = D_000972BC;
    D_000972BC = i + 1;
    D_0009728C[D_000A7EA4[i].target_barrel_region] = -1;
    if (D_000972BC == D_0009727C) {
        D_0009727C = 0;
        D_000972BC = 0;
    }
}

/* ---------------- data_cb_textures (src/pilot/data_cb_textures.c) ---------------- */
/* Driver 2: data_cb_textures */
void data_cb_textures(void)
{
    SPOOLQ *s;
    if (D_00097200 == 0)
        return;
    spoolpos_writing;
    D_00097208 = D_00097210;
    D_00097200 = 0;
    s = D_000A7BA4;
    SendTPage();
    if (D_00097208 != 0)
        D_00097244++;
    D_00097210++;
    if (D_00097210 != 5)
        return;
    D_00097210 = 0;
    spoolpos_writing++;
    if (D_00097220 != 0)
        return;
    if (D_00097204 != 0) {
        D_00097204 = 0;
        D_00097224 = 0;
        switch (s[spoolpos_writing].type) {
        case 0: func_0007A53C(data_cb_regions); break;
        case 1: func_0007A53C(data_cb_textures); break;
        case 3: func_0007A53C(data_cb_misc); break;
        case 2: func_0007A53C(func_00034EE4); break;
        }
    } else {
        func_0007A53C(0);
        if (spoolpos_writing == spoolcounter) {
            D_000972D4 = D_000969EC = spoolpos_writing = spoolcounter = 0;
        } else
            UpdateSpool();
    }
}

/* ---------------- ready_cb_textures (src/pilot/ready_cb_textures.c) ---------------- */
/* Driver 2: ready_cb_textures */
void ready_cb_textures(unsigned char intr, unsigned char *result)
{
    char buf[8];
    SPOOLQ *cur;
    if (intr == 1) {
        func_0007A4FC(D_000971F0, 512);
        D_000971F0 += 0x800;
        D_000971FC--;
        D_000971F8--;
        D_000971F4++;
        if (D_000971FC != 0)
            return;
        cur = &D_000A7BA4[D_000969EC];
        if (D_0009720C)
            D_00097268++;
        D_00097200 = intr;
        D_0009720C++;
        if (D_000971F8 != 0) {
            D_000971FC = 4;
            D_000971F0 = cur->addr + (D_00097268 & 1) * 0x2000;
        } else {
            D_00097220--;
            D_000969EC++;
            if (D_00097220 != 0) {
                D_0009720C = 0;
                D_000971F8 = 17;
                D_000971FC = intr;
                D_000971F0 = cur->addr + 0x4000;
            } else {
                test_changemode();
            }
        }
    } else {
        if (*result & 0x10)
            func_00034808();
        D_000972D8 = 60;
        func_0007A580(D_000971F4, buf);
        func_0007A27C(27, buf);
    }
}

/* ---------------- ready_cb_regions (src/pilot/ready_cb_regions.c) ---------------- */
/* ready_cb_regions (Driver 2) */
void ready_cb_regions(unsigned char intr, unsigned char *result)
{
    char buf[8];
    SPOOLQ *q;
    if (intr == 1) {
        func_0007A4FC(D_000971F0, 512);
        D_000971F0 += 0x800;
        D_000971FC--;
        D_000971F8--;
        D_000971F4++;
        if (D_000971FC == 0) {
            D_000969EC++;
            D_00097200 = intr;
            if (D_000971F8 != 0) {
                q = &D_000A7BA4[D_000969EC];
                D_000971F0 = q->addr;
                D_000971FC = q->nsectors;
            } else {
                D_00097224 = intr;
                test_changemode();
            }
        }
    } else {
        if (*result & 0x10)
            func_00034808();
        D_000972D8 = 60;
        func_0007A580(D_000971F4, buf);
        func_0007A27C(27, buf);
    }
}

/* ---------------- data_cb_regions (src/pilot/data_cb_regions.c) ---------------- */
/* data_cb_regions */
void data_cb_regions(void)
{
    SPOOLQ *cur;
    if (D_00097200 == 0) return;
    cur = &D_000A7BA4[spoolpos_writing];
    D_00097200 = 0;
    if (cur->func != 0) cur->func();
    spoolpos_writing++;
    if (D_00097224 == 0) return;
    if (D_00097204 != 0) {
        D_00097204 = 0;
        D_00097224 = 0;
        switch (D_000A7BA4[spoolpos_writing].type) {
        case 0: func_0007A53C(data_cb_regions); break;
        case 1: func_0007A53C(data_cb_textures); break;
        case 3: func_0007A53C(data_cb_misc); break;
        case 2: func_0007A53C(func_00034EE4); break;
        }
    } else {
        func_0007A53C(0);
        if (spoolpos_writing == spoolcounter) {
            D_000972D4 = D_000969EC = spoolpos_writing = spoolcounter = 0;
        } else {
            UpdateSpool();
        }
    }
}

/* ---------------- func_00034EE4 (src/pilot/func_00034EE4.c) ---------------- */
/* Driver 2: data_cb_soundbank (weak match) */
void func_00034EE4(void)
{
    SPOOLQ *q;
    if (D_00097200) {
        q = D_000A7BA4;
        D_00097200 = 0;
        spoolpos_writing;
        SendSBK();
        D_00097244++;
        if (D_00097224) {
            spoolpos_writing++;
            if (D_00097204) {
                D_00097204 = 0;
                D_00097224 = 0;
                switch (q[spoolpos_writing].type) {
                case 0:
                    func_0007A53C(data_cb_regions);
                    break;
                case 1:
                    func_0007A53C(data_cb_textures);
                    break;
                case 3:
                    func_0007A53C(data_cb_misc);
                    break;
                case 2:
                    func_0007A53C(func_00034EE4);
                    break;
                }
            } else {
                func_0007A53C(0);
                if (spoolpos_writing == spoolcounter) {
                    spoolcounter = 0;
                    spoolpos_writing = spoolcounter;
                    D_000969EC = spoolpos_writing;
                    D_000972D4 = D_000969EC;
                } else {
                    UpdateSpool();
                }
            }
        }
    }
}

/* ---------------- ready_cb_soundbank (src/pilot/ready_cb_soundbank.c) ---------------- */
/* ready_cb_soundbank */
void ready_cb_soundbank(unsigned char intr, unsigned char *result)
{
    int buf[2];
    if (intr == 1) {
        int n; SPOOLQ *q;
        func_0007A4FC(D_000971F0, 0x200);
        D_000971F0 += 0x800;
        D_000971FC--;
        n = D_000971F8 - 1;
        D_000971F8 = n;
        D_000971F4++;
        if (D_000971FC == 0) {
            D_00097268++;
            q = &D_000A7BA4[D_000969EC];
            D_00097200 = intr;
            D_0009720C++;
            if (n == 0) {
                D_00097224 = intr;
                D_000969EC++;
                test_changemode();
            } else {
                D_000971FC = 2;
                if (n < 2) D_000971FC = n;
                D_000971F0 = (int)q->addr + ((D_00097268 & 1) << 12);
            }
        }
    } else {
        if (*result & 0x10) func_00034808();
        D_000972D8 = 0x3C;
        func_0007A580(D_000971F4, buf);
        func_0007A27C(0x1B, buf);
    }
}

/* ---------------- data_cb_misc (src/pilot/data_cb_misc.c) ---------------- */
/* Driver 2: data_cb_misc */
void data_cb_misc(void)
{
    void (*f)();
    if (D_00097200 == 0) return;
    f = D_000A7BA4[spoolpos_writing].func;
    D_00097200 = 0;
    if (f != 0) (*f)();
    spoolpos_writing++;
    if (D_00097204 != 0) {
        D_00097204 = 0;
        D_00097224 = 0;
        switch (D_000A7BA4[spoolpos_writing].type) {
        case 0: func_0007A53C(data_cb_regions); break;
        case 1: func_0007A53C(data_cb_textures); break;
        case 3: func_0007A53C(data_cb_misc); break;
        case 2: func_0007A53C(func_00034EE4); break;
        }
    } else {
        func_0007A53C(0);
        if (spoolpos_writing == spoolcounter) {
            spoolcounter = 0;
            spoolpos_writing = spoolcounter;
            D_000969EC = spoolpos_writing;
            D_000972D4 = D_000969EC;
        } else UpdateSpool();
    }
}

/* ---------------- func_000352D4 (src/pilot/func_000352D4.c) ---------------- */
void func_000352D4(unsigned char a, unsigned char *b)
{
    char buf[8];
    if (a == 1) {
        func_0007A4FC(D_000971F0, 0x200);
        D_000971F0 += 0x800;
        D_000971F8--;
        D_000971F4++;
        if (D_000971F8 == 0) {
            D_00097200 = a;
            D_000969EC++;
            test_changemode();
        }
    } else {
        if (*b & 0x10) func_00034808();
        D_000972D8 = 0x3C;
        func_0007A580(D_000971F4, buf);
        func_0007A27C(0x1B, buf);
    }
}

/* ---------------- StartSpooling (src/pilot/StartSpooling.c) ---------------- */
/* Driver 2: StartSpooling */
void StartSpooling(void) {
    int v;
    if (spoolcounter == 0 || D_000972D4 != 0) return;
    D_000971E0[0] = 0x80;
    CdControl(14, D_000971E0, D_000971E8);
    if (D_000971E8[0] & 0x15) func_00034808();
    D_000972D4 = 1;
    UpdateSpool();
    v = *(unsigned char *)&FastForward;
loop: if (!v) return; v = D_000972D4; goto loop;
}

/* ---------------- func_0003542C (src/pilot/func_0003542C.c) ---------------- */
void func_0003542C(void) {
    short *out, *src, *t;
    int n, c, v;
    int idx = D_000972BC;
    D_000972A4 = 0;
    t = D_0009E724[D_000A7EA4[idx].target_barrel_region];
    src = D_0009725C = (short *)D_000A7EA4[idx].roadm_addr;
    out = D_00097258 = t;
    n = 0xE10;
    if (*(int *)src == -1) {
        do {
            *out++ = -1;
        } while (--n);
    } else {
        do {
            c = src[0];
            v = src[1];
            src += 2;
            n -= c;
            for (; c > 0; c--) *out++ = v;
        } while (n > 0);
    }
}

/* ---------------- func_000354D8 (src/pilot/func_000354D8.c) ---------------- */
void func_000354D8(void) {
    int *out, *src, *t;
    int n, c, v;
    int idx = D_000972BC;
    D_000972A8 = 0;
    t = D_0009D8F4[D_000A7EA4[idx].target_barrel_region];
    src = D_000972CC = (int *)D_000A7EA4[idx].roadh_addr;
    out = D_000972C8 = t;
    n = 0xE10;
    do {
        c = src[0];
        v = src[1];
        src += 2;
        n -= c;
        for (; c != 0; c--) *out++ = v;
    } while (n != 0);
}

/* ---------------- func_00035558 (src/pilot/func_00035558.c) ---------------- */
#define OUT (*(unsigned short **)&D_000970E0)
static inline void fill(int k) { OUT[k] = 0xFFFF; }
void func_00035558()
{
    int idx = D_000A7F44.idx;
    int n = *(int *)&D_0009733C;
    int base = idx * n * n;
    int *d;
    int mode;
    unsigned short *src;
    int i;

    D_000972A0 = 0;
    d = D_000A7F44.d;
    d += 1;
    mode = *d;
    d += 1;
    src = (unsigned short *)d;
    if (mode == 0) {
        /* guarded do-while: a plain for lets cse_around_loop drop the bound copy */
        if (n * n > 0) {
            i = 0;
            do {
                OUT[base + i] = 0xFFFF;
            } while (++i < n * n);
        }
    } else if (mode == 1) {
        for (i = 0; i < n * n; i++) {
            unsigned short s = *src++;
            if (s != 0xFFFF)
                s += *(unsigned short *)&D_000AA7D4[idx];
            OUT[base + i] = s;
        }
    } else if (mode == 2) {
        int bit = 0x8000;
        unsigned short flags = *src++;
        for (i = 0; i < n * n; i++) {
            unsigned short s;
            if (flags & bit) {
                s = *src++;
                s += *(unsigned short *)&D_000AA7D4[idx];
            } else
                s = 0xFFFF;
            OUT[base + i] = s;
            bit >>= 1;
            if (!bit) {
                bit = 0x8000;
                flags = *src++;
            }
        }
    } else {
        printf("Ulp!!! error\n");
        while (*(volatile int *)&D_00097534 != 0x78654321)
            __asm__ volatile("break 1024");
    }
}
#undef OUT

/* ---------------- func_0003575C (src/pilot/func_0003575C.c) ---------------- */
void func_0003575C(void) {
    int i = D_000972BC;
    int a = D_000A7EA4[i].region_to_unpack;
    int b = D_000A7EA4[i].target_barrel_region;
    int c = (int)D_000A7EA4[i].cell_addr;
    D_000A7F44.a = a;
    D_000A7F44.idx = b;
    D_000A7F44.d = (int *)c;
    func_00035558(b, c);
}
