/* Boundary evidence (pilot/TU1.md, FRONTEND section):
 * FE entry func_001C0AE8 (main jal 0x1C0AE8), main-menu handler table D_001CDDDC (24 handlers), buffer flip,
 * MCARICO3 icons, menu memsets. END: pad nop at 0x1C1AFC (each object's .text is 8-aligned), .rodata pad word
 * 0x1C0144 after "MCARICO3", last .data D_001CDE6C before fecore's D_001CDE88. .rodata one block 0x1C0138 (the
 * menu-table strings 0x1C0000-0x1C012C come first, still asm: their .data is extern).
 */
/* femain.c: 30 functions func_001C0AE8..func_001C19C4 (0x1C0AE8-0x1C1B00)
 * Step 3: types from include/frontend.h + structs.h (DB *current/D_00097524, DB_ENV for the envs, POLY_FT4, TEX).
 * FE_ICON (D_001CDCD8, 0x14-byte icon records) and FE_ICON_PRIMS (D_001CF888, 3 prims per icon) are file-local.
 * No views.
 */

/* ---- types ---- */
#include "frontend.h"
typedef struct { short x, y; char *name; short h8; short pad[4]; short h12; } FE_ICON;
typedef struct { POLY_FT4 m[3]; } FE_ICON_PRIMS;

/* ---- externs ---- */
extern int D_000967C8;
extern int D_00096C90;
extern int D_001CDE60;
extern int D_001CDE68;
extern int D_001CFDB0;
extern MENU_ITEM D_001CDDDC[];
extern FE_ICON D_001CDCD8[];
extern MENU_TEXT D_001CDC84[];
extern void func_00073F88();
extern void SetDispMask();
extern void func_001C2644();
extern void func_0006F5CC();
extern void func_0006F478();
extern void func_000514E0();
extern void func_001C2114();
extern void func_001C1BD4();
extern int func_001C1B00();
extern int func_001C26E4();
extern short D_001CDD1C;
extern int D_001CDCC0;
extern int D_001CDC8C;
extern int D_000967F0;
extern int D_000967F4;
extern int D_001CDE5C;
extern void func_001C8D08();
extern int DrawSync();
extern int VSync();
extern void func_00024754();
extern short D_001CDD30;
extern int D_001CDCC4;
extern int func_001C5A30();
extern short D_001CDD6C;
extern int D_001CDCD0;
extern int D_001CDE64;
extern int func_001C41D8();
extern short D_001CDD08;
extern int D_001CDCBC;
extern int func_001C6DE0();
extern int D_001CDCD4;
extern int func_001C3530();
extern short D_001CDD44;
extern int D_001CDCC8;
extern int func_001C4D04();
extern short D_001CDD58;
extern int D_001CDCCC;
extern int func_001C70F0();
extern short D_001CDCF4;
extern int D_001CDCB8;
extern int func_001C7CA8();
extern int D_000967D0;
extern int D_00096808;
extern int D_00096834;
extern DB *current;
extern DB *D_00097524;
extern void PutDispEnv();
extern void PutDrawEnv();
extern void func_00074684();
extern void func_0007457C();
extern char *D_001CDE6C[];
extern FE_ICON_PRIMS D_001CF888[];
extern void func_001C334C();
extern int func_0007A7F4();
extern void memset();
extern void func_001C2014();

/* ---- functions defined in this file ---- */
void func_001C0AE8();
void func_001C0CCC();
void func_001C0CEC();
int func_001C0CF8();
void func_001C0DA4();
void func_001C0DD0();
int func_001C0DDC();
void func_001C0E2C();
void func_001C0E58();
int func_001C0E64();
void func_001C0EE8();
void func_001C0F10();
int func_001C0F1C();
void func_001C0F6C();
void func_001C0F8C();
int func_001C0F98();
void func_001C0FE8();
void func_001C1008();
int func_001C1014();
void func_001C106C();
void func_001C108C();
int func_001C1098();
void func_001C10E4();
void func_001C1104();
int func_001C1110();
void func_001C1160();
void func_001C11E4();
void func_001C1270();
void func_001C18AC();
void func_001C19C4();

