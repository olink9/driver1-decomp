/* Boundary evidence (pilot/TU1.md, FRONTEND section):
 * FE init (FESAMPS.SBK bank, display env, SLIDE/BUTT sprites, menu driver func_001C2114 = wip, INCLUDE_ASM) and
 * the loader func_001C2F78 (fonts, FRNT.B%d, tpdata, MCARD.BIN; D2 LoadFrontendScreens analog). START: text pad
 * after func_001C19C4, .rodata pad 0x1C0144. END: text pad at 0x1C352C, .rodata pad 0x1C025C. Own object: its
 * literals (gfx\font1.bnk, DRFONT1-4, MCARD.BIN, FRNT\FRNT.B%d, GFX\GLOAD.TIM) are duplicated in options.c and
 * ladview.c - gcc 2.7 reuses a literal only inside one object. .rodata 2 pieces 0x1C0148/0x1C0194: the hole is its
 * own wip jtbl 0x1C0170 + const struct D_001C0188 (still asm).
 */
/* fecore.c: 12 functions func_001C1B00..func_001C334C (0x1C1B00-0x1C3530)
 * Step 3: types from include/frontend.h + structs.h (MENU_ITEM, MENU_TEXT, DB/DB_ENV, TP_SHEET, TEXINFO, prims).
 * No views left.
 */

