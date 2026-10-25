/* Boundary evidence (pilot/TU1.md, FRONTEND section):
 * city menu func_001C41D8 + table D_001CE048 (New York/Los Angeles/San Francisco/Miami), city draw, sub-menu
 * D_001CE0FC (func_001C4AE4 called by the city handlers). START: .rodata pad 0x1C031C, its .data's "" at 8-aligned
 * 0x1C0320. END: no call/data edge crosses 0x1C4BC8 (span 25); the city names and "" are duplicated by
 * drivinggames.c's D_001CE204 strings -> separate object.
 */
/* cityselect.c: 19 functions func_001C41D8..func_001C4AE4 (0x1C41D8-0x1C4BC8)
 * Step 3: types from include/frontend.h + structs.h (MENU_ITEM D_001CE048/D_001CE0FC, MENU_TEXT D_001CDFE8/D_001CE0CC,
 * DB *current, POLY_FT4/TEX). MENU_TITLE D_001CDFC0. No views;
 * SetDispMask is `extern int` for the whole file (func_001C4A7C's FAKEMATCH note; same bytes elsewhere).
 */

/* ---- types ---- */
#include "frontend.h"
typedef struct { short y, pad; } YPOS;

/* ---- externs ---- */
extern MENU_ITEM D_001CE048[];
extern MENU_TEXT D_001CDFE8[];
extern MENU_TITLE D_001CDFC0;
extern char *D_001CE08C[];
extern char *D_001CE09C[];
extern int D_001CDEDC;
extern int D_001CE088;
extern int D_001CDE64;
extern int D_000967E0;
extern void func_001C2644();
extern void func_001C2114();
extern void func_000514E0();
extern void func_001C19C4();
extern void func_001C1BD4();
extern void func_001C11E4();
extern int func_001C26E4();
extern char D_001C03AC[];
extern POLY_FT4 D_001D2D10[];
extern char D_001CDFFC;
extern int D_00096834;
extern POLY_FT4 *D_001D2DB0;
extern int D_000967F4;
extern int D_00096808;
extern char D_001C0388[];
extern POLY_FT4 D_001D2D38[]; /* = D_001D2D10[1..3] */
extern char D_001CE00C;
extern char D_001C037C[];
extern POLY_FT4 D_001D2D60[]; /* = D_001D2D10[1..3] */
extern char D_001CE01C;
extern void DrawSync();
extern void VSync();
extern int SetDispMask();
extern char D_001C0370[];
extern POLY_FT4 D_001D2D88[]; /* = D_001D2D10[1..3] */
extern char D_001CE02C;
extern int D_001CE0AC[];
extern YPOS D_001CE0BC[];
extern void func_001C334C();
extern int func_0007A7F4();
extern DB *current;
extern MENU_TEXT D_001CE0CC[];
extern int D_001D2DC0;
extern int D_001D2DB8;
extern int D_001CDE5C;
extern MENU_ITEM D_001CE0FC[];

/* ---- functions defined in this file ---- */
int func_001C41D8();
void func_001C43EC();
void func_001C4440();
int func_001C444C();
void func_001C44FC();
void func_001C4554();
int func_001C4560();
void func_001C4610();
void func_001C4668();
int func_001C4674();
void func_001C46FC();
void func_001C4754();
int func_001C4760();
void func_001C4810();
void func_001C49C8();
void func_001C4A2C();
void func_001C4A50();
int func_001C4A7C();
int func_001C4AE4();

/* ---------------- func_001C41D8 (src/pilot/frontend/func_001C41D8.c) ---------------- */
int func_001C41D8(void)
{
    int i;
    int r;

    D_001CDEDC = 0;
    D_001CE088 = 0;
    func_001C2644(D_001CE048, 4, 0);
    D_001CDFE8[0].text = D_001CE08C[D_001CDE64];
    for (i = 0; i < 4; i++)
        D_001CDFE8[i + 1].text = D_001CE09C[i];
    for (i = D_000967E0; i < 4; i++) {
        D_001CDFE8[i + 1].col.r = 0x30;
        D_001CDFE8[i + 1].col.g = 0x30;
        D_001CDFE8[i + 1].col.b = 0x30;
    }
    if (D_000967E0 == 1) {
        D_001CE048[0].down = 0;
        D_001CE048[0].up = 0;
    } else if (D_000967E0 == 2) {
        D_001CE048[0].up = 1;
        D_001CE048[0].down = 1;
        D_001CE048[1].up = 0;
        D_001CE048[1].down = 0;
    } else if (D_000967E0 == 3) {
        D_001CE048[0].up = 2;
        D_001CE048[0].down = 1;
        D_001CE048[1].up = 0;
        D_001CE048[1].down = 2;
        D_001CE048[2].up = 1;
        D_001CE048[2].down = 0;
    } else {
        D_001CE048[0].up = 3;
        D_001CE048[0].down = 1;
        D_001CE048[1].up = 0;
        D_001CE048[1].down = 2;
        D_001CE048[2].up = 1;
        D_001CE048[2].down = 3;
        D_001CE048[3].up = 2;
        D_001CE048[3].down = 0;
    }
    func_001C4810();
    func_001C2114(&D_001CDFC0, D_001CDFE8, 1);
    for (;;) {
        func_000514E0();
        r = func_001C26E4(D_001CE048, &D_001CE088);
        if (r != 0)
            break;
        func_001C19C4();
        func_001C49C8();
        func_001C1BD4(D_001CDFE8);
        func_001C11E4();
    }
    if (r == -1)
        return 0;
    return r;
}