/* ---------------- func_001C0AE8 (src/pilot/frontend/func_001C0AE8.c) ---------------- */
void func_001C0AE8(void)
{
    int r;

    D_000967C8 = 1;
    func_00073F88(0);
    D_001CDE68 = 0;
    SetDispMask(0);
    if (func_001C1B00() == 0)
        return;
    func_001C2644(D_001CDDDC, 8, D_001CDE60);
    SetDispMask(0);
    for (;;) {
        if (D_001CFDB0 == 2) {
            func_0006F5CC(8, 4, 7);
            func_0006F478(D_00096C90);
            SetDispMask(1);
        }
        func_000514E0();
        r = func_001C26E4(D_001CDDDC, &D_001CDE60);
        if (r == -1) {
            D_001CDE68 = 0;
            func_001C2644(D_001CDDDC, 8, D_001CDE60);
        } else if (r > 0) {
            D_001CDE60 = r - 1;
            func_001C2644(D_001CDDDC, 8, r - 1);
            func_001C2114(D_001CDCD8, 0, 1);
            func_0006F5CC(8, 4, 7);
            func_0006F478(D_00096C90);
        } else if (r == -2) {
            D_001CDE68 = 0;
            D_001CFDB0 = 0;
        }
        func_001C18AC();
        func_001C1BD4(D_001CDC84);
        func_001C11E4();
        func_001C18AC();
        func_001C1BD4(D_001CDC84);
        func_001C11E4();
        if (D_001CDE68 >= 1500)
            func_001C1160();
        D_001CDE68++;
        D_001CFDB0++;
    }
}

/* ---------------- func_001C0CCC (src/pilot/frontend/func_001C0CCC.c) ---------------- */
void func_001C0CCC(void)
{
    D_001CDD1C = 1;
    D_001CDC8C = D_001CDCC0;
}

/* ---------------- func_001C0CEC (src/pilot/frontend/func_001C0CEC.c) ---------------- */
void func_001C0CEC(void)
{
    D_001CDD1C = 0;
}

/* ---------------- func_001C0CF8 (src/pilot/frontend/func_001C0CF8.c) ---------------- */
int func_001C0CF8(void)
{
    func_001C2114(D_001CDCD8, 0, 0);
    D_001CDE5C = 6;
    func_001C8D08();
    if (D_000967F0 != 0) {
        DrawSync(0);
        VSync(0);
        func_001C11E4();
        D_000967F4 = 16;
        func_001C2114(0, D_001CDC84, 0);
        SetDispMask(0);
        func_00024754();
    }
    D_001CDE5C = 1;
    return 2;
}

/* ---------------- func_001C0DA4 (src/pilot/frontend/func_001C0DA4.c) ---------------- */
void func_001C0DA4(void)
{
    D_001CDD30 = 1;
    D_000967F4 = 3;
    D_001CDC8C = D_001CDCC4;
}

/* ---------------- func_001C0DD0 (src/pilot/frontend/func_001C0DD0.c) ---------------- */
void func_001C0DD0(void){D_001CDD30 = 0;}

/* ---------------- func_001C0DDC (src/pilot/frontend/func_001C0DDC.c) ---------------- */
/* The callee returns int: its call_value sets v0, so v0 has 2 sets and sched1 gives `v0 = 3` no birthing boost. */
int func_001C0DDC(void){ func_001C2114(D_001CDCD8, 0, 0); D_001CDE5C = 3; func_001C5A30(); D_001CDE5C = 1; return 3; }

/* ---------------- func_001C0E2C (src/pilot/frontend/func_001C0E2C.c) ---------------- */
void func_001C0E2C(void)
{
    D_001CDD6C = 1;
    D_000967F4 = 4;
    D_001CDC8C = D_001CDCD0;
}

/* ---------------- func_001C0E58 (src/pilot/frontend/func_001C0E58.c) ---------------- */
void func_001C0E58(void){D_001CDD6C = 0;}

/* ---------------- func_001C0E64 (src/pilot/frontend/func_001C0E64.c) ---------------- */
int func_001C0E64(void)
{
    func_001C2114(D_001CDCD8, 0, 0);
    D_001CDE5C = 2;
    D_001CDE64 = 2;
    D_000967F4 = 0x17;
    if (func_001C41D8()) {
        D_000967F4 = 0x17;
        func_00024754();
    }
    D_001CDE5C = 1;
    return 6;
}

/* ---------------- func_001C0EE8 (src/pilot/frontend/func_001C0EE8.c) ---------------- */
void func_001C0EE8(void)
{
    D_001CDD08 = 1;
    D_000967F4 = 0;
    D_001CDC8C = D_001CDCBC;
}

