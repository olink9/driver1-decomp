/* Boundary evidence (pilot/TU1.md, FRONTEND section):
 * New Game / Load Saved menu func_001C6DE0 + table D_001CEA18, func_001C6F40 reloads frontend\font3.bnk
 * (duplicated by carchases.c). START: text pad. END: no call/data edge crosses 0x1C70F0 at any span (medium: no
 * padding visible there).
 */
/* newgame.c: 7 functions func_001C6DE0..func_001C6F40 (0x1C6DE0-0x1C70F0)
 * Step 3: types from include/frontend.h (MENU_TEXT D_001CE9E8, MENU_ITEM D_001CEA18, DB_ENV D_000BDFA4[2]); no views.
 */

/* ---- types ---- */
#include "frontend.h"

/* ---- externs ---- */
extern int D_001CEA38;
extern MENU_ITEM D_001CEA18[];
extern MENU_TEXT D_001CE9E8[];
extern void func_001C2644();
extern void func_001C2114();
extern void func_000514E0();
extern int func_001C26E4();
extern void func_001C19C4();
extern void func_001C1BD4();
extern void func_001C11E4();
extern char D_001CE9EC;
extern int D_000967F4;
extern void DrawSync();
extern void VSync();
extern void SetDispMask();
extern void func_00024754();
extern char D_001CE9FC;
extern int D_000967F0;
extern int D_000A083C;
extern DB_ENV D_000BDFA4[];
extern void func_000EC280();
extern void func_000E6D60();
extern void func_0006F6E8();
extern void func_000E7508();
extern void func_001C2DAC();
extern void func_001C2F78();
extern void func_001C1B4C();
extern void func_0007457C();

/* ---- functions defined in this file ---- */
int func_001C6DE0();
void func_001C6EA4();
void func_001C6EBC();
int func_001C6EC8();
void func_001C6F24();
void func_001C6F34();
int func_001C6F40();

/* ---------------- func_001C6DE0 (src/pilot/frontend/func_001C6DE0.c) ---------------- */
int func_001C6DE0(void)
{
    int r;
    D_001CEA38 = 0;
    func_001C2644(D_001CEA18, 2, 0);
    func_001C2114(0, D_001CE9E8, 1);
    for (;;) {
        func_000514E0();
        r = func_001C26E4(D_001CEA18, &D_001CEA38);
        if (r != 0) break;
        func_001C19C4();
        func_001C1BD4(D_001CE9E8);
        func_001C11E4();
    }
    func_001C2114(0, D_001CE9E8, 0);
    return r;
}

/* ---------------- func_001C6EA4 (src/pilot/frontend/func_001C6EA4.c) ---------------- */
/* main .sdata: $gp-relative in main, absolute here (-G0) */
void func_001C6EA4(void)
{
    D_001CE9EC = 1;
    D_000967F4 = 0;
}

/* ---------------- func_001C6EBC (src/pilot/frontend/func_001C6EBC.c) ---------------- */
void func_001C6EBC(void) { D_001CE9EC = 0; }

/* ---------------- func_001C6EC8 (src/pilot/frontend/func_001C6EC8.c) ---------------- */
int func_001C6EC8(void){ DrawSync(0); VSync(0); func_001C11E4(); D_000967F4 = 0; func_001C2114(0, D_001CE9E8, 0); SetDispMask(0); func_00024754(); return 1; }

/* ---------------- func_001C6F24 (src/pilot/frontend/func_001C6F24.c) ---------------- */
void func_001C6F24(void) {
    D_001CE9FC = 1;
}

/* ---------------- func_001C6F34 (src/pilot/frontend/func_001C6F34.c) ---------------- */
void func_001C6F34(void) { D_001CE9FC = 0; }

/* ---------------- func_001C6F40 (src/pilot/frontend/func_001C6F40.c) ---------------- */
int func_001C6F40(void)
{
    int r;

    func_001C2114(0, D_001CE9E8, 0);
    func_000EC280(0);
    func_000E6D60("frontend\\font3.bnk", "DRFONT3", 1, -2);
    func_0006F6E8();
    func_000E7508(0, &D_000A083C, 0x1E00);
    SetDispMask(0);
    if (D_000967F0 == 0) {
        DB_ENV *p;
        func_001C2DAC(0x280);
        func_001C2F78();
        p = D_000BDFA4;
        func_001C1B4C();
        func_0007457C(p[0].ot, 0x1080);
        func_0007457C(p[1].ot, 0x1080);
        SetDispMask(1);
    } else {
        SetDispMask(0);
    }
    r = 0;
    func_001C2644(D_001CEA18, 2, D_001CEA38);
    if (D_000967F0 != 0) {
        if (D_000A083C != -1) {
            DrawSync(r);
            VSync(r);
            func_001C11E4();
            D_000967F4 = 0;
            func_001C2114(r, D_001CE9E8, r);
        } else {
            D_000967F0 = 0;
            DrawSync(0);
            VSync(0);
            func_001C11E4();
            D_000967F4 = 0;
            func_001C2114(0, D_001CE9E8, 0);
        }
        r = 1;
        SetDispMask(0);
        func_00024754();
    } else {
        func_001C2114(0, D_001CE9E8, 1);
    }
    return r;
}
