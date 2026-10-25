/* Boundary evidence (pilot/TU1.md, FRONTEND section):
 * Hi-Score Options (Save/Load Hi-Scores, View Tables) func_001C7CA8 + table D_001CEDC4, high-score tables
 * func_001C8018 + D_001CED84; the two tables' .data interleave -> one object. START: .rodata pad 0x1C087C. END:
 * text pad at 0x1C832C.
 */
/* hiscores.c: 24 functions func_001C7CA8..func_001C82C8 (0x1C7CA8-0x1C8330)
 * Step 3: types from include/frontend.h (MENU_TEXT D_001CED34/D_001CECD4, MENU_ITEM D_001CED84/D_001CEDC4, DB_ENV);
 * BIG = the 0x5E4-byte block copied D_000A083C -> D_000868DC (file-local). No views.
 */

/* ---- types ---- */
#include "frontend.h"
typedef struct { char c[0x5E4]; } BIG __attribute__((aligned(4)));

/* ---- externs ---- */
extern int D_001CEDF8;
extern MENU_ITEM D_001CEDC4[];
extern MENU_TEXT D_001CED34[];
extern void func_001C2644();
extern void func_001C2114();
extern void func_000514E0();
extern int func_001C26E4();
extern void func_001C19C4();
extern void func_001C1BD4();
extern void func_001C11E4();
extern char D_001CED48;
extern char D_001CED58;
extern int D_000967F0;
extern DB_ENV D_000BDFA4[];
extern BIG D_000A083C;
extern BIG D_000868DC;
extern void func_0007457C();
extern void func_0006F6E8();
extern void func_000EC280();
extern void func_000E7508();
extern void func_001C2DAC();
extern void func_001C2F78();
extern void func_001C1B4C();
extern void SetDispMask();
extern char D_001CED68;
extern int D_001CEDF4;
extern MENU_ITEM D_001CED84[];
extern MENU_TEXT D_001CECD4[];
extern char D_001C08FC[];
extern char D_001CECE8;
extern int D_001CECB8;
extern int func_001C7730();
extern char D_001CECF8;
extern char D_001CED08;
extern char D_001CED18;

/* ---- functions defined in this file ---- */
int func_001C7CA8();
void func_001C7D78();
void func_001C7D88();
int func_001C7D94();
void func_001C7DEC();
void func_001C7DFC();
int func_001C7E08();
void func_001C7F2C();
void func_001C7F3C();
int func_001C7F48();
int func_001C8018();
void func_001C80EC();
void func_001C8134();
void func_001C8144();
int func_001C8150();
void func_001C81B0();
void func_001C81C0();
int func_001C81CC();
void func_001C822C();
void func_001C823C();
int func_001C8248();
void func_001C82AC();
void func_001C82BC();
int func_001C82C8();

/* ---------------- func_001C7CA8 (src/pilot/frontend/func_001C7CA8.c) ---------------- */
int func_001C7CA8(void)
{
    int r;
    D_001CEDF8 = 0;
    func_001C2644(D_001CEDC4, 3, 0);
    func_001C2114(0, D_001CED34, 1);
    for (;;) {
        func_000514E0();
        r = func_001C26E4(D_001CEDC4, &D_001CEDF8);
        if (r != 0) break;
        func_001C19C4();
        func_001C1BD4(D_001CED34);
        func_001C11E4();
    }
    func_001C2114(0, D_001CED34, 0);
    if (r == -1) return 0;
    return r;
}

/* ---------------- func_001C7D78 (src/pilot/frontend/func_001C7D78.c) ---------------- */
void func_001C7D78(void) {
    D_001CED48 = 1;
}

/* ---------------- func_001C7D88 (src/pilot/frontend/func_001C7D88.c) ---------------- */
void func_001C7D88(void) { D_001CED48 = 0; }

/* ---------------- func_001C7D94 (src/pilot/frontend/func_001C7D94.c) ---------------- */
int func_001C7D94(void){ int r; func_001C2114(0, D_001CED34, 0); r = func_001C8018(); func_001C2114(0, D_001CED34, 1); return r; }

/* ---------------- func_001C7DEC (src/pilot/frontend/func_001C7DEC.c) ---------------- */
void func_001C7DEC(void) {
    D_001CED58 = 1;
}

/* ---------------- func_001C7DFC (src/pilot/frontend/func_001C7DFC.c) ---------------- */
void func_001C7DFC(void) { D_001CED58 = 0; }

/* ---------------- func_001C7E08 (src/pilot/frontend/func_001C7E08.c) ---------------- */
int func_001C7E08(void)
{
    func_001C2114(0, D_001CED34, 0);
    func_0007457C(D_000BDFA4[0].ot, 0x1080);
    func_0007457C(D_000BDFA4[1].ot, 0x1080);
    func_0006F6E8();
    func_000EC280(0);
    func_000E7508(2, &D_000A083C, 0x1E00);
    if (D_000967F0 != 0) {
        D_000868DC = D_000A083C;
    }
    func_001C2DAC(0x280);
    func_001C2F78();
    func_001C1B4C();
    func_0007457C(D_000BDFA4[0].ot, 0x1080);
    func_0007457C(D_000BDFA4[1].ot, 0x1080);
    SetDispMask(1);
    func_001C2114(0, D_001CED34, 1);
    return 0;
}