/* ---------------- func_001C0F10 (src/pilot/frontend/func_001C0F10.c) ---------------- */
void func_001C0F10(void){D_001CDD08 = 0;}

/* ---------------- func_001C0F1C (src/pilot/frontend/func_001C0F1C.c) ---------------- */
/* The callee returns int (call_value -> 2 sets of v0 -> no birthing boost on the return set). */
int func_001C0F1C(void){ func_001C2114(D_001CDCD8, 0, 0); D_001CDE5C = 6; func_001C6DE0(); D_001CDE5C = 1; return 1; }

/* ---------------- func_001C0F6C (src/pilot/frontend/func_001C0F6C.c) ---------------- */
void func_001C0F6C(void)
{
    D_001CDD6C = 2;
    D_001CDC8C = D_001CDCD4;
}

/* ---------------- func_001C0F8C (src/pilot/frontend/func_001C0F8C.c) ---------------- */
void func_001C0F8C(void) { D_001CDD6C = 0; }

/* ---------------- func_001C0F98 (src/pilot/frontend/func_001C0F98.c) ---------------- */
/* The callee returns int (call_value -> 2 sets of v0 -> no birthing boost on the return set). */
int func_001C0F98(void){ func_001C2114(D_001CDCD8, 0, 0); D_001CDE5C = 6; func_001C3530(); D_001CDE5C = 1; return 7; }

/* ---------------- func_001C0FE8 (src/pilot/frontend/func_001C0FE8.c) ---------------- */
void func_001C0FE8(void)
{
    D_001CDD44 = 1;
    D_001CDC8C = D_001CDCC8;
}

/* ---------------- func_001C1008 (src/pilot/frontend/func_001C1008.c) ---------------- */
void func_001C1008(void) { D_001CDD44 = 0; }

/* ---------------- func_001C1014 (src/pilot/frontend/func_001C1014.c) ---------------- */
/* The callee returns int (call_value -> 2 sets of v0 -> no birthing boost on the return set). */
int func_001C1014(void){ func_001C2114(D_001CDCD8, 0, 0); D_001CDE5C = 4; D_001CDE64 = 0; func_001C4D04(); D_001CDE5C = 1; return 5; }

/* ---------------- func_001C106C (src/pilot/frontend/func_001C106C.c) ---------------- */
void func_001C106C(void)
{
    D_001CDD58 = 1;
    D_001CDC8C = D_001CDCCC;
}

/* ---------------- func_001C108C (src/pilot/frontend/func_001C108C.c) ---------------- */
void func_001C108C(void) { D_001CDD58 = 0; }

/* ---------------- func_001C1098 (src/pilot/frontend/func_001C1098.c) ---------------- */
int func_001C1098(void){ func_001C2114(D_001CDCD8, 0, 0); if (func_001C70F0() != -1) { D_000967F4 = 2; func_00024754(); } return 4; }

/* ---------------- func_001C10E4 (src/pilot/frontend/func_001C10E4.c) ---------------- */
void func_001C10E4(void)
{
    D_001CDCF4 = 1;
    D_001CDC8C = D_001CDCB8;
}

/* ---------------- func_001C1104 (src/pilot/frontend/func_001C1104.c) ---------------- */
void func_001C1104(void) { D_001CDCF4 = 0; }

/* ---------------- func_001C1110 (src/pilot/frontend/func_001C1110.c) ---------------- */
/* The callee returns int (call_value -> 2 sets of v0 -> no birthing boost on the return set). */
int func_001C1110(void){ func_001C2114(D_001CDCD8, 0, 0); D_001CDE5C = 6; func_001C7CA8(); D_001CDE5C = 1; return 8; }

/* ---------------- func_001C1160 (src/pilot/frontend/func_001C1160.c) ---------------- */
void func_001C1160(void)
{
    D_000967F4 = 7;
    D_00096834 = 0;
    if (D_000967D0 >= 4)
        D_000967D0 = 0;
    D_00096808 = D_000967D0 + 1;
    func_00024754();
    func_001C2644(D_001CDDDC, 8, D_001CDE60);
    D_000967D0++;
}

/* ---------------- func_001C11E4 (src/pilot/frontend/func_001C11E4.c) ---------------- */
void func_001C11E4(void)
{
    DB *t;
    DrawSync(0);
    VSync(0);
    PutDispEnv(&((DB_ENV *)current)->disp);
    PutDrawEnv(&((DB_ENV *)current)->draw);
    t = current;
    D_00097524->primptr = D_00097524->primtab;
    current = D_00097524;
    D_00097524 = t;
    func_00074684(t->ot + 16);
    func_0007457C(current->ot, 0x10);
}

