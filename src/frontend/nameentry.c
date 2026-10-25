/* Boundary evidence (pilot/TU1.md, FRONTEND section):
 * name entry ("Enter Name:", "Name: %s ", Delete/Accept), func_001C8330 called from main 0x6E7F0. START: text pad.
 * END: no call/data edge crosses 0x1C8D08 at any span.
 */
/* nameentry.c: 3 functions func_001C8330..func_001C8640 (0x1C8330-0x1C8D08)
 * Step 3: types from include/frontend.h + structs.h (MENU_TEXT D_001CEE00, DB *current, POLY_F4); D_001D30E0 is
 * unsigned char everywhere (char is unsigned). PAIR is file-local. No views.
 */

/* ---- types ---- */
#include "frontend.h"
typedef struct { unsigned char a, b; } PAIR;

/* ---- externs ---- */
extern MENU_TEXT D_001CEE00[];
extern int D_001CEE30;
extern unsigned char D_001CEE37;
extern int D_001D30D8;
extern int D_001D30E8;
extern char D_001D30E0[];
extern void func_000514E0();
extern void func_001C1BD4();
extern void func_001C19C4();
extern void func_001C11E4();
extern int sprintf();
extern char *strcpy();
extern DB *current;
extern char D_001C0928[];
extern void func_0005D4C0();
extern void SetTextColour(unsigned char, unsigned char, unsigned char);
extern int PrintString();
extern int PrintStringRightAligned();
extern unsigned short D_0009752C;
extern unsigned short D_00097538;
extern unsigned char D_000BF6EC[];
extern unsigned char D_001CEE34[];
extern PAIR D_001CEE58[];
extern PAIR D_001CEE38[];
extern int func_0005D86C();

/* ---- functions defined in this file ---- */
int func_001C8330();
void func_001C84C0();
int func_001C8640();

/* ---------------- func_001C8330 (src/pilot/frontend/func_001C8330.c) ---------------- */
int func_001C8330(char *dst)
{
    char buf[128];
    int blink = 0;
    int r;

    D_001D30D8 = 0x21;
    D_001D30E8 = 0;
    for (;;) {
        func_000514E0();
        r = func_001C8640();
        if (r != 0)
            break;
        sprintf(buf, "Name: %s ", D_001D30E0);
        if (++blink > 32) {
            D_001CEE37 ^= 1;
            blink = 0;
        }
        if (D_001CEE30 < 6) {
            if (D_001CEE37 == 1) {
                D_001D30E0[D_001CEE30] = '-';
                D_001D30E0[D_001CEE30 + 1] = 0;
            } else {
                D_001D30E0[D_001CEE30] = 0;
                D_001D30E0[D_001CEE30 + 1] = 0;
            }
        }
        D_001CEE00[1].text = buf;
        func_001C1BD4(D_001CEE00);
        func_001C84C0(D_001D30D8);
        blink++;
        func_001C19C4();
        func_001C11E4();
    }
    if (r == -1)
        return 0;
    D_001D30E0[D_001CEE30] = 0;
    strcpy(dst, D_001D30E0);
    return r;
}

/* ---------------- func_001C84C0 (src/pilot/frontend/func_001C84C0.c) ---------------- */
/* FAKEMATCH: unused char pad[8] only sizes the frame (0x28); probably an unused local aggregate (RECT?) in the original. */
#define addPrim(ot, p) (((P_TAG *)(p))->addr = ((P_TAG *)(ot))->addr, ((P_TAG *)(ot))->addr = (unsigned)(p))
void func_001C84C0(int sel)
{
    POLY_F4 *poly;
    char pad[8]; /* FAKEMATCH: frame size */

    func_0005D4C0(0x22, 0xC2, 0x10C, sel, 1);
    if (sel == 0x80)
        SetTextColour(255, 0, 0);
    PrintString("Delete", 0x22, 0x11E);
    SetTextColour(255, 255, 255);
    if (sel == 0x81)
        SetTextColour(255, 0, 0);
    PrintStringRightAligned(D_001C0928, 0x112, 0x11E);
    SetTextColour(255, 255, 255);

    poly = (POLY_F4 *)current->primptr;
    ((P_TAG *)poly)->len = 5;
    poly->code = 0x2A;
    poly->x0 = 0x20;
    poly->y0 = 0xBE;
    poly->x1 = 0x124;
    poly->y1 = 0xBE;
    poly->x2 = 0x20;
    poly->y2 = 0x136;
    poly->x3 = 0x124;
    poly->y3 = 0x136;
    poly->r0 = 16;
    poly->g0 = 16;
    poly->b0 = 16;
    addPrim(current->ot[8], poly);
    current->primptr += sizeof(POLY_F4);
}
#undef addPrim

