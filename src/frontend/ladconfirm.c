/* Boundary evidence (pilot/TU1.md, FRONTEND section):
 * "Are You Sure" YES/NO (D_001CF208 strings start at 0x1C0A28, right after the pad), handler tables D_001CF248 and
 * D_001CF198; .data 0x1CF098-0x1CF28C. START: text pad. END: text pad at 0x1CB18C; ladview's .data starts at
 * 0x1CF290.
 */
/* ladconfirm.c: 20 functions func_001CA600..func_001CB16C (0x1CA600-0x1CB190)
 * Step 3: types from include/frontend.h (MENU_TEXT D_001CF198/D_001CF208). D_00097538 is short for the whole file
 * (func_001CACE0 needs the lh; func_001CA834 stores it into an unsigned short, same bytes either way; a
 * `*(short *)&` cast instead changes func_001CACE0's allocation). D_001CF0F0 is an int that func_001CAE64 indexes as the head of an array
 * ((&D_001CF0F0)[k]). E4/S are file-local. No views.
 */

/* ---- types ---- */
#include "frontend.h"
typedef struct { int a, b, c, d; } E4;
typedef struct { unsigned short f; unsigned short g; int pad; } S;

/* ---- externs ---- */
extern int D_00096C90;
extern int D_001CEF88;
extern int D_001CF098;
extern int D_001CF09C;
extern int D_001CF0A0;
extern int D_001CF0A4;
extern int D_001CF0D8;
extern int D_001CF0DC;
extern char D_001CF0B8[];
extern void func_001CA270();
extern void func_000514E0();
extern void func_0004E454();
extern void func_001CB380();
extern void func_001CA298();
extern void func_001CB95C();
extern void DrawSync();
extern void func_000513D4();
extern void SetTextColour();
extern void func_001CB4A0();
extern int func_001C9A18();
extern int D_001CF288;
extern E4 D_001D32B0[];
extern int D_001CF28C;
extern short D_00097538;
extern int D_001CF0A8;
extern int D_001CF0AC;
extern int D_001CF0E0;
extern int D_001CF0E4;
extern int D_001CF0E8;
extern int D_001CF0EC;
extern int D_001CF0F0;
extern char D_001D3230[];
extern char D_001D3240[];
extern char D_001D3250[];
extern char D_001D3260[];
extern void func_001CA408();
extern int D_000BFA54;
extern int D_000BFA58;
extern int D_000BFA5C;
extern int D_000BFA60;
extern int D_001CF0C0;
extern void func_0006F5CC();
extern int func_0006F6DC();
extern void func_0006F728();
extern S *D_001CEF58;
extern int D_001CF26C;
extern int D_001CF27C;
extern int D_001CF278;
extern int D_001CDECC;
extern MENU_TEXT D_001CF208[];
extern char D_001CF248[];
extern int func_001CB790();
extern MENU_TEXT D_001CF198[];
extern void func_001CD5A8(void);
extern void func_001CCD58(void);

/* ---- functions defined in this file ---- */
int func_001CA600();
void func_001CA7EC();
int func_001CA834();
void func_001CAA0C();
void func_001CAC70();
void func_001CACE0();
void func_001CAE64();
int func_001CAF70();
void func_001CB054();
void func_001CB070();
int func_001CB08C();
void func_001CB0A8();
void func_001CB0C4();
void func_001CB0E0();
void func_001CB0FC();
void func_001CB104();
int func_001CB124();
void func_001CB144();
void func_001CB14C();
int func_001CB16C();

/* ---------------- func_001CA600 (src/pilot/frontend/func_001CA600.c) ---------------- */
int func_001CA600(void)
{
    int r;
    int step;

    func_001CA270(0, 0, 0, 0x40);
    step = (D_00096C90 - 10000) / func_001C9A18(9);
    if (D_001CF098 == 0)
        func_001CA7EC();
    D_001CF09C = 1;
    D_001CF0A0 = 0;
    D_001CF0DC = 0;
    D_001CF0D8 = 1;
    for (;;) {
        func_000514E0();
        r = func_001CA834();
        if (r != 0)
            break;
        if (D_001CF0DC == 2) {
            D_00096C90 = -10000;
            func_0004E454(-10000);
            D_001CF0D8 = 0;
            if (D_001CF0A4 > 0)
                func_001CACE0(1);
        }
        func_001CB380();
        func_001CA298();
        if (D_001CF0D8) {
            D_00096C90 += step;
            func_0004E454(D_00096C90);
        }
        if (D_001CEF88 == 0) {
            func_001CB95C(D_001CF0B8);
            func_001CAA0C();
            D_001CF0DC++;
        }
        DrawSync(0);
        func_000513D4();
    }
    SetTextColour(0, 0, 0);
    func_001CA270(0, 0, 0, 0x40);
    if (r != -1)
        func_001CB4A0(0, 0x20);
    return r;
}

/* ---------------- func_001CA7EC (src/pilot/frontend/func_001CA7EC.c) ---------------- */
void func_001CA7EC(void){ D_001CF09C = 1; D_001CF0A0 = 0; D_001CF098 = 1; D_001CF0A4 = D_001CF288; func_001CAC70(); }