/* ---------------- func_001C1270 (src/pilot/frontend/func_001C1270.c) ---------------- */
/* builds the frontend menu sprites: 11 POLY_FT4 icons, 7 overlays and one extra (D_001CF888 = 3 prims per entry) */
void func_001C1270(void)
{
    TEX tex;
    POLY_FT4 *p;
    POLY_FT4 *q;
    int i;

    for (i = 0; i < 7; i++) {
        func_001C334C(D_001CDCD8[i + 1].name, &tex);
        ((P_TAG *)&D_001CF888[i].m[0])->len = 9;
        D_001CF888[i].m[0].code = 0x2C;
        D_001CF888[i].m[0].r0 = 128; D_001CF888[i].m[0].g0 = 128; D_001CF888[i].m[0].b0 = 128;
        D_001CF888[i].m[0].code |= 2;
        D_001CF888[i].m[0].code &= ~1;
        D_001CF888[i].m[0].x0 = D_001CDCD8[i + 1].x;
        D_001CF888[i].m[0].y0 = D_001CDCD8[i + 1].y;
        D_001CF888[i].m[0].x1 = D_001CDCD8[i + 1].x + (tex.u1 - tex.u0 + 1);
        D_001CF888[i].m[0].y1 = D_001CDCD8[i + 1].y;
        D_001CF888[i].m[0].x2 = D_001CDCD8[i + 1].x;
        D_001CF888[i].m[0].y2 = D_001CDCD8[i + 1].y + (tex.v2 - tex.v0 + 1);
        D_001CF888[i].m[0].x3 = D_001CDCD8[i + 1].x + (tex.u1 - tex.u0 + 1);
        D_001CF888[i].m[0].y3 = D_001CDCD8[i + 1].y + (tex.v2 - tex.v0 + 1);
        D_001CF888[i].m[0].tpage = tex.tpage;
        D_001CF888[i].m[0].clut = func_0007A7F4(960, tex.clut + 256);
        D_001CF888[i].m[0].u0 = tex.u0; D_001CF888[i].m[0].v0 = tex.v0; D_001CF888[i].m[0].u1 = tex.u1; D_001CF888[i].m[0].v1 = tex.v1;
        D_001CF888[i].m[0].u2 = tex.u2; D_001CF888[i].m[0].v2 = tex.v2; D_001CF888[i].m[0].u3 = tex.u3; D_001CF888[i].m[0].v3 = tex.v3;
    }
    for (i = 7; i < 11; i++) {
        func_001C334C(D_001CDCD8[i + 1].name, &tex);
        ((P_TAG *)&D_001CF888[i].m[0])->len = 9;
        D_001CF888[i].m[0].code = 0x2C;
        D_001CF888[i].m[0].r0 = 128; D_001CF888[i].m[0].g0 = 128; D_001CF888[i].m[0].b0 = 128;
        D_001CF888[i].m[0].code &= ~2;
        D_001CF888[i].m[0].code |= 1;
        D_001CF888[i].m[0].x0 = D_001CDCD8[i + 1].x;
        D_001CF888[i].m[0].y0 = D_001CDCD8[i + 1].y;
        D_001CF888[i].m[0].x1 = D_001CDCD8[i + 1].x + (tex.u1 - tex.u0 + 1);
        D_001CF888[i].m[0].y1 = D_001CDCD8[i + 1].y;
        D_001CF888[i].m[0].x2 = D_001CDCD8[i + 1].x;
        D_001CF888[i].m[0].y2 = D_001CDCD8[i + 1].y + (tex.v2 - tex.v0 + 1);
        D_001CF888[i].m[0].x3 = D_001CDCD8[i + 1].x + (tex.u1 - tex.u0 + 1);
        D_001CF888[i].m[0].y3 = D_001CDCD8[i + 1].y + (tex.v2 - tex.v0 + 1);
        D_001CF888[i].m[0].tpage = tex.tpage;
        D_001CF888[i].m[0].clut = func_0007A7F4(960, tex.clut + 256);
        D_001CF888[i].m[0].u0 = tex.u0; D_001CF888[i].m[0].v0 = tex.v0; D_001CF888[i].m[0].u1 = tex.u1; D_001CF888[i].m[0].v1 = tex.v1;
        D_001CF888[i].m[0].u2 = tex.u2; D_001CF888[i].m[0].v2 = tex.v2; D_001CF888[i].m[0].u3 = tex.u3; D_001CF888[i].m[0].v3 = tex.v3;
    }
    for (i = 0; i < 7; i++) {
        func_001C334C(D_001CDE6C[i], &tex);
        p = &D_001CF888[i].m[1];
        ((P_TAG *)p)->len = 9;
        p->code = 0x2C;
        p->r0 = 128; p->g0 = 128; p->b0 = 128;
        p->code |= 2;
        p->code &= ~1;
        p->x0 = D_001CDCD8[i + 1].x;
        p->y0 = D_001CDCD8[i + 1].y;
        p->x1 = D_001CDCD8[i + 1].x + (tex.u1 - tex.u0 + 1);
        p->y1 = D_001CDCD8[i + 1].y;
        p->x2 = D_001CDCD8[i + 1].x;
        p->y2 = D_001CDCD8[i + 1].y + (tex.v2 - tex.v0 + 1);
        p->x3 = D_001CDCD8[i + 1].x + (tex.u1 - tex.u0 + 1);
        p->y3 = D_001CDCD8[i + 1].y + (tex.v2 - tex.v0 + 1);
        D_001CF888[i].m[1].tpage = tex.tpage;
        p->clut = func_0007A7F4(960, tex.clut + 256);
        p->u0 = tex.u0; p->v0 = tex.v0; p->u1 = tex.u1; p->v1 = tex.v1;
        p->u2 = tex.u2; p->v2 = tex.v2; p->u3 = tex.u3; p->v3 = tex.v3;
    }
    func_001C334C("MCARICO3", &tex);
    q = &D_001CF888[6].m[2];
    ((P_TAG *)q)->len = 9;
    q->code = 0x2C;
    q->r0 = 128; q->g0 = 128; q->b0 = 128;
    q->code |= 2;
    q->code &= ~1;
    q->x0 = D_001CDCD8[7].x;
    q->y0 = D_001CDCD8[7].y;
    q->x1 = D_001CDCD8[7].x + (tex.u1 - tex.u0 + 1);
    q->y1 = D_001CDCD8[7].y;
    q->x2 = D_001CDCD8[7].x;
    q->y2 = D_001CDCD8[7].y + (tex.v2 - tex.v0 + 1);
    q->x3 = D_001CDCD8[7].x + (tex.u1 - tex.u0 + 1);
    q->y3 = D_001CDCD8[7].y + (tex.v2 - tex.v0 + 1);
    q->tpage = tex.tpage;
    q->clut = func_0007A7F4(960, tex.clut + 256);
    q->u0 = tex.u0; q->v0 = tex.v0; q->u1 = tex.u1; q->v1 = tex.v1;
    q->u2 = tex.u2; q->v2 = tex.v2; q->u3 = tex.u3; q->v3 = tex.v3;
}

