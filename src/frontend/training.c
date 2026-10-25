/* Boundary evidence (pilot/TU1.md, FRONTEND section):
 * "Training:" menu (Desert, Car park; DESERT/GARAGE) func_001C5A30 + table D_001CE498. END: text pad at 0x1C5F44
 * and .rodata pad 0x1C0554.
 */
/* training.c: 9 functions func_001C5A30..func_001C5EE0 (0x1C5A30-0x1C5F48)
 * Step 3: types from include/frontend.h + structs.h (MENU_TITLE D_001CE430 (D_001CE434 = its .text), MENU_ITEM,
 * MENU_TEXT, DB *current, POLY_FT4/TEX). No views.
 */

/* ---- types ---- */
#include "frontend.h"
typedef struct { short y, pad; } YPOS;

/* ---- externs ---- */
extern int D_001CE4B8;
extern MENU_ITEM D_001CE498[];
extern MENU_TITLE D_001CE430;
extern MENU_TEXT D_001CE458[];
extern void func_001C2644();
extern void func_001C2114();
extern void func_000514E0();
extern int func_001C26E4();
extern void func_001C19C4();
extern void func_001C1BD4();
extern void func_001C11E4();
extern char *D_001CE434;
extern char D_001CE46C;
extern POLY_FT4 *D_001D2E20;
extern char D_001C054C[];
extern POLY_FT4 D_001D2DD0[];
extern int D_000967F4;
extern int D_00096834;
extern int D_00096858;
extern int D_001CDE5C;
extern void SetDispMask();
extern void func_00024754();
extern char D_001CE47C;
extern char D_001C0544[];
extern POLY_FT4 D_001D2DF8[]; /* = D_001D2DD0[1] */
extern int D_001CE4BC[];
extern YPOS D_001CE4C4[];
extern void func_001C334C();
extern int func_0007A7F4();
extern DB *current;

/* ---- functions defined in this file ---- */
int func_001C5A30();
void func_001C5AEC();
void func_001C5B1C();
int func_001C5B28();
void func_001C5C10();
void func_001C5C40();
int func_001C5C4C();
void func_001C5D28();
void func_001C5EE0();

/* ---------------- func_001C5A30 (src/pilot/frontend/func_001C5A30.c) ---------------- */
int func_001C5A30(void)
{
    int r;
    D_001CE4B8 = 0;
    func_001C2644(D_001CE498, 2, 0);
    func_001C5D28();
    func_001C2114(&D_001CE430, D_001CE458, 1);
    for (;;) {
        func_000514E0();
        r = func_001C26E4(D_001CE498, &D_001CE4B8);
        if (r != 0) break;
        func_001C19C4();
        func_001C5EE0();
        func_001C1BD4(D_001CE458);
        func_001C11E4();
    }
    if (r == -1) return 0;
    return r;
}

/* ---------------- func_001C5AEC (src/pilot/frontend/func_001C5AEC.c) ---------------- */
void func_001C5AEC(void){ D_001CE434 = D_001C054C; D_001CE46C = 1; D_001D2E20 = D_001D2DD0; }

/* ---------------- func_001C5B1C (src/pilot/frontend/func_001C5B1C.c) ---------------- */
void func_001C5B1C(void) { D_001CE46C = 0; }

/* ---------------- func_001C5B28 (src/pilot/frontend/func_001C5B28.c) ---------------- */
/* func_001C5B28: menu screen enter. D_001CE4B8 = 0 BEFORE D_001CDE5C = 3: shortens the %hi(D_001CE4B8) pseudo so it
   beats the shared constant 1 in a local-alloc priority tie (.2308 each; ties go to the earlier-born qty). */
int func_001C5B28(void)
{
    MENU_TITLE *t = &D_001CE430;
    D_00096858 = 1;
    D_00096834 = 2;
    D_000967F4 = 3;
    func_001C2114(t, D_001CE458, 0);
    SetDispMask(0);
    func_00024754();
    D_001CE4B8 = 1;
    func_001C5D28();
    func_001C2644(D_001CE498, 2, 0);
    D_001CE4B8 = 0;
    D_001CDE5C = 3;
    t->text = D_001C054C;
    func_001C2114(t, D_001CE458, 1);
    D_00096858 = 0;
    return 0;
}

/* ---------------- func_001C5C10 (src/pilot/frontend/func_001C5C10.c) ---------------- */
void func_001C5C10(void){ D_001CE434 = D_001C0544; D_001CE47C = 1; D_001D2E20 = D_001D2DF8; }

/* ---------------- func_001C5C40 (src/pilot/frontend/func_001C5C40.c) ---------------- */
void func_001C5C40(void) { D_001CE47C = 0; }

/* ---------------- func_001C5C4C (src/pilot/frontend/func_001C5C4C.c) ---------------- */
/* FAKEMATCH: local `one` (=1) only steers register allocation */
int func_001C5C4C(void)
{
    int one;
    MENU_TITLE *t = &D_001CE430;
    D_00096834 = 6;
    D_000967F4 = 3;
    func_001C2114(t, D_001CE458, 0);
    SetDispMask(0);
    one = 1;
    func_00024754();
    D_001CE4B8 = one;
    func_001C5D28();
    func_001C2644(D_001CE498, 2, one);
    t->text = D_001C0544;
    D_001CE4B8 = one;
    D_001CDE5C = 3;
    func_001C2114(t, D_001CE458, one);
    D_00096858 = 0;
    return 0;
}

/* ---------------- func_001C5D28 (src/pilot/frontend/func_001C5D28.c) ---------------- */
/* func_001C5D28: twin of func_001C4810 (2 sprites). setSemiTrans(p,0); setShadeTex(p,0) as two separate &= after rgb. */
void func_001C5D28(void)
{
    TEX tex;
    POLY_FT4 *p;
    int i;

    for (i = 0; i < 2; i++) {
        func_001C334C(D_001CE4BC[i], &tex);
        p = &D_001D2DD0[i];
        ((P_TAG *)p)->len = 9;
        p->code = 0x2C;
        p->r0 = 128;
        p->g0 = 128;
        p->b0 = 128;
        p->code &= ~2;
        p->code &= ~1;
        p->x0 = 160;
        p->y0 = D_001CE4C4[i].y;
        p->x1 = tex.u1 - tex.u0 + 161;
        p->y1 = D_001CE4C4[i].y;
        p->x2 = 160;
        p->y2 = D_001CE4C4[i].y + (tex.v2 - tex.v0 + 1);
        p->x3 = tex.u1 - tex.u0 + 161;
        p->y3 = D_001CE4C4[i].y + (tex.v2 - tex.v0 + 1);
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
    D_001D2E20 = D_001D2DD0;
}

/* ---------------- func_001C5EE0 (src/pilot/frontend/func_001C5EE0.c) ---------------- */
/* addPrim(current->ot[2], D_001D2E20): the P_TAG bitfield form gives the target's mask registers (ov1-opus). */
#define addPrim(ot, p) (((P_TAG *)(p))->addr = ((P_TAG *)(ot))->addr, ((P_TAG *)(ot))->addr = (unsigned)(p))
void func_001C5EE0(void)
{
    addPrim(current->ot[2], D_001D2E20);
}
#undef addPrim