/* ---------------- func_001CA834 (src/pilot/frontend/func_001CA834.c) ---------------- */
int func_001CA834(void)
{
    unsigned short pad;

    if (D_001CEF88 == 0) {
        pad = D_00097538;
        if (pad & 0x10) {
            func_001CA270(0, 0, 0, 0x40);
            func_001C9A18(4);
            return -1;
        } else if (pad & 0x8000) {
            if (--D_001CF09C < 0)
                D_001CF09C = 3;
            func_001CAC70();
        } else if (pad & 0x2000) {
            if (++D_001CF09C >= 4)
                D_001CF09C = 0;
            func_001CAC70();
        } else if (pad & 0x40) {
            switch (D_001CF09C) {
            case 0:
                if (--D_001CF0A0 < 0)
                    D_001CF0A0 = D_001CF288 - 1;
                func_001CACE0(D_001D32B0[D_001CF0A0].a);
                break;
            case 1:
                func_001CACE0(D_001D32B0[D_001CF0A0].a);
                break;
            case 2:
                if (++D_001CF0A0 >= D_001CF288)
                    D_001CF0A0 = 0;
                func_001CACE0(D_001D32B0[D_001CF0A0].a);
                break;
            case 3:
                if (D_001D32B0[D_001CF0A0].b) {
                    D_001CF28C = D_001CF0A0;
                    return 1;
                }
                break;
            default:
                D_001CF09C = 0;
                break;
            }
        }
    }
    return 0;
}

/* ---------------- func_001CAA0C (src/pilot/frontend/func_001CAA0C.c) ---------------- */
void func_001CAA0C(void)
{
    D_001CF0A8 += D_001CF0AC;
    if (D_001CF0A8 > 255) {
        D_001CF0A8 = 255;
        D_001CF0AC = -D_001CF0AC;
    }
    if (D_001CF0A8 < 0) {
        D_001CF0A8 = 0;
        D_001CF0AC = -D_001CF0AC;
    }
    if (D_001CF09C == 0)
        func_001CA408(D_001CF0E0, D_001CF0F0, D_001D3240, (unsigned char)D_001CF0A8, (unsigned char)D_001CF0A8, (unsigned char)D_001CF0A8, 1, 3);
    else
        func_001CA408(D_001CF0E0, D_001CF0F0, D_001D3240, 255, 255, 255, 1, 3);
    if (D_001CF09C == 1)
        func_001CA408(D_001CF0E4, D_001CF0F0, D_001D3230, (unsigned char)D_001CF0A8, (unsigned char)D_001CF0A8, (unsigned char)D_001CF0A8, 0, 3);
    else
        func_001CA408(D_001CF0E4, D_001CF0F0, D_001D3230, 255, 255, 255, 0, 3);
    if (D_001CF09C == 2)
        func_001CA408(D_001CF0E8, D_001CF0F0, D_001D3260, (unsigned char)D_001CF0A8, (unsigned char)D_001CF0A8, (unsigned char)D_001CF0A8, 0, 3);
    else
        func_001CA408(D_001CF0E8, D_001CF0F0, D_001D3260, 255, 255, 255, 0, 3);
    if (D_001CF09C == 3)
        func_001CA408(D_001CF0EC, D_001CF0F0, D_001D3250, (unsigned char)D_001CF0A8, (unsigned char)D_001CF0A8, (unsigned char)D_001CF0A8, 0, 3);
    else
        func_001CA408(D_001CF0EC, D_001CF0F0, D_001D3250, 255, 255, 255, 0, 3);
}

/* ---------------- func_001CAC70 (src/pilot/frontend/func_001CAC70.c) ---------------- */
/* Switch with the default body FIRST in source (target code order: default, 1, 2, 3); case 0 shares it (ov1-opus). */
void func_001CAC70(void)
{
    switch (D_001CF09C) {
    case 0:
    default: D_001CF0C0 = D_000BFA54; break;
    case 1: D_001CF0C0 = D_000BFA58; break;
    case 2: D_001CF0C0 = D_000BFA5C; break;
    case 3: D_001CF0C0 = D_000BFA60; break;

    }
}

/* ---------------- func_001CACE0 (src/pilot/frontend/func_001CACE0.c) ---------------- */
void func_001CACE0(int mode)
{
    int saved = D_001CF09C;

    switch (mode) {
    case 8:
        func_0006F5CC(8, D_001D32B0[D_001CF0A0].c, D_001D32B0[D_001CF0A0].d);
        D_001CF0A4--;
        break;
    case 1:
        func_0006F5CC(1, D_001CF0A4 - 1, 0);
        break;
    case 4:
        func_0006F5CC(4, 0, D_001D32B0[D_001CF0A0].d);
        D_001CF0A4--;
        break;
    default:
        return;
    }
    D_001CF09C = -1;
    while (func_0006F6DC()) {
        func_000514E0();
        if (D_00097538)
            func_0006F728();
        func_001CB380();
        if (D_001CEF88 == 0) {
            func_001CB95C(D_001CF0B8);
            func_001CAA0C();
            D_001CF0DC++;
        }
        DrawSync(0);
        func_000513D4();
    }
    D_001CF09C = saved;
}