/* ---------------- func_001C8640 (src/pilot/frontend/func_001C8640.c) ---------------- */
/* Name-entry keyboard: cursor D_001D30D8 over AsciiTable (0..0x7F chars, 0x80 = DEL, 0x81 = END), 15 columns */
static inline unsigned char inlist(unsigned char c)
{
    unsigned char found = 0;
    unsigned char *p = D_001CEE34;
    unsigned char *end = p + 3;

    for (; p < end; p++) {
        if (*p == c) {
            found = 1;
            break;
        }
    }
    return found;
}
static inline void skip_fwd(void)
{
    for (;;) {
        if (func_0005D86C(D_000BF6EC[D_001D30D8]) && !inlist(D_001D30D8))
            break;
        if (++D_001D30D8 > 0x80) {
            D_001D30D8 = 0x81;
            break;
        }
    }
}
static inline void skip_back(void)
{
    for (;;) {
        if (func_0005D86C(D_000BF6EC[D_001D30D8]) && !inlist(D_001D30D8))
            break;
        if (--D_001D30D8 < 0) {
            D_001D30D8 = 0x81;
            break;
        }
    }
}
int func_001C8640(void)
{
    int ret = 0;
    unsigned short pad = D_0009752C;
    int found;
    int found2;
    int i;
    int n;

    if (pad & 0x2000) {
        if (D_001D30E8 <= 0) {
            found = 0;
            for (i = 0; i < 5; i++) {
                if (D_001D30D8 == D_001CEE58[i].b) {
                    D_001D30D8 = D_001CEE58[i].a;
                    found = 1;
                }
                D_001D30E8 = 8;
            }
            if (!found) {
                if (D_001D30D8 == 0x81)
                    D_001D30D8 = 0x80;
                else if (D_001D30D8 == 0x80)
                    D_001D30D8 = 0x81;
                else {
                    if (++D_001D30D8 > 0x81)
                        D_001D30D8 = 0;
                    if (D_001D30D8 < 0x80)
                        skip_fwd();
                }
                D_001D30E8 = 8;
            }
        }
    } else if (pad & 0x8000) {
        if (D_001D30E8 <= 0) {
            found = 0;
            for (i = 0; i < 5; i++) {
                if (D_001D30D8 == D_001CEE58[i].a) {
                    D_001D30D8 = D_001CEE58[i].b;
                    found = 1;
                }
                D_001D30E8 = 8;
            }
            if (!found) {
                if (D_001D30D8 == 0x81)
                    D_001D30D8 = 0x80;
                else if (D_001D30D8 == 0x80)
                    D_001D30D8 = 0x81;
                else {
                    if (--D_001D30D8 < 0)
                        D_001D30D8 = 0x81;
                    if (D_001D30D8 < 0x80)
                        skip_back();
                }
                D_001D30E8 = 8;
            }
        }
    } else if (pad & 0x4000) {
        if (D_001D30E8 <= 0) {
            found2 = 0;
            for (i = 0; i < 15; i++) {
                if (D_001D30D8 == D_001CEE38[i].b) {
                    if (D_001D30D8 >= 0x6F && D_001D30D8 < 0x75)
                        D_001D30D8 = 0x80;
                    else if (D_001D30D8 >= 0x70)
                        D_001D30D8 = 0x81;
                    else
                        D_001D30D8 = D_001CEE38[i].a;
                    found2 = 1;
                }
                D_001D30E8 = 8;
            }
            if (!found2) {
                n = 0;
                if (D_001D30D8 == 0x80)
                    D_001D30D8 = 0x21;
                else if (D_001D30D8 == 0x81)
                    D_001D30D8 = 0x36;
                else {
                    do {
                        D_001D30D8++;
                        n++;
                        skip_fwd();
                    } while (n < 15);
                }
                D_001D30E8 = 8;
            }
        }
    } else if (pad & 0x1000) {
        if (D_001D30E8 <= 0) {
            found = 0;
            for (i = 0; i < 15; i++) {
                if (D_001D30D8 == D_001CEE38[i].a) {
                    D_001D30D8 = D_001CEE38[i].b;
                    found = 1;
                }
                D_001D30E8 = 8;
            }
            if (!found) {
                n = 15;
                if (D_001D30D8 == 0x80)
                    D_001D30D8 = 0x6F;
                else if (D_001D30D8 == 0x81)
                    D_001D30D8 = 0x7A;
                else {
                    do {
                        D_001D30D8--;
                        n--;
                        skip_back();
                    } while (n > 0);
                }
                D_001D30E8 = 8;
            }
        }
    } else if (D_00097538 & 0x10) {
        ret = -1;
    } else if (D_00097538 & 0x40) {
        if (D_001D30D8 == 0x81)
            ret = 1;
        else if (D_001D30D8 == 0x80) {
            if (D_001CEE30 > 0) {
                D_001D30E0[D_001CEE30] = ' ';
                D_001CEE30--;
            }
        } else if (D_001CEE30 < 6) {
            D_001D30E0[D_001CEE30] = D_001D30D8;
            D_001CEE30++;
        }
    }
    if (--D_001D30E8 < 0)
        D_001D30E8 = 0;
    return ret;
}

