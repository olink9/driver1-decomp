/* Boundary evidence (pilot/TU1.md, FRONTEND section):
 * in-game "Options:" (Vibration, Auto Handbrake, Sfx/Music Volume; duplicates of options.c/gameplay.c/soundopts.c
 * strings -> own object) func_001CD5A8 + table D_001CF7C0; func_001CD7C0 is a wip (INCLUDE_ASM). START: text pad
 * at 0x1CD5A4. END: end of .text.
 */
/* ladoptions.c: 24 functions func_001CD5A8..func_001CDC44 (0x1CD5A8-0x1CDC80)
 * Step 3: types from include/frontend.h: D_001CF710 is a MENU_TEXT table (the colour-only records were views
 * of it). No views.
 */

/* include_asm.h's INCLUDE_ASM minus its .include of labels.inc (the build pipeline prepends macro.inc) */
#define INCLUDE_ASM(FOLDER, NAME) \
    __asm__(".section .text\n.set noat\n.set noreorder\n.include \"" FOLDER "/" #NAME ".s\"\n.set reorder\n.set at\n")

/* ---- types ---- */
#include "frontend.h"

/* ---- externs ---- */
extern int D_00096C94;
extern int D_00096C90;
extern int D_000967EC;
extern int D_00096784;
extern int D_001D32F8;
extern int D_001D32FC;
extern int D_001D3300;
extern int D_001D3304;
extern int D_001CF850;
extern int D_001CF878;
extern int D_001CDECC;
extern int D_001CEF88;
extern int D_001CEF54;
extern char D_001CF7C0[];
extern MENU_TEXT D_001CF710[];
extern void func_001C9A18();
extern void func_000514E0();
extern void VSync();
extern int func_001CB790();
extern void func_001CB380();
extern void func_001CA298();
extern void func_001CB95C();
extern void DrawSync();
extern void func_000513D4();
extern void func_0004E454();
extern void SetMasterVolume();
extern void SetTextColour();
extern void func_001CA270();
extern unsigned char D_000A801D;
extern char *D_001CF85C;
extern char *D_001CF860;
extern char D_001D32F0[];
extern char D_001D32F4[];
extern int sprintf();
extern int D_001CF880;
extern int D_001CF87C;
extern int D_001CF864[];

/* ---- functions defined in this file ---- */
int func_001CD5A8();
void func_001CD780();
void func_001CD7C0();
void func_001CD90C();
void func_001CD9FC();
void func_001CDA20();
void func_001CDA28();
int func_001CDA48();
void func_001CDA5C();
void func_001CDA64();
int func_001CDA84();
void func_001CDAC4();
void func_001CDACC();
int func_001CDAEC();
void func_001CDB18();
void func_001CDB28();
int func_001CDB50();
void func_001CDB58();
void func_001CDB68();
int func_001CDB90();
void func_001CDB98();
void func_001CDBD0();
void func_001CDC0C();
void func_001CDC44();

/* ---------------- func_001CD5A8 (src/pilot/frontend/func_001CD5A8.c) ---------------- */
int func_001CD5A8(void)
{
    func_001C9A18(8);
    D_001D32F8 = D_00096C94;
    D_001D32FC = D_00096C90;
    D_001D3300 = D_000967EC;
    D_001D3304 = D_00096784;
    func_001CD780();
    D_001CF878 = 0;
    D_001CF850 = 0;
    D_001CDECC = 1;
    do {
        func_000514E0();
        VSync(0);
        if (D_001CEF88 == 0) {
            D_001CF850 = func_001CB790(D_001CF7C0, D_001CF850);
            func_001CD90C();
        }
        func_001CB380();
        if (D_001CEF88 == 0) {
            func_001CA298();
            func_001CB95C(D_001CF710);
        }
        DrawSync(0);
        func_000513D4();
    } while (D_001CF850 != -1 && D_001CF878 == 0);
    if (D_001CF878 == 0) {
        D_00096C94 = D_001D32F8;
        D_00096C90 = D_001D32FC;
        D_000967EC = D_001D3300;
        D_00096784 = D_001D3304;
        func_0004E454(D_00096C90);
        SetMasterVolume(D_00096C94);
    } else {
        D_001CEF54 = D_00096C90;
    }
    func_001CD9FC(9);
    SetTextColour(0, 0, 0);
    func_001CA270(255, 255, 255, 0x40);
    func_001C9A18(3);
    return -1;
}

/* ---------------- func_001CD780 (src/pilot/frontend/func_001CD780.c) ---------------- */
void func_001CD780(void){ func_001CD9FC(1); func_001CD9FC(3); func_001CD9FC(5); func_001CD9FC(7); func_001CD7C0(); }

/* ---------------- func_001CD7C0 (WIP: still asm; the C draft is src/pilot/frontend/func_001CD7C0.wip.c) ---------------- */
INCLUDE_ASM("asm_fn/nonmatchings/frontend", func_001CD7C0);

/* ---------------- func_001CD90C (src/pilot/frontend/func_001CD90C.c) ---------------- */
void func_001CD90C(void)
{
    if (D_001CF880 == 0) {
        D_001CF87C += 16;
        if (D_001CF87C >= 256) {
            D_001CF87C = 255;
            D_001CF880 = 1;
        }
    } else {
        D_001CF87C -= 16;
        if (D_001CF87C < 0) {
            D_001CF87C = 0;
            D_001CF880 = 0;
        }
    }
    if (D_001CF850 != -1) {
        D_001CF710[D_001CF864[D_001CF850]].col.r = D_001CF87C;
        D_001CF710[D_001CF864[D_001CF850]].col.g = D_001CF87C;
        D_001CF710[D_001CF864[D_001CF850]].col.b = D_001CF87C;
    }
}

/* ---------------- func_001CD9FC (src/pilot/frontend/func_001CD9FC.c) ---------------- */
void func_001CD9FC(int i)
{
    D_001CF710[i].col.r = 255;
    D_001CF710[i].col.g = 255;
    D_001CF710[i].col.b = 255;
}

/* ---------------- func_001CDA20 (src/pilot/frontend/func_001CDA20.c) ---------------- */
void func_001CDA20(void){}

/* ---------------- func_001CDA28 (src/pilot/frontend/func_001CDA28.c) ---------------- */
void func_001CDA28(void)
{
    MENU_TEXT *p = D_001CF710;
    p += 9;
    p->col.r = 255;
    p->col.g = 255;
    p->col.b = 255;
}

/* ---------------- func_001CDA48 (src/pilot/frontend/func_001CDA48.c) ---------------- */
int func_001CDA48(void) {
    D_001CF878 = 1;
    return -2;
}

/* ---------------- func_001CDA5C (src/pilot/frontend/func_001CDA5C.c) ---------------- */
void func_001CDA5C(void){}

/* ---------------- func_001CDA64 (src/pilot/frontend/func_001CDA64.c) ---------------- */
void func_001CDA64(void)
{
    MENU_TEXT *p = D_001CF710;
    p += 7;
    p->col.r = 255;
    p->col.g = 255;
    p->col.b = 255;
}

/* ---------------- func_001CDA84 (src/pilot/frontend/func_001CDA84.c) ---------------- */
int func_001CDA84(void){ if (D_000A801D & 4) { D_000967EC ^= 1; func_001CD7C0(); } return 1; }

/* ---------------- func_001CDAC4 (src/pilot/frontend/func_001CDAC4.c) ---------------- */
void func_001CDAC4(void){}

/* ---------------- func_001CDACC (src/pilot/frontend/func_001CDACC.c) ---------------- */
void func_001CDACC(void)
{
    MENU_TEXT *p = D_001CF710;
    p += 5;
    p->col.r = 255;
    p->col.g = 255;
    p->col.b = 255;
}

/* ---------------- func_001CDAEC (src/pilot/frontend/func_001CDAEC.c) ---------------- */
int func_001CDAEC(void){ D_00096784 ^= 1; func_001CD7C0(); return 1; }

/* ---------------- func_001CDB18 (src/pilot/frontend/func_001CDB18.c) ---------------- */
void func_001CDB18(void) {
    D_001CDECC = 1;
}

/* ---------------- func_001CDB28 (src/pilot/frontend/func_001CDB28.c) ---------------- */
void func_001CDB28(void)
{
    MENU_TEXT *p = D_001CF710;
    p += 3;
    p->col.r = 255;
    p->col.g = 255;
    p->col.b = 255;
    D_001CDECC = 0;
}

/* ---------------- func_001CDB50 (src/pilot/frontend/func_001CDB50.c) ---------------- */
int func_001CDB50(void){return 1;}

/* ---------------- func_001CDB58 (src/pilot/frontend/func_001CDB58.c) ---------------- */
void func_001CDB58(void) {
    D_001CDECC = 1;
}

/* ---------------- func_001CDB68 (src/pilot/frontend/func_001CDB68.c) ---------------- */
void func_001CDB68(void)
{
    MENU_TEXT *p = D_001CF710;
    p += 1;
    p->col.r = 255;
    p->col.g = 255;
    p->col.b = 255;
    D_001CDECC = 0;
}

/* ---------------- func_001CDB90 (src/pilot/frontend/func_001CDB90.c) ---------------- */
int func_001CDB90(void){return 0;}

/* ---------------- func_001CDB98 (src/pilot/frontend/func_001CDB98.c) ---------------- */
void func_001CDB98(void){ if (D_00096C94 < 0) { D_00096C94 += 100; SetMasterVolume(D_00096C94); func_001CD7C0(); } }

/* ---------------- func_001CDBD0 (src/pilot/frontend/func_001CDBD0.c) ---------------- */
void func_001CDBD0(void){ if (D_00096C94 >= -9999) { D_00096C94 -= 100; SetMasterVolume(D_00096C94); func_001CD7C0(); } }

/* ---------------- func_001CDC0C (src/pilot/frontend/func_001CDC0C.c) ---------------- */
void func_001CDC0C(void){ if (D_00096C90 < 0) { D_00096C90 += 100; func_0004E454(D_00096C90); func_001CD7C0(); } }

/* ---------------- func_001CDC44 (src/pilot/frontend/func_001CDC44.c) ---------------- */
void func_001CDC44(void){ if (D_00096C90 >= -9999) { D_00096C90 -= 100; func_0004E454(D_00096C90); func_001CD7C0(); } }