/* ---------------- func_001C7F2C (src/pilot/frontend/func_001C7F2C.c) ---------------- */
void func_001C7F2C(void) {
    D_001CED68 = 1;
}

/* ---------------- func_001C7F3C (src/pilot/frontend/func_001C7F3C.c) ---------------- */
void func_001C7F3C(void) { D_001CED68 = 0; }

/* ---------------- func_001C7F48 (src/pilot/frontend/func_001C7F48.c) ---------------- */
int func_001C7F48(void)
{
    func_001C2114(0, D_001CED34, 0);
    func_0007457C(D_000BDFA4[0].ot, 0x1080);
    func_0007457C(D_000BDFA4[1].ot, 0x1080);
    func_0006F6E8();
    func_000EC280(0);
    func_000E7508(6, &D_000868DC, 0x1E00);
    func_001C2DAC(0x280);
    func_001C2F78();
    func_001C1B4C();
    func_0007457C(D_000BDFA4[0].ot, 0x1080);
    func_0007457C(D_000BDFA4[1].ot, 0x1080);
    SetDispMask(1);
    func_001C2114(0, D_001CED34, 1);
    return 0;
}

/* ---------------- func_001C8018 (src/pilot/frontend/func_001C8018.c) ---------------- */
int func_001C8018(void)
{
    int r;
    func_001C80EC();
    D_001CEDF4 = 0;
    func_001C2644(D_001CED84, 3, 0);
    func_001C2114(0, D_001CECD4, 1);
    for (;;) {
        func_000514E0();
        r = func_001C26E4(D_001CED84, &D_001CEDF4);
        if (r != 0) break;
        func_001C19C4();
        func_001C1BD4(D_001CECD4);
        func_001C11E4();
    }
    func_001C2114(0, D_001CECD4, 0);
    if (r == -1) return 0;
    return r;
}

/* ---------------- func_001C80EC (src/pilot/frontend/func_001C80EC.c) ---------------- */
void func_001C80EC(void){ D_001CECD4[0].text = "<High Scores>"; D_001CECD4[1].text = "Checkpoint"; D_001CECD4[2].text = "Trail Blazer"; D_001CECD4[3].text = "Survival"; D_001CECD4[4].text = D_001C08FC; }

/* ---------------- func_001C8134 (src/pilot/frontend/func_001C8134.c) ---------------- */
void func_001C8134(void) {
    D_001CECE8 = 1;
}

/* ---------------- func_001C8144 (src/pilot/frontend/func_001C8144.c) ---------------- */
void func_001C8144(void) { D_001CECE8 = 0; }

/* ---------------- func_001C8150 (src/pilot/frontend/func_001C8150.c) ---------------- */
int func_001C8150(void)
{
    int r;
    func_001C2114(0, D_001CECD4, 0);
    D_001CECB8 = 0x12;
    r = func_001C7730();
    func_001C2114(0, D_001CECD4, 1);
    return r;
}

/* ---------------- func_001C81B0 (src/pilot/frontend/func_001C81B0.c) ---------------- */
void func_001C81B0(void) {
    D_001CECF8 = 1;
}

/* ---------------- func_001C81C0 (src/pilot/frontend/func_001C81C0.c) ---------------- */
void func_001C81C0(void) { D_001CECF8 = 0; }

/* ---------------- func_001C81CC (src/pilot/frontend/func_001C81CC.c) ---------------- */
int func_001C81CC(void)
{
    int r;
    func_001C2114(0, D_001CECD4, 0);
    D_001CECB8 = 0x11;
    r = func_001C7730();
    func_001C2114(0, D_001CECD4, 1);
    return r;
}

/* ---------------- func_001C822C (src/pilot/frontend/func_001C822C.c) ---------------- */
void func_001C822C(void) {
    D_001CED08 = 1;
}

/* ---------------- func_001C823C (src/pilot/frontend/func_001C823C.c) ---------------- */
void func_001C823C(void) { D_001CED08 = 0; }

/* ---------------- func_001C8248 (src/pilot/frontend/func_001C8248.c) ---------------- */
int func_001C8248(void)
{
    int r;
    D_001CECB8 = 0x13;
    func_001C2114(0, D_001CECD4, 0);
    r = func_001C7730();
    func_001C2114(0, D_001CECD4, 1);
    return r;
}

/* ---------------- func_001C82AC (src/pilot/frontend/func_001C82AC.c) ---------------- */
void func_001C82AC(void) {
    D_001CED18 = 1;
}

/* ---------------- func_001C82BC (src/pilot/frontend/func_001C82BC.c) ---------------- */
void func_001C82BC(void) { D_001CED18 = 0; }

/* ---------------- func_001C82C8 (src/pilot/frontend/func_001C82C8.c) ---------------- */
int func_001C82C8(void)
{
    int r;
    D_001CECB8 = 0x16;
    func_001C2114(0, D_001CECD4, 0);
    r = func_001C7730();
    func_001C2114(0, D_001CECD4, 1);
    return r;
}