/* ---------------- func_001C43EC (src/pilot/frontend/func_001C43EC.c) ---------------- */
void func_001C43EC(void){ D_001CDFC0.text = D_001C03AC; D_001CDFC0.x = 0x147; D_001CDFC0.y = 0xB6; D_001CDFFC = 1; D_001CDEDC = 0; D_00096834 = 0; D_001D2DB0 = D_001D2D10; }

/* ---------------- func_001C4440 (src/pilot/frontend/func_001C4440.c) ---------------- */
void func_001C4440(void) { D_001CDFFC = 0; }

/* ---------------- func_001C444C (src/pilot/frontend/func_001C444C.c) ---------------- */
int func_001C444C(void)
{
    int r;
    if (D_000967F4 == 0x13) {
        func_001C2114(&D_001CDFC0, D_001CDFE8, 0);
        return 1;
    }
    func_001C2114(&D_001CDFC0, D_001CDFE8, 0);
    D_00096808 = 0x78;
    r = func_001C4AE4();
    if (r == 0) {
        func_001C2114(&D_001CDFC0, D_001CDFE8, 1);
    }
    return r;
}

/* ---------------- func_001C44FC (src/pilot/frontend/func_001C44FC.c) ---------------- */
void func_001C44FC(void){ D_001CDFC0.text = D_001C0388; D_001CDFC0.x = 0x148; D_001CDFC0.y = 0x9A; D_001CE00C = 1; D_001CDEDC = 0; D_00096834 = 1; D_001D2DB0 = D_001D2D38; }

/* ---------------- func_001C4554 (src/pilot/frontend/func_001C4554.c) ---------------- */
void func_001C4554(void) { D_001CE00C = 0; }

/* ---------------- func_001C4560 (src/pilot/frontend/func_001C4560.c) ---------------- */
int func_001C4560(void)
{
    int r;
    if (D_000967F4 == 0x13) {
        func_001C2114(&D_001CDFC0, D_001CDFE8, 0);
        return 1;
    }
    func_001C2114(&D_001CDFC0, D_001CDFE8, 0);
    D_00096808 = 0x7A;
    r = func_001C4AE4();
    if (r == 0) {
        func_001C2114(&D_001CDFC0, D_001CDFE8, 1);
    }
    return r;
}

/* ---------------- func_001C4610 (src/pilot/frontend/func_001C4610.c) ---------------- */
void func_001C4610(void){ D_001CDFC0.text = D_001C037C; D_001CDFC0.x = 0x148; D_001CDFC0.y = 0xC8; D_001CE01C = 1; D_001CDEDC = 0; D_00096834 = 3; D_001D2DB0 = D_001D2D60; }

/* ---------------- func_001C4668 (src/pilot/frontend/func_001C4668.c) ---------------- */
void func_001C4668(void) { D_001CE01C = 0; }

/* ---------------- func_001C4674 (src/pilot/frontend/func_001C4674.c) ---------------- */
int func_001C4674(void)
{
    if (D_000967F4 != 0x13) {
        func_001C11E4();
        DrawSync(0);
        VSync(0);
        func_001C2114(&D_001CDFC0, D_001CDFE8, 0);
        SetDispMask(0);
        D_00096808 = 0x7C;
    } else {
        func_001C2114(&D_001CDFC0, D_001CDFE8, 0);
    }
    return 1;
}

/* ---------------- func_001C46FC (src/pilot/frontend/func_001C46FC.c) ---------------- */
void func_001C46FC(void){ D_001CDFC0.text = D_001C0370; D_001CDFC0.x = 0x147; D_001CDFC0.y = 0x8A; D_001CE02C = 1; D_001CDEDC = 0; D_00096834 = 5; D_001D2DB0 = D_001D2D88; }

/* ---------------- func_001C4754 (src/pilot/frontend/func_001C4754.c) ---------------- */
void func_001C4754(void) { D_001CE02C = 0; }