/* include_asm.h's INCLUDE_ASM minus its .include of labels.inc (the build pipeline prepends macro.inc) */
#define INCLUDE_ASM(FOLDER, NAME) \
    __asm__(".section .text\n.set noat\n.set noreorder\n.include \"" FOLDER "/" #NAME ".s\"\n.set reorder\n.set at\n")

/* ---- types ---- */
#include "frontend.h"
typedef struct { int n[3]; } N3;

/* ---- externs ---- */
extern int D_001CDE88;
extern int D_001CDE8C;
extern void SetDispMask();
extern void func_00051374();
extern int D_00097098;
extern int D_00096C94;
extern void func_0004D5F8();
extern int Loadfile();
extern void LoadSoundBank();
extern void SetMasterVolume();
extern void func_0004E08C();
extern void func_0006F2DC();
extern void func_0006F57C();
extern void func_000E7350();
extern void func_000E6F14();
extern void func_000E6F98();
extern POLY_FT4 D_001D2C70[];
extern POLY_FT4 D_001D2CC0[];
extern int func_0007A7F4();
extern DB *current;
extern int D_001CDE5C;
extern void memset();
extern void func_001C11E4();
extern void func_001C18AC();
extern void func_001C19C4();
extern void func_001C49C8();
extern void func_001C5EE0();
extern int D_001CFDE8;
extern int D_00096A08;
extern int D_00096C90;
extern int D_001CDECC;
extern int D_001CDED0;
extern int D_001CDE68;
extern int D_001CDC80;
extern int D_000967D8;
extern unsigned short D_00097538;
extern short D_0009752C;
extern unsigned char D_001CDEE0[];
extern int D_0008671C[][14];
extern int func_0006F6DC();
extern void func_0006F5CC();
extern void func_0006F478();
extern void StartSound();
extern short D_001CDE98[];
extern short D_001CDEA8[];
extern int D_001CDEC8;
extern int D_001CDEC0;
extern int D_001CDEC4;
extern int D_001CDEB8;
extern int D_001CDEBC;
extern char D_000BDFB4[];
extern DB *D_00097524;
extern void func_00079800();
extern void func_0007D66C();
extern void func_0007D720();
extern void PutDispEnv();
extern void PutDrawEnv();
extern N3 D_001C0188;
extern TP_SHEET D_001CFDF0[];
extern RECT D_000976A4;
extern int D_000967C0;
extern int D_000D0200;
extern void func_00073F88();
extern void ShowLoadingScreen();
extern void func_000505E0();
extern void LoadFont();
extern void func_00074404();
extern void DrawSync();
extern void func_0007D608();
extern void func_001C1270();
extern int sprintf();
extern void func_000E6D60(char *file, char *name, unsigned char idx, short val);
extern int printf();
extern int func_0007EC88();
extern int func_0007B218();

/* ---- functions defined in this file ---- */
int func_001C1B00();
void func_001C1B4C();
void func_001C1BD4();
void func_001C1C4C();
void func_001C1F40();
void func_001C2014();
void func_001C2114();
void func_001C2644();
int func_001C26E4();
void func_001C2DAC();
void func_001C2F78();
void func_001C334C();

/* ---------------- func_001C1B00 (src/pilot/frontend/func_001C1B00.c) ---------------- */
int func_001C1B00(void){ SetDispMask(0); func_001C2DAC(0x280); func_001C2F78(); func_001C1B4C(); D_001CDE88 = 0; D_001CDE8C = 0; func_00051374(); return 1; }

/* ---------------- func_001C1B4C (src/pilot/frontend/func_001C1B4C.c) ---------------- */
/* FAKEMATCH: the unused `char buf[128]` only reproduces the 0x98 frame (0x80 gap); the original probably used it in code that was compiled out. */
void func_001C1B4C(void)
{
    char buf[128];
    func_0004D5F8(0);
    D_00097098 = 0x114600;
    LoadSoundBank(D_00097098, Loadfile("SOUND\\FESAMPS.SBK", 0x114600), 0);
    SetMasterVolume(D_00096C94);
    func_0004E08C();
    func_0006F2DC(0, -1);
    func_0006F57C();
}

/* ---------------- func_001C1BD4 (src/pilot/frontend/func_001C1BD4.c) ---------------- */
void func_001C1BD4(MENU_TEXT *p)
{
    MENU_TEXT *q = p;
    func_000E7350();
    while (q->text != 0) {
        func_000E6F14(q->x, q->y, q->font, q->justify, &q->col);
        func_000E6F98(q->text);
        q++;
    }
}

/* ---------------- func_001C1C4C (src/pilot/frontend/func_001C1C4C.c) ---------------- */
void func_001C1C4C(void)
{
    TEX tex;
    POLY_FT4 *p, *q, *r, *t;

    func_001C334C("SLIDEPSX", &tex);
    p = &D_001D2C70[0];
    q = &D_001D2C70[1];
    ((P_TAG *)p)->len = 9;
    p->code = 0x2C;
    ((P_TAG *)q)->len = 9;
    q->code = 0x2C;
    p->r0 = 128; p->g0 = 128; p->b0 = 128;
    q->r0 = 128; q->g0 = 128; q->b0 = 128;
    p->code |= 2;
    q->code |= 2;
    p->code &= ~1;
    q->code &= ~1;
    p->x0 = 0; p->y0 = 0; p->x1 = tex.u1 - tex.u0 + 1; p->y1 = 0;
    p->x2 = 0; p->y2 = tex.v2 - tex.v0 + 1; p->x3 = tex.u1 - tex.u0 + 1; p->y3 = tex.v2 - tex.v0 + 1;
    q->x0 = 0; q->y0 = 0; q->x1 = tex.u1 - tex.u0 + 1; q->y1 = 0;
    q->x2 = 0; q->y2 = tex.v2 - tex.v0 + 1; q->x3 = tex.u1 - tex.u0 + 1; q->y3 = tex.v2 - tex.v0 + 1;
    p->tpage = tex.tpage;
    D_001D2C70[1].tpage = tex.tpage;
    p->clut = func_0007A7F4(960, tex.clut + 256);
    q->clut = func_0007A7F4(960, tex.clut + 256);
    p->u0 = tex.u0; p->v0 = tex.v0; p->u1 = tex.u1; p->v1 = tex.v1;
    p->u2 = tex.u2; p->v2 = tex.v2; p->u3 = tex.u3; p->v3 = tex.v3;
    q->u0 = tex.u0; q->v0 = tex.v0; q->u1 = tex.u1; q->v1 = tex.v1;
    q->u2 = tex.u2; q->v2 = tex.v2; q->u3 = tex.u3; q->v3 = tex.v3;

    func_001C334C("BUTTPSX", &tex);
    r = &D_001D2CC0[0];
    t = &D_001D2CC0[1];
    ((P_TAG *)r)->len = 9;
    r->code = 0x2C;
    ((P_TAG *)t)->len = 9;
    t->code = 0x2C;
    r->r0 = 128; r->g0 = 128; r->b0 = 128;
    t->r0 = 128; t->g0 = 128; t->b0 = 128;
    r->code |= 2;
    t->code |= 2;
    r->code &= ~1;
    t->code &= ~1;
    r->x0 = 0; r->y0 = 0; r->x1 = tex.u1 - tex.u0 + 1; r->y1 = 0;
    r->x2 = 0; r->y2 = tex.v2 - tex.v0 + 1; r->x3 = tex.u1 - tex.u0 + 1; r->y3 = tex.v2 - tex.v0 + 1;
    t->x0 = 0; t->y0 = 0; t->x1 = tex.u1 - tex.u0 + 1; t->y1 = 0;
    t->x2 = 0; t->y2 = tex.v2 - tex.v0 + 1; t->x3 = tex.u1 - tex.u0 + 1; t->y3 = tex.v2 - tex.v0 + 1;
    r->tpage = tex.tpage;
    D_001D2CC0[1].tpage = tex.tpage;
    r->clut = func_0007A7F4(960, tex.clut + 256);
    t->clut = func_0007A7F4(960, tex.clut + 256);
    r->u0 = tex.u0; r->v0 = tex.v0; r->u1 = tex.u1; r->v1 = tex.v1;
    r->u2 = tex.u2; r->v2 = tex.v2; r->u3 = tex.u3; r->v3 = tex.v3;
    t->u0 = tex.u0; t->v0 = tex.v0; t->u1 = tex.u1; t->v1 = tex.v1;
    t->u2 = tex.u2; t->v2 = tex.v2; t->u3 = tex.u3; t->v3 = tex.v3;
}

/* ---------------- func_001C1F40 (src/pilot/frontend/func_001C1F40.c) ---------------- */
/* func_001C1F40: PsyQ addPrim macro (P_TAG bitfield) on current->ot[k], plain for loop. */
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (unsigned)(_addr))
#define getaddr(p) (unsigned)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
void func_001C1F40(int n)
{
    int i;
    for (i = 0; i < n; i++) {
        addPrim(current->ot[5], &D_001D2C70[i]);
        addPrim(current->ot[4], &D_001D2CC0[i]);
    }
}
#undef addPrim
#undef getaddr
#undef setaddr

