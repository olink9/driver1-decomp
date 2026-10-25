/* Boundary evidence (pilot/TU1.md, FRONTEND section):
 * "Car Chases:" menu (Play Chase 1-4, Load Film) func_001C8D08 + table D_001CEED8; frontend\font3.bnk/DRFONT3
 * duplicate newgame.c's. END: .rodata pad 0x1C09C4. .rodata one block 0x1C0990.
 */
/* carchases.c: 5 functions func_001C8D08..func_001C8F54 (0x1C8D08-0x1C9048)
 * Step 3: types from include/frontend.h (MENU_TEXT/MENU_ITEM tables, DB_ENV D_000BDFA4[2]); no views left.
 */

/* ---- types ---- */
#include "frontend.h"

/* ---- externs ---- */
extern int D_001CEF28;
extern int D_001CEF2C;
extern MENU_ITEM D_001CEED8[];
extern MENU_TEXT D_001CEE68[];
extern void func_001C2644();
extern void func_001C2114();
extern void func_000514E0();
extern int func_001C26E4();
extern void func_001C19C4();
extern void func_001C1BD4();
extern void func_001C11E4();
extern int D_00096808;
extern int D_000967F4;
extern void func_00024754();
extern int D_000967F0;
extern int D_0009763C;
extern DB_ENV D_000BDFA4[];
extern char D_00096895;
extern void func_000EC280();
extern void func_000E6D60();
extern void func_0007457C();
extern void func_0006F6E8();
extern void func_000E7508();
extern void SetDispMask();
extern void func_001C2DAC();
extern void func_001C2F78();

/* ---- functions defined in this file ---- */
int func_001C8D08();
void func_001C8DC8();
void func_001C8DF0();
int func_001C8E20();
int func_001C8F54();

/* ---------------- func_001C8D08 (src/pilot/frontend/func_001C8D08.c) ---------------- */
int func_001C8D08(void)
{
    int r;
    D_001CEF28 = 0;
    func_001C2644(D_001CEED8, 5, 0);
    func_001C2114(0, D_001CEE68, 1);
    for (;;) {
        func_000514E0();
        D_001CEF2C = D_001CEF28;
        r = func_001C26E4(D_001CEED8, &D_001CEF28);
        if (r != 0) break;
        func_001C19C4();
        func_001C1BD4(D_001CEE68);
        func_001C11E4();
    }
    if (r == -1) return 0;
    return r;
}

/* ---------------- func_001C8DC8 (src/pilot/frontend/func_001C8DC8.c) ---------------- */
void func_001C8DC8(void)
{
    D_001CEE68[D_001CEF28 + 1].font = 1;
}

/* ---------------- func_001C8DF0 (src/pilot/frontend/func_001C8DF0.c) ---------------- */
void func_001C8DF0(void){ if (D_001CEF2C != -1) D_001CEE68[D_001CEF2C + 1].font = 0; }

/* ---------------- func_001C8E20 (src/pilot/frontend/func_001C8E20.c) ---------------- */
int func_001C8E20(void)
{
    func_001C2114(0, D_001CEE68, 0);
    switch (D_001CEF28) {
    case 0:
        func_001C8F54();
        break;
    case 1:
        D_00096808 = 1;
        D_000967F4 = 7;
        func_00024754();
        func_001C2644(D_001CEED8, 5, 1);
        break;
    case 2:
        D_00096808 = 2;
        D_000967F4 = 7;
        func_00024754();
        func_001C2644(D_001CEED8, 5, 2);
        break;
    case 3:
        D_00096808 = 3;
        D_000967F4 = 7;
        func_00024754();
        func_001C2644(D_001CEED8, 5, 3);
        break;
    case 4:
        D_00096808 = 4;
        D_000967F4 = 7;
        func_00024754();
        func_001C2644(D_001CEED8, 5, 4);
        break;
    case 5:
        break;
    }
    return 1;
}

/* ---------------- func_001C8F54 (src/pilot/frontend/func_001C8F54.c) ---------------- */
/* func_001C8F54: early `if (!x) { ...; return 0; }` + final `return 1;` (if/else with one r reorders v0) */
int func_001C8F54(void)
{
    func_000EC280(0);
    func_000E6D60("frontend\\font3.bnk", "DRFONT3", 1, -2);
    func_0007457C(D_000BDFA4[0].ot, 0x1080);
    func_0007457C(D_000BDFA4[1].ot, 0x1080);
    func_0006F6E8();
    func_000E7508(1, 0x1FABBC, 0x3E00);
    SetDispMask(0);
    if (D_000967F0 == 0) {
        func_001C2DAC(0x280);
        func_001C2F78();
        func_0007457C(D_000BDFA4[0].ot, 0x1080);
        func_0007457C(D_000BDFA4[1].ot, 0x1080);
        SetDispMask(1);
        return 0;
    }
    SetDispMask(0);
    D_00096895 = 1;
    D_0009763C = 0;
    return 1;
}