/* ---------------- func_001C18AC (src/pilot/frontend/func_001C18AC.c) ---------------- */
void func_001C18AC(void)
{
    char col[4];
    int i;
    memset(col, 0, 4);
    func_001C19C4();
    for (i = 0; i < 7; i++) {
        POLY_FT4 *p = &D_001CF888[i].m[D_001CDCD8[i + 1].h8];
        ((P_TAG *)p)->addr = ((P_TAG *)current->ot[D_001CDCD8[i + 1].h12])->addr;
        ((P_TAG *)current->ot[D_001CDCD8[i + 1].h12])->addr = (unsigned)p;
    }
    func_001C2014(0x6F, 0x8C, col, 0x60, 0x8E, 9);
}

/* ---------------- func_001C19C4 (src/pilot/frontend/func_001C19C4.c) ---------------- */
void func_001C19C4(void)
{
    char col[4];
    int i;
    memset(col, 0, 4);
    for (i = 7; i < 11; i++) {
        POLY_FT4 *p = &D_001CF888[i].m[D_001CDCD8[i + 1].h8];
        ((P_TAG *)p)->addr = ((P_TAG *)current->ot[D_001CDCD8[i + 1].h12])->addr;
        ((P_TAG *)current->ot[D_001CDCD8[i + 1].h12])->addr = (unsigned)p;
    }
    func_001C2014(0, 0, col, 0x140, 0x30, 10);
    func_001C2014(0, 0x1B0, col, 0x140, 0x50, 10);
}