/* ---------------- func_001C2014 (src/pilot/frontend/func_001C2014.c) ---------------- */
/* func_001C2014: draw a flat POLY_F4 box (setPolyF4, setXYWH order, setRGB0, addPrim).
   FAKEMATCH: char pad[32] only sizes the 0x20 frame (no saves; probably an unused local aggregate). */
void func_001C2014(int x, int y, unsigned char *c, int w, int h, int z)
{
    char pad[32];
    POLY_F4 *p;
    x += D_001CDE88;
    y += D_001CDE8C;
    p = (POLY_F4 *)current->primptr;
    ((unsigned char *)p)[3] = 5;
    ((unsigned char *)p)[7] = 0x29;
    p->x0 = x;
    p->y0 = y;
    p->x1 = x + w;
    p->y1 = y;
    p->x2 = x;
    p->y2 = y + h;
    p->x3 = x + w;
    p->y3 = y + h;
    p->r0 = c[0];
    p->g0 = c[1];
    p->b0 = c[2];
    ((P_TAG *)p)->addr = ((P_TAG *)current->ot[z])->addr;
    ((P_TAG *)current->ot[z])->addr = (unsigned)p;
    current->primptr += 24;
}

/* ---------------- func_001C2114 (WIP: still asm; the C draft is src/pilot/frontend/func_001C2114.wip.c) ---------------- */
INCLUDE_ASM("asm_fn/nonmatchings/frontend", func_001C2114);

