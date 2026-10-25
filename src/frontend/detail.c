/* Boundary evidence (pilot/TU1.md, FRONTEND section):
 * "Detail:" options (Viewing Distance ... Shadows; On/Off/Full/Flat) table D_001CE678, worker func_001C62CC.
 * START: text + .rodata pad. END: no edge crosses 0x1C6438 (any span); "On"/"Off"/"" duplicated by gameplay.c.
 */
/* detail.c: 38 functions func_001C5F48..func_001C6408 (0x1C5F48-0x1C6438)
 * Step 3: types from include/frontend.h: D_001CE534 is the MENU_TEXT table (D_001CE668/D_001CE678 are interior
 * .font symbols, kept: they keep their relocations); TX (0x28-byte records, short at +0x14) is file-local. No views.
 */

/* ---- types ---- */
#include "frontend.h"
typedef struct { char pad[0x14]; short x; char pad2[0x12]; } TX;

/* ---- externs ---- */
extern MENU_TEXT D_001CE534[];
extern int D_00096ABC;
extern int D_00096AC0;
extern int D_00096A88;
extern char D_001CE668;
extern int D_001CDECC;
extern char D_001CE678;
extern char *D_001CE784[];
extern char *D_001CE794[];
extern int D_001CE79C;
extern int D_001CE7A0;
extern TX D_001CE4D0[];

/* ---- functions defined in this file ---- */
void func_001C5F48();
void func_001C5F60();
int func_001C5F78();
void func_001C5F98();
void func_001C5FB0();
int func_001C5FC8();
void func_001C5FF4();
void func_001C600C();
int func_001C6024();
void func_001C6050();
void func_001C6068();
int func_001C6080();
void func_001C60A0();
void func_001C60B8();
int func_001C60D0();
void func_001C60D8();
void func_001C60F0();
int func_001C6108();
void func_001C6134();
void func_001C614C();
int func_001C6164();
void func_001C6184();
void func_001C619C();
int func_001C61B4();
void func_001C61D4();
void func_001C61EC();
int func_001C6204();
void func_001C6224();
void func_001C6240();
int func_001C6258();
void func_001C6278();
void func_001C6294();
int func_001C62AC();
void func_001C62CC();
void func_001C6370();
void func_001C63A4();
void func_001C63D4();
void func_001C6408();

/* ---------------- func_001C5F48 (src/pilot/frontend/func_001C5F48.c) ---------------- */
void func_001C5F48(void) {
    D_001CE534[1].font = 3;
    D_001CE534[2].font = 3;
}

/* ---------------- func_001C5F60 (src/pilot/frontend/func_001C5F60.c) ---------------- */
void func_001C5F60(void) {
    D_001CE534[1].font = 2;
    D_001CE534[2].font = 2;
}

/* ---------------- func_001C5F78 (src/pilot/frontend/func_001C5F78.c) ---------------- */
int func_001C5F78(void)
{
    func_001C62CC();
    return 0;
}

/* ---------------- func_001C5F98 (src/pilot/frontend/func_001C5F98.c) ---------------- */
void func_001C5F98(void) {
    D_001CE534[3].font = 3;
    D_001CE534[4].font = 3;
}

/* ---------------- func_001C5FB0 (src/pilot/frontend/func_001C5FB0.c) ---------------- */
void func_001C5FB0(void) {
    D_001CE534[3].font = 2;
    D_001CE534[4].font = 2;
}

/* ---------------- func_001C5FC8 (src/pilot/frontend/func_001C5FC8.c) ---------------- */
int func_001C5FC8(void)
{
    D_00096ABC ^= 1;
    func_001C62CC();
    return 0;
}

/* ---------------- func_001C5FF4 (src/pilot/frontend/func_001C5FF4.c) ---------------- */
void func_001C5FF4(void) {
    D_001CE534[5].font = 3;
    D_001CE534[6].font = 3;
}

/* ---------------- func_001C600C (src/pilot/frontend/func_001C600C.c) ---------------- */
void func_001C600C(void) {
    D_001CE534[5].font = 2;
    D_001CE534[6].font = 2;
}

/* ---------------- func_001C6024 (src/pilot/frontend/func_001C6024.c) ---------------- */
int func_001C6024(void)
{
    D_00096AC0 ^= 1;
    func_001C62CC();
    return 0;
}

/* ---------------- func_001C6050 (src/pilot/frontend/func_001C6050.c) ---------------- */
void func_001C6050(void) {
    D_001CE534[7].font = 3;
    D_001CE534[8].font = 3;
}

/* ---------------- func_001C6068 (src/pilot/frontend/func_001C6068.c) ---------------- */
void func_001C6068(void) {
    D_001CE534[7].font = 2;
    D_001CE534[8].font = 2;
}

/* ---------------- func_001C6080 (src/pilot/frontend/func_001C6080.c) ---------------- */
int func_001C6080(void)
{
    func_001C62CC();
    return 0;
}

/* ---------------- func_001C60A0 (src/pilot/frontend/func_001C60A0.c) ---------------- */
void func_001C60A0(void) {
    D_001CE534[9].font = 3;
    D_001CE534[10].font = 3;
}

/* ---------------- func_001C60B8 (src/pilot/frontend/func_001C60B8.c) ---------------- */
void func_001C60B8(void) {
    D_001CE534[9].font = 2;
    D_001CE534[10].font = 2;
}

/* ---------------- func_001C60D0 (src/pilot/frontend/func_001C60D0.c) ---------------- */
int func_001C60D0(void)
{
    return 0;
}

/* ---------------- func_001C60D8 (src/pilot/frontend/func_001C60D8.c) ---------------- */
void func_001C60D8(void) {
    D_001CE534[11].font = 3;
    D_001CE534[12].font = 3;
}