/* ---------------- func_001C4760 (src/pilot/frontend/func_001C4760.c) ---------------- */
int func_001C4760(void)
{
    int r;
    if (D_000967F4 == 0x13) {
        func_001C2114(&D_001CDFC0, D_001CDFE8, 0);
        return 1;
    }
    func_001C2114(&D_001CDFC0, D_001CDFE8, 0);
    D_00096808 = 0x7D;
    r = func_001C4AE4();
    if (r == 0) {
        func_001C2114(&D_001CDFC0, D_001CDFE8, 1);
    }
    return r;
}

/* ---------------- func_001C4810 (src/pilot/frontend/func_001C4810.c) ---------------- */
/* func_001C4810: builds 4 POLY_FT4 menu sprites. code = 0x2C then setSemiTrans(p,0); setShadeTex(p,0) as two
   separate &= after the rgb stores (a single &= ~3 is cse-folded). */
void func_001C4810(void)
{
    TEX tex;
    POLY_FT4 *p;
    int i;

    for (i = 0; i < 4; i++) {
        func_001C334C(D_001CE0AC[i], &tex);
        p = &D_001D2D10[i];
        ((P_TAG *)p)->len = 9;
        p->code = 0x2C;
        p->r0 = 128;
        p->g0 = 128;
        p->b0 = 128;
        p->code &= ~2;
        p->code &= ~1;
        p->x0 = 160;
        p->y0 = D_001CE0BC[i].y;
        p->x1 = tex.u1 - tex.u0 + 161;
        p->y1 = D_001CE0BC[i].y;
        p->x2 = 160;
        p->y2 = D_001CE0BC[i].y + (tex.v2 - tex.v0 + 1);
        p->x3 = tex.u1 - tex.u0 + 161;
        p->y3 = D_001CE0BC[i].y + (tex.v2 - tex.v0 + 1);
        p->tpage = tex.tpage;
        p->clut = func_0007A7F4(960, tex.clut + 256);
        p->u0 = tex.u0;
        p->v0 = tex.v0;
        p->u1 = tex.u1;
        p->v1 = tex.v1;
        p->u2 = tex.u2;
        p->v2 = tex.v2;
        p->u3 = tex.u3;
        p->v3 = tex.v3;
    }
    D_001D2DB0 = D_001D2D10;
}

/* ---------------- func_001C49C8 (src/pilot/frontend/func_001C49C8.c) ---------------- */
/* addPrim(current->ot[2], D_001D2DB0): the P_TAG bitfield form gives the target's mask registers (ov1-opus). */
#define addPrim(ot, p) (((P_TAG *)(p))->addr = ((P_TAG *)(ot))->addr, ((P_TAG *)(ot))->addr = (unsigned)(p))
void func_001C49C8(void)
{
    addPrim(current->ot[2], D_001D2DB0);
}
#undef addPrim

/* ---------------- func_001C4A2C (src/pilot/frontend/func_001C4A2C.c) ---------------- */
void func_001C4A2C(void)
{
    D_001CE0CC[D_001D2DC0].font = 1;
}

/* ---------------- func_001C4A50 (src/pilot/frontend/func_001C4A50.c) ---------------- */
void func_001C4A50(void)
{
    int i = D_001D2DB8;
    if (i != -1)
        D_001CE0CC[i].font = 0;
}

/* ---------------- func_001C4A7C (src/pilot/frontend/func_001C4A7C.c) ---------------- */
/* FAKEMATCH: SetDispMask is declared `extern int` here (PsyQ libgpu declares it void). It matches as if the original called it without a prototype (implicit int); unconfirmed. */
/* func_001C4A7C: menu exit. Plain C once SetDispMask is declared returning int (v0 live after the call - probably an
   implicit-int K&R call in the original; PsyQ's SetDispMask is void). */
int func_001C4A7C(void)
{
    func_001C11E4();
    DrawSync(0);
    VSync(0);
    func_001C2114(&D_001CDFC0, D_001CDFE8, 0);
    SetDispMask(0);
    D_00096808 += D_001D2DC0;
    return D_001D2DC0 + 1;
}

/* ---------------- func_001C4AE4 (src/pilot/frontend/func_001C4AE4.c) ---------------- */
int func_001C4AE4(void)
{
    int r;
    D_001D2DC0 = 0;
    func_001C2644(D_001CE0FC, 2, 0);
    D_001CDE5C = 6;
    func_001C2114(0, D_001CE0CC, 1);
    D_001D2DB8 = -1;
    for (;;) {
        func_000514E0();
        D_001D2DB8 = D_001D2DC0;
        r = func_001C26E4(D_001CE0FC, &D_001D2DC0);
        if (r != 0) break;
        func_001C19C4();
        func_001C1BD4(D_001CE0CC);
        func_001C11E4();
    }
    D_001CDE5C = 2;
    if (r == -1) return 0;
    return r;
}