/* ---------------- func_001C2644 (src/pilot/frontend/func_001C2644.c) ---------------- */
void func_001C2644(MENU_ITEM *t, int n, int idx)
{
    int i;
    D_001CFDE8 = 1;
    for (i = 0; i < n; i++) {
        t[i].leave();
    }
    t[idx].enter();
    D_001CFDE8 = 0;
}

/* ---------------- func_001C26E4 (src/pilot/frontend/func_001C26E4.c) ---------------- */
int func_001C26E4(MENU_ITEM *m, int *sel)
{
    int moved = 0;
    int ret = 0;
    int old = *sel;
    int mode;
    int i;

    D_001CDED0 = 0;
    mode = D_00096A08;
    if (mode != 1)
        return 0;
    if (!func_0006F6DC()) {
        func_0006F5CC(8, 4, 7);
        func_0006F478(D_00096C90);
    }
    if (D_00097538 & 0x10) {
        m[old].leave();
        if (D_001CDE5C != 1)
            StartSound(0, 0, 0, D_00096C94, 0x1000);
        return -1;
    }
    if (D_001CDECC) {
        if (D_0009752C & 0x8000) {
            m[m[*sel].left].enter();
        } else if ((unsigned short)D_0009752C & 0x2000) {
            m[m[*sel].right].enter();
        } else if (D_00097538 & 0x1000) {
            if (*sel != m[*sel].up)
                D_001CDED0 = 1;
            *sel = m[*sel].up;
            moved = 1;
        } else if (D_00097538 & 0x4000) {
            if (*sel != m[*sel].down)
                D_001CDED0 = 1;
            *sel = m[*sel].down;
            moved = 1;
        } else if (D_00097538 & 0x40) {
            StartSound(0, 0, 2, D_00096C94, 0x1000);
            ret = m[*sel].action();
        }
    } else {
        if (D_00097538 & 0x4) {
            for (i = 0; i < D_000967D8; i++) {
                if (D_0008671C[i][D_001CDEE0[i]] == 0)
                    D_001CDEE0[i]++;
                else
                    D_001CDEE0[i] = 0;
            }
        } else if (D_00097538 & 0x1) {
            for (i = 0; i < D_000967D8; i++) {
                if (D_0008671C[i][D_001CDEE0[i]] == 1)
                    D_001CDEE0[i]++;
                else
                    D_001CDEE0[i] = 0;
            }
        } else if (D_00097538 & 0x8) {
            for (i = 0; i < D_000967D8; i++) {
                if (D_0008671C[i][D_001CDEE0[i]] == 2)
                    D_001CDEE0[i]++;
                else
                    D_001CDEE0[i] = 0;
            }
        } else if (D_00097538 & 0x2) {
            for (i = 0; i < D_000967D8; i++) {
                if (D_0008671C[i][D_001CDEE0[i]] == 3)
                    D_001CDEE0[i]++;
                else
                    D_001CDEE0[i] = 0;
            }
        } else if (D_00097538 & 0x8000) {
            if (*sel != m[*sel].left)
                D_001CDED0 = 1;
            *sel = m[*sel].left;
            moved = 1;
        } else if (D_00097538 & 0x2000) {
            if (*sel != m[*sel].right)
                D_001CDED0 = 1;
            *sel = m[*sel].right;
            moved = 1;
        } else if (D_00097538 & 0x1000) {
            if (*sel != m[*sel].up)
                D_001CDED0 = 1;
            *sel = m[*sel].up;
            moved = 1;
        } else if (D_00097538 & 0x4000) {
            if (*sel != m[*sel].down)
                D_001CDED0 = 1;
            *sel = m[*sel].down;
            moved = 1;
        } else if (D_00097538 & 0x40) {
            StartSound(0, 0, 2, D_00096C94, 0x1000);
            ret = m[*sel].action();
        }
    }
    if (moved) {
        D_001CDE68 = 0;
        D_001CDC80 = 1;
        m[old].leave();
        m[*sel].enter();
        if (D_001CDED0)
            StartSound(0, 0, 0, D_00096C94, 0x1000);
    } else {
        D_001CDC80 = 0;
    }
    for (i = 0; i < D_000967D8; i++) {
        if (D_0008671C[i][D_001CDEE0[i]] == -1) {
            StartSound(0, 0, 2, D_00096C94, 0x1000);
            D_0008671C[i][D_001CDEE0[i]] = -2;
            D_0008671C[i][0] = -2;
        }
    }
    return ret;
}