/* ---------------- func_001CAE64 (src/pilot/frontend/func_001CAE64.c) ---------------- */
/* func_001CAE64: index every access by i (D[i].f) - the giv form; flag 8 stores 2 (the old wip stored 4 for both). */
void func_001CAE64(void)
{
    int i;
    D_001CF288 = 0;
    for (i = 0; i < 4; i++) {
        if (D_001CEF58[i].f & 4) {
            D_001D32B0[i].a = 4;
            D_001D32B0[i].b = 0;
            D_001D32B0[i].c = 0;
            D_001D32B0[i].d = D_001CEF58[i].g;
            D_001CF288++;
        } else if (D_001CEF58[i].f & 8) {
            D_001D32B0[i].a = 2;
            D_001D32B0[i].b = 0;
            D_001D32B0[i].c = 0;
            D_001D32B0[i].d = D_001CEF58[i].g;
            D_001CF288++;
        } else if (D_001CEF58[i].f & 2) {
            D_001D32B0[i].a = 8;
            D_001D32B0[i].b = 1;
            D_001D32B0[i].c = (&D_001CF0F0)[D_001CEF58[i].g] / 8;
            D_001D32B0[i].d = (&D_001CF0F0)[D_001CEF58[i].g] % 8;
            D_001CF288++;
        }
    }
}

/* ---------------- func_001CAF70 (src/pilot/frontend/func_001CAF70.c) ---------------- */
int func_001CAF70(void)
{
    D_001CF26C = 1;
    D_001CF208[1].col.r = 255;
    D_001CF208[1].col.g = 255;
    D_001CF208[1].col.b = 255;
    D_001CF208[2].col.r = 255;
    D_001CF208[2].col.g = 0;
    D_001CF208[2].col.b = 0;
    D_001CF27C = 0;
    D_001CDECC = 0;
    do {
        func_000514E0();
        D_001CF26C = func_001CB790(D_001CF248, D_001CF26C);
        if (D_001CF26C == -1) D_001CF27C = 1;
        func_001CB380();
        func_001CA298();
        func_001CB95C(D_001CF208);
        DrawSync(0);
        func_000513D4();
    } while (D_001CF27C == 0);
    return D_001CF278;
}

/* ---------------- func_001CB054 (src/pilot/frontend/func_001CB054.c) ---------------- */
void func_001CB054(void)
{
    D_001CF208[1].col.r = 255;
    D_001CF208[1].col.g = 0;
    D_001CF208[1].col.b = 0;
}

/* ---------------- func_001CB070 (src/pilot/frontend/func_001CB070.c) ---------------- */
void func_001CB070(void)
{
    D_001CF208[1].col.r = 255;
    D_001CF208[1].col.g = 255;
    D_001CF208[1].col.b = 255;
}

/* ---------------- func_001CB08C (src/pilot/frontend/func_001CB08C.c) ---------------- */
int func_001CB08C(void)
{
    D_001CF27C = 1;
    D_001CF278 = 1;
    return -1;
}

/* ---------------- func_001CB0A8 (src/pilot/frontend/func_001CB0A8.c) ---------------- */
void func_001CB0A8(void)
{
    D_001CF208[2].col.r = 255;
    D_001CF208[2].col.g = 0;
    D_001CF208[2].col.b = 0;
}

/* ---------------- func_001CB0C4 (src/pilot/frontend/func_001CB0C4.c) ---------------- */
void func_001CB0C4(void)
{
    D_001CF208[2].col.r = 255;
    D_001CF208[2].col.g = 255;
    D_001CF208[2].col.b = 255;
}

/* ---------------- func_001CB0E0 (src/pilot/frontend/func_001CB0E0.c) ---------------- */
void func_001CB0E0(void)
{
    D_001CF27C = 1;
    D_001CF278 = -1;
}

/* ---------------- func_001CB0FC (src/pilot/frontend/func_001CB0FC.c) ---------------- */
void func_001CB0FC(void){}

/* ---------------- func_001CB104 (src/pilot/frontend/func_001CB104.c) ---------------- */
void func_001CB104(void)
{
    MENU_TEXT *p = D_001CF198;
    p += 1;
    p->col.r = 255;
    p->col.g = 255;
    p->col.b = 255;
}

/* ---------------- func_001CB124 (src/pilot/frontend/func_001CB124.c) ---------------- */
int func_001CB124(void)
{
    func_001CD5A8();
    return 0;
}

/* ---------------- func_001CB144 (src/pilot/frontend/func_001CB144.c) ---------------- */
void func_001CB144(void){}

/* ---------------- func_001CB14C (src/pilot/frontend/func_001CB14C.c) ---------------- */
void func_001CB14C(void)
{
    MENU_TEXT *p = D_001CF198;
    p += 2;
    p->col.r = 255;
    p->col.g = 255;
    p->col.b = 255;
}

/* ---------------- func_001CB16C (src/pilot/frontend/func_001CB16C.c) ---------------- */
int func_001CB16C(void)
{
    func_001CCD58();
    return 0;
}
