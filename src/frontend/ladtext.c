/* Boundary evidence (pilot/TU1.md, FRONTEND section):
 * text/sprite helpers func_001CA270, func_001CA298 (SetTextColour), func_001CA408 (GetTPage). START: text pad at
 * 0x1CA26C. END: text pad at 0x1CA5FC.
 */
/* ladtext.c: 3 functions func_001CA270..func_001CA408 (0x1CA270-0x1CA600)
 * Step 3: types from include/frontend.h + structs.h (SPRT, POLY_FT3, TEXINFO, DB *current). No views.
 */

/* ---- types ---- */
#include "frontend.h"

/* ---- externs ---- */
extern unsigned char D_00097680[];
extern int D_00097690;
extern int D_0009768C;
extern unsigned char D_00097698[];
extern void SetTextColour(unsigned char, unsigned char, unsigned char);
extern DB *current;
extern unsigned short GetTPage();

/* ---- functions defined in this file ---- */
void func_001CA270();
int func_001CA298();
void func_001CA408(int x, int y, TEXINFO *tex, unsigned char r, unsigned char g, unsigned char b, int unused, unsigned char mode);

/* ---------------- func_001CA270 (src/pilot/frontend/func_001CA270.c) ---------------- */
void func_001CA270(int a, int b, int c, int d)
{
    D_00097680[0] = a;
    D_00097680[1] = b;
    D_00097680[2] = c;
    D_00097690 = d;
    D_0009768C = 0;
}

/* ---------------- func_001CA298 (src/pilot/frontend/func_001CA298.c) ---------------- */
int func_001CA298(void)
{
    if (D_0009768C == D_00097690) {
        SetTextColour(0, 0, 0);
        return 0;
    }
    SetTextColour(D_00097698[0] + (D_00097680[0] - D_00097698[0]) * D_0009768C / D_00097690,
                  D_00097698[1] + (D_00097680[1] - D_00097698[1]) * D_0009768C / D_00097690,
                  D_00097698[2] + (D_00097680[2] - D_00097698[2]) * D_0009768C / D_00097690);
    D_0009768C++;
    return 1;
}

/* ---------------- func_001CA408 (src/pilot/frontend/func_001CA408.c) ---------------- */
/* FAKEMATCH: unused pad[32] only sizes the frame (0x38); probably an unused local aggregate in the original. */
#define addPrim(ot, p) (((P_TAG *)(p))->addr = ((P_TAG *)(ot))->addr, ((P_TAG *)(ot))->addr = (unsigned)(p))
void func_001CA408(int x, int y, TEXINFO *tex, unsigned char r, unsigned char g, unsigned char b, int unused, unsigned char mode)
{
    SPRT *sp;
    POLY_FT3 *tp;
    char pad[32]; /* FAKEMATCH: frame size */

    sp = (SPRT *)current->primptr;
    ((P_TAG *)sp)->len = 4;
    sp->code = 0x64;
    sp->r0 = r;
    sp->g0 = g;
    sp->b0 = b;
    sp->x0 = x;
    sp->y0 = y;
    sp->code |= 2;
    sp->code &= ~1;
    sp->u0 = tex->u0;
    sp->v0 = tex->v0;
    sp->w = tex->u1 - tex->u0 + 1;
    sp->h = tex->v2 - tex->v0 + 1;
    sp->clut = tex->clut;
    addPrim(current->ot[1], sp);
    current->primptr += sizeof(SPRT);

    tp = (POLY_FT3 *)current->primptr;
    ((P_TAG *)tp)->len = 7;
    tp->code = 0x24;
    tp->x0 = -1;
    tp->y0 = -1;
    tp->x1 = -1;
    tp->y1 = -1;
    tp->x2 = -1;
    tp->y2 = -1;
    if (mode == 3) {
        tp->code |= 2;
        tp->tpage = GetTPage(0, 0, 0x240, 0);
    } else {
        tp->code |= 2;
        tp->tpage = tex->tpage;
    }
    addPrim(current->ot, tp);
    current->primptr += sizeof(POLY_FT3);
}
#undef addPrim