/* ---------------- func_001C2DAC (src/pilot/frontend/func_001C2DAC.c) ---------------- */
/* func_001C2DAC. FAKEMATCH: the empty volatile asm at the top is a sched2 barrier that keeps the four prologue saves
   above the lui/lw of D_001CDEC8 (no C form found: volatile, [], struct, switch, inline helper, no param all tried). */
void func_001C2DAC(int unused)
{
    short *t;
    int mode;
    char *p;
    DB_ENV *db;

    __asm__ volatile(""); /* FAKEMATCH: sched barrier */
    if (D_001CDEC8 == 1) {
        t = D_001CDE98;
        mode = 1;
        D_001CDEC0 = 50;
        D_001CDEC4 = 302;
    } else {
        t = D_001CDEA8;
        mode = 0;
        D_001CDEC0 = 60;
        D_001CDEC4 = 262;
    }
    func_00079800(mode);
    p = D_000BDFB4;
    func_0007D66C(p, t[0], t[1], t[4], t[5]);
    func_0007D720(p + 0x5C, t[0], t[1], t[4], t[5]);
    func_0007D66C(p + 0x80, t[0], t[1], t[4], t[5]);
    func_0007D720(p + 0xDC, t[0], t[1], t[4], t[5]);
    D_001CDEB8 = t[4];
    D_001CDEBC = t[5];
    db = (DB_ENV *)(p - 0x10);
    db[0].draw.isbg = 0;
    db[0].disp.isinter = 1;
    db[0].draw.dfe = 0;
    db[0].disp.screen.x = t[6];
    db[0].disp.screen.y = t[7];
    db[0].disp.screen.h = t[5];
    current = (DB *)&db[0];
    db[0].primtab = (char *)0xD8600;
    db[0].primptr = (char *)0xD8600;
    db[0].ot = (unsigned long *)0xD0200;
    db[1].draw.isbg = 0;
    db[1].disp.isinter = 1;
    db[1].draw.dfe = 0;
    db[1].disp.screen.x = t[6];
    D_00097524 = (DB *)&db[1];
    db[1].disp.screen.y = t[7];
    db[1].disp.screen.h = t[5];
    db[1].primtab = (char *)0xF6600;
    db[1].primptr = (char *)0xF6600;
    db[1].ot = (unsigned long *)0xD4400;
    PutDispEnv(&db[0].disp);
    PutDrawEnv(&((DB_ENV *)current)->draw);
}

/* ---------------- func_001C2F78 (src/pilot/frontend/func_001C2F78.c) ---------------- */
/* FAKEMATCH: unused RECT r2 only sizes the frame (counts at sp+0x20). Only fnscore diff: target reloc %hi/%lo(D_000D0200) vs literal 0xD0200 (byte-identical; buf is the 0xD0200 load address, as in mcard func_000EC280). */
#define getTPage(tp, abr, x, y) \
    ((((tp) & 0x3) << 7) | (((abr) & 0x3) << 5) | (((y) & 0x100) >> 4) | (((x) & 0x3ff) >> 6) | (((y) & 0x200) << 2))