/* ---------------- func_001C60F0 (src/pilot/frontend/func_001C60F0.c) ---------------- */
void func_001C60F0(void) {
    D_001CE534[11].font = 2;
    D_001CE534[12].font = 2;
}

/* ---------------- func_001C6108 (src/pilot/frontend/func_001C6108.c) ---------------- */
int func_001C6108(void)
{
    D_00096A88 ^= 1;
    func_001C62CC();
    return 0;
}

/* ---------------- func_001C6134 (src/pilot/frontend/func_001C6134.c) ---------------- */
void func_001C6134(void) {
    D_001CE534[13].font = 3;
    D_001CE534[14].font = 3;
}

/* ---------------- func_001C614C (src/pilot/frontend/func_001C614C.c) ---------------- */
void func_001C614C(void) {
    D_001CE534[13].font = 2;
    D_001CE534[14].font = 2;
}

/* ---------------- func_001C6164 (src/pilot/frontend/func_001C6164.c) ---------------- */
int func_001C6164(void)
{
    func_001C62CC();
    return 0;
}

/* ---------------- func_001C6184 (src/pilot/frontend/func_001C6184.c) ---------------- */
void func_001C6184(void) {
    D_001CE534[15].font = 3;
    D_001CE534[16].font = 3;
}

/* ---------------- func_001C619C (src/pilot/frontend/func_001C619C.c) ---------------- */
void func_001C619C(void) {
    D_001CE534[15].font = 2;
    D_001CE534[16].font = 2;
}

/* ---------------- func_001C61B4 (src/pilot/frontend/func_001C61B4.c) ---------------- */
int func_001C61B4(void)
{
    func_001C62CC();
    return 0;
}

/* ---------------- func_001C61D4 (src/pilot/frontend/func_001C61D4.c) ---------------- */
void func_001C61D4(void) {
    D_001CE534[17].font = 3;
    D_001CE534[18].font = 3;
}

/* ---------------- func_001C61EC (src/pilot/frontend/func_001C61EC.c) ---------------- */
void func_001C61EC(void) {
    D_001CE534[17].font = 2;
    D_001CE534[18].font = 2;
}

/* ---------------- func_001C6204 (src/pilot/frontend/func_001C6204.c) ---------------- */
int func_001C6204(void)
{
    func_001C62CC();
    return 0;
}

/* ---------------- func_001C6224 (src/pilot/frontend/func_001C6224.c) ---------------- */
void func_001C6224(void)
{
    D_001CE668 = 3;
    D_001CDECC = 1;
}

/* ---------------- func_001C6240 (src/pilot/frontend/func_001C6240.c) ---------------- */
void func_001C6240(void) {
    D_001CE668 = 2;
    D_001CDECC = 0;
}

/* ---------------- func_001C6258 (src/pilot/frontend/func_001C6258.c) ---------------- */
int func_001C6258(void)
{
    func_001C62CC();
    return 0;
}

/* ---------------- func_001C6278 (src/pilot/frontend/func_001C6278.c) ---------------- */
void func_001C6278(void)
{
    D_001CE678 = 3;
    D_001CDECC = 1;
}

/* ---------------- func_001C6294 (src/pilot/frontend/func_001C6294.c) ---------------- */
void func_001C6294(void) {
    D_001CE678 = 2;
    D_001CDECC = 0;
}

/* ---------------- func_001C62AC (src/pilot/frontend/func_001C62AC.c) ---------------- */
int func_001C62AC(void)
{
    func_001C62CC();
    return 0;
}

/* ---------------- func_001C62CC (src/pilot/frontend/func_001C62CC.c) ---------------- */
/* func_001C62CC - refresh menu item texts (D_001CE534: 16-byte menu items, text pointer at +8) and two value positions.
   D_001CE794 as an unsized array (MEM_IN_STRUCT) keeps its load below the item stores; 79C/7A0 are the plain ints the
   sibling inc/dec callbacks (func_001C6370/63A4) use; D_001CE4D0 = 0x28-byte records with a short at +0x14. */
void func_001C62CC(void)
{
    D_001CE534[2].text = D_001CE784[1];
    D_001CE534[4].text = D_001CE784[D_00096ABC];
    D_001CE534[6].text = D_001CE784[D_00096AC0];
    D_001CE534[8].text = D_001CE784[1];
    D_001CE534[10].text = D_001CE794[0];
    D_001CE534[12].text = D_001CE784[D_00096A88];
    D_001CE534[16].text = D_001CE784[1];
    D_001CE534[18].text = D_001CE784[1];
    D_001CE4D0[0].x = D_001CE79C + 0x1C6;
    D_001CE4D0[1].x = D_001CE7A0 + 0x1C6;
}

/* ---------------- func_001C6370 (src/pilot/frontend/func_001C6370.c) ---------------- */
void func_001C6370(void){ if (D_001CE79C < 0x74) { D_001CE79C = D_001CE79C + 1; func_001C62CC(); } }

/* ---------------- func_001C63A4 (src/pilot/frontend/func_001C63A4.c) ---------------- */
void func_001C63A4(void){ if (D_001CE79C > 0) { D_001CE79C = D_001CE79C - 1; func_001C62CC(); } }

/* ---------------- func_001C63D4 (src/pilot/frontend/func_001C63D4.c) ---------------- */
void func_001C63D4(void){ if (D_001CE7A0 < 0x74) { D_001CE7A0 = D_001CE7A0 + 1; func_001C62CC(); } }

/* ---------------- func_001C6408 (src/pilot/frontend/func_001C6408.c) ---------------- */
void func_001C6408(void){ if (D_001CE7A0 > 0) { D_001CE7A0 = D_001CE7A0 - 1; func_001C62CC(); } }
