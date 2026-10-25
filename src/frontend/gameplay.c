/* Boundary evidence (pilot/TU1.md, FRONTEND section):
 * "Gameplay:" options (Vibration, Auto Handbrake, Cop Difficulty) func_001C6438 + table D_001CE84C. END: .rodata
 * pad 0x1C0694; "On"/"Off"/"Accept Changes" duplicated by soundopts.c; no edge crosses 0x1C6748 (any span).
 */
/* gameplay.c: 14 functions func_001C6438..func_001C66B4 (0x1C6438-0x1C6748)
 * Step 3: types from include/frontend.h: D_001CE7BC is the MENU_TEXT table (D_001CE830 = its [7].font, kept as its own
 * symbol: an interior symbol keeps its relocation), D_001CE84C the MENU_ITEM table. No views.
 */

/* ---- types ---- */
#include "frontend.h"

/* ---- externs ---- */
extern int D_00096870;
extern int D_00096784;
extern int D_000967EC;
extern int D_001D2E28;
extern int D_001D2E2C;
extern int D_001D2E30;
extern int D_001CE88C;
extern MENU_ITEM D_001CE84C[];
extern char D_001CE7A8[];
extern MENU_TEXT D_001CE7BC[];
extern void func_001C2644();
extern void func_001C2114();
extern void func_000514E0();
extern int func_001C26E4();
extern void func_001C19C4();
extern void func_001C1BD4();
extern void func_001C11E4();
extern char D_001CE830;
extern unsigned char D_000A801D;
extern char *D_001CE898[];
extern char *D_001CE890[];

/* ---- functions defined in this file ---- */
int func_001C6438();
void func_001C6554();
void func_001C6564();
int func_001C6574();
void func_001C657C();
void func_001C6594();
int func_001C65AC();
void func_001C65E8();
void func_001C6600();
int func_001C6618();
void func_001C6644();
void func_001C665C();
int func_001C6674();
void func_001C66B4();

/* ---------------- func_001C6438 (src/pilot/frontend/func_001C6438.c) ---------------- */
int func_001C6438(void)
{
    int r;
    D_001D2E28 = D_00096870;
    D_001D2E2C = D_00096784;
    D_001D2E30 = D_000967EC;
    func_001C66B4();
    D_001CE88C = 0;
    func_001C2644(D_001CE84C, 4, 0);
    func_001C2114(D_001CE7A8, D_001CE7BC, 1);
    for (;;) {
        func_000514E0();
        r = func_001C26E4(D_001CE84C, &D_001CE88C);
        if (r != 0) break;
        func_001C19C4();
        func_001C1BD4(D_001CE7BC);
        func_001C11E4();
    }
    if (r != -1) return 0;
    D_00096870 = D_001D2E28;
    D_00096784 = D_001D2E2C;
    D_000967EC = D_001D2E30;
    return 0;
}

/* ---------------- func_001C6554 (src/pilot/frontend/func_001C6554.c) ---------------- */
void func_001C6554(void) {
    D_001CE830 = 3;
}

/* ---------------- func_001C6564 (src/pilot/frontend/func_001C6564.c) ---------------- */
void func_001C6564(void) {
    D_001CE830 = 2;
}

/* ---------------- func_001C6574 (src/pilot/frontend/func_001C6574.c) ---------------- */
int func_001C6574(void){return 1;}

/* ---------------- func_001C657C (src/pilot/frontend/func_001C657C.c) ---------------- */
void func_001C657C(void) {
    D_001CE7BC[1].font = 3;
    D_001CE7BC[2].font = 3;
}

/* ---------------- func_001C6594 (src/pilot/frontend/func_001C6594.c) ---------------- */
void func_001C6594(void) {
    D_001CE7BC[1].font = 2;
    D_001CE7BC[2].font = 2;
}

/* ---------------- func_001C65AC (src/pilot/frontend/func_001C65AC.c) ---------------- */
int func_001C65AC(void){ D_00096870 = D_00096870 + 1; if (D_00096870 == 3) D_00096870 = 0; func_001C66B4(); return 0; }

/* ---------------- func_001C65E8 (src/pilot/frontend/func_001C65E8.c) ---------------- */
void func_001C65E8(void) {
    D_001CE7BC[3].font = 3;
    D_001CE7BC[4].font = 3;
}

/* ---------------- func_001C6600 (src/pilot/frontend/func_001C6600.c) ---------------- */
void func_001C6600(void)
{
    D_001CE7BC[3].font = 2;
    D_001CE7BC[4].font = 2;
}

/* ---------------- func_001C6618 (src/pilot/frontend/func_001C6618.c) ---------------- */
int func_001C6618(void){ D_00096784 ^= 1; func_001C66B4(); return 0; }

/* ---------------- func_001C6644 (src/pilot/frontend/func_001C6644.c) ---------------- */
void func_001C6644(void)
{
    D_001CE7BC[5].font = 3;
    D_001CE7BC[6].font = 3;
}

/* ---------------- func_001C665C (src/pilot/frontend/func_001C665C.c) ---------------- */
void func_001C665C(void)
{
    D_001CE7BC[5].font = 2;
    D_001CE7BC[6].font = 2;
}

/* ---------------- func_001C6674 (src/pilot/frontend/func_001C6674.c) ---------------- */
int func_001C6674(void){ if (D_000A801D & 4) { D_000967EC ^= 1; func_001C66B4(); } return 0; }

/* ---------------- func_001C66B4 (src/pilot/frontend/func_001C66B4.c) ---------------- */
void func_001C66B4(void)
{
    D_001CE7BC[2].text = D_001CE898[D_00096870];
    D_001CE7BC[4].text = D_001CE890[D_00096784];
    if (D_000A801D & 4) {
        D_001CE7BC[6].text = D_001CE890[D_000967EC];
    } else {
        D_000967EC = 0;
        D_001CE7BC[6].text = D_001CE890[0];
    }
}