void func_001C2F78(void)
{
    RECT r;
    RECT r2; /* FAKEMATCH: frame size */
    N3 counts;
    char name[16];
    unsigned char *p;
    short *e;
    int *buf;
    int i, j, k, n, x, y;

    counts = D_001C0188;
    func_00073F88(1);
    func_00079800(0);
    SetDispMask(0);
    if (D_000967C0 != 0) {
        ShowLoadingScreen("GFX\\MLOAD.TIM", 1, 7);
        func_000505E0();
        D_000967C0 = 0;
    } else {
        ShowLoadingScreen("GFX\\GLOAD.TIM", 1, 7);
        func_000505E0();
    }
    Loadfile("frnt\\tpdata.dat", D_001CFDF0);
    for (j = 31; j >= 0; j--)
        D_001CFDF0[j].used = 0;
    func_000505E0();
    r.x = 0x3C0;
    r.y = 0x100;
    r.w = 16;
    r.h = 1;
    D_000976A4.x = 0x3D0;
    D_000976A4.y = 0x103;
    D_000976A4.w = 16;
    D_000976A4.h = 1;
    LoadFont(&r);
    k = 0;
    r.w = 64;
    r.h = 256;
    for (j = 0; j < 3; j++) {
        sprintf(name, "FRNT\\FRNT.B%d", j + 1);
        Loadfile(name, 0xD8600);
        p = (unsigned char *)0xD8600;
        p += *p;
        for (i = 0; i < counts.n[j]; i++) {
            r.x = (k % 11) * 64 + 320;
            r.y = (k / 11) << 8;
            D_001CFDF0[k].tpage = getTPage(0, 0, r.x, r.y);
            D_001CFDF0[k].slot = k;
            D_001CFDF0[k].used = 1;
            func_00074404(&r, p);
            DrawSync(0);
            p += 0x8000;
            k++;
        }
        func_000505E0();
        k--;
    }
    Loadfile("MCARD.BIN", 0xE6600);
    func_000505E0();
    buf = (int *)0xD0200;
    Loadfile("frnt\\feclut.clt", buf);
    e = (short *)buf;
    n = *(int *)e;
    e += 2;
    x = 0x3C0;
    y = 0x100;
    for (k = 0; k < n; k++) {
        if (*e == 0x80)
            *e = -1;
        func_0007D608(e, x, y);
        DrawSync(0);
        e += 16;
        y++;
    }
    func_000E6D60("gfx\\font1.bnk", "DRFONT1", 0, -2);
    func_000505E0();
    func_000E6D60("gfx\\font1.bnk", "DRFONT2", 1, -2);
    func_000505E0();
    func_000E6D60("gfx\\font3.bnk", "DRFONT3", 2, -2);
    func_000505E0();
    func_000E6D60("gfx\\font3.bnk", "DRFONT4", 3, -2);
    func_001C1270();
}
#undef getTPage

/* ---------------- func_001C334C (src/pilot/frontend/func_001C334C.c) ---------------- */
/* strlen */
/* strncmp */
/* find a texture by name in the loaded sheets */
void func_001C334C(char *name, TEXINFO *out)
{
    char buf[16];
    int k;
    int i;
    int n;

    for (k = 0; k < 32; k++) {
        n = D_001CFDF0[k].count;
        for (i = 0; i < n; i++) {
            sprintf(buf, "%s.TIM", name);
            if (func_0007B218(buf, D_001CFDF0[k].e[i].name, func_0007EC88(buf)) == 0) {
                out->tpage = D_001CFDF0[k].tpage;
                out->slot = D_001CFDF0[k].slot;
                out->idx = i;
                out->u0 = D_001CFDF0[k].e[i].x * 4;
                out->v0 = D_001CFDF0[k].e[i].y;
                out->u1 = (D_001CFDF0[k].e[i].x + D_001CFDF0[k].e[i].w) * 4 - 1;
                out->v1 = D_001CFDF0[k].e[i].y;
                out->u2 = D_001CFDF0[k].e[i].x * 4;
                out->v2 = D_001CFDF0[k].e[i].y + D_001CFDF0[k].e[i].h - 1;
                out->u3 = (D_001CFDF0[k].e[i].x + D_001CFDF0[k].e[i].w) * 4 - 1;
                out->v3 = D_001CFDF0[k].e[i].y + D_001CFDF0[k].e[i].h - 1;
                out->clut = D_001CFDF0[k].e[i].clut;
                return;
            }
        }
    }
    printf("(details) couldn't find texture %s\n", name);
    for (;;)
        __asm__ volatile("break 1024");
}

