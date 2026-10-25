/* Boundary evidence (pilot/TU1.md, FRONTEND section):
 * "Sound:" options (Effects/Music volume) func_001C6748 + table D_001CE95C. START: .rodata pad 0x1C0694. END: text
 * pad at 0x1C6DDC.
 */
/* soundopts.c: 15 functions func_001C6748..func_001C6D78 (0x1C6748-0x1C6DE0)
 * Step 3: common.h (libgpu setXYWH) + include/frontend.h (MENU_TEXT D_001CE90C, MENU_ITEM D_001CE95C); no views
 * (D_001CE9E0 was a char/unsigned char view: the same type under -funsigned-char).
 */

#include "common.h"

/* ---- types ---- */
#include "frontend.h"

/* ---- externs ---- */
extern char D_001CE9E0;
extern int D_001CE9D4;
extern int D_00096C90;
extern int D_00096C94;
extern int D_001D2E38;
extern int D_001D2E3C;
extern MENU_ITEM D_001CE95C[];
extern char D_001CE8A8[];
extern MENU_TEXT D_001CE90C[];
extern void func_001C1C4C();
extern void func_001C2644();
extern void func_001C2114();
extern void func_000514E0();
extern int func_001C26E4();
extern void func_001C19C4();
extern void func_001C1F40();
extern void func_001C1BD4();
extern void func_001C11E4();
extern void func_0006F478();
extern void SetMasterVolume();
extern char D_001CE940;
extern int D_001CDECC;
extern char D_001CE930;
extern void func_0004E430();
extern void StartSound();
extern int D_001CFDE8;
extern void func_0004E118();
extern char D_001CE920;
extern int D_00096C98;
extern int D_00096C9C;
extern POLY_FT4 D_001D2C70[2];
extern POLY_FT4 D_001D2CC0[2];
extern void func_0004DB84();

/* ---- functions defined in this file ---- */
int func_001C6748();
void func_001C6894();
void func_001C68A4();
int func_001C68B4();
void func_001C68BC();
void func_001C6910();
int func_001C694C();
void func_001C6954();
void func_001C6970();
int func_001C6988();
void func_001C6990();
void func_001C6C3C();
void func_001C6CA8();
void func_001C6D20();
void func_001C6D78();

/* ---------------- func_001C6748 (src/pilot/frontend/func_001C6748.c) ---------------- */
int func_001C6748(void)
{
    int r;
    D_001CE9E0 = 1;
    func_001C1C4C();
    func_001C6990();
    D_001CE9E0 = 0;
    D_001CE9D4 = 0;
    func_001C2644(D_001CE95C, 3, 0);
    func_001C2114(D_001CE8A8, D_001CE90C, 1);
    D_001D2E38 = D_00096C90;
    D_001D2E3C = D_00096C94;
    for (;;) {
        func_000514E0();
        r = func_001C26E4(D_001CE95C, &D_001CE9D4);
        if (r != 0) break;
        func_001C19C4();
        func_001C1F40(2);
        func_001C1BD4(D_001CE90C);
        func_001C11E4();
    }
    func_001C2114(D_001CE8A8, D_001CE90C, 0);
    if (r != -1) return 0;
    D_00096C90 = D_001D2E38;
    D_00096C94 = D_001D2E3C;
    func_0006F478(D_00096C90);
    SetMasterVolume(D_00096C94);
    return 0;
}

/* ---------------- func_001C6894 (src/pilot/frontend/func_001C6894.c) ---------------- */
void func_001C6894(void) {
    D_001CE940 = 3;
}

/* ---------------- func_001C68A4 (src/pilot/frontend/func_001C68A4.c) ---------------- */
void func_001C68A4(void) {
    D_001CE940 = 2;
}

/* ---------------- func_001C68B4 (src/pilot/frontend/func_001C68B4.c) ---------------- */
int func_001C68B4(void){return 1;}

/* ---------------- func_001C68BC (src/pilot/frontend/func_001C68BC.c) ---------------- */
void func_001C68BC(void){ func_0004E430(); StartSound(0, 0, 1, D_00096C94, 0x1000); D_001CE930 = 3; D_001CDECC = 1; }

/* ---------------- func_001C6910 (src/pilot/frontend/func_001C6910.c) ---------------- */
void func_001C6910(void){ if (D_001CFDE8 == 0) func_0004E118(0); D_001CE930 = 2; D_001CDECC = 0; }

/* ---------------- func_001C694C (src/pilot/frontend/func_001C694C.c) ---------------- */
int func_001C694C(void){return 0;}

/* ---------------- func_001C6954 (src/pilot/frontend/func_001C6954.c) ---------------- */
void func_001C6954(void)
{
    D_001CE920 = 3;
    D_001CDECC = 1;
}

/* ---------------- func_001C6970 (src/pilot/frontend/func_001C6970.c) ---------------- */
void func_001C6970(void)
{
    D_001CE920 = 2;
    D_001CDECC = 0;
}

/* ---------------- func_001C6988 (src/pilot/frontend/func_001C6988.c) ---------------- */
int func_001C6988(void){return 0;}

/* ---------------- func_001C6990 (src/pilot/frontend/func_001C6990.c) ---------------- */
/* func_001C6990 - position the two volume bars (frame + fill scaled by the volume). The fill width source
   `s = u1 - u0` is its own statement after the frame's setXYWH (inline in the scale expression the loads interleave
   u1,u0,u1,u0 and the regs swap). */
void func_001C6990(void)
{
    int w, s;

    if (D_00096C9C != 0 || D_001CE9E0 != 0) {
        setXYWH(&D_001D2C70[0], 0xE2, 0xE1, D_001D2C70[0].u1 - D_001D2C70[0].u0, D_001D2C70[0].v2 - D_001D2C70[0].v0);
 s = D_001D2C70[0].u1 - D_001D2C70[0].u0;
 w = ((D_00096C94 + 10000) * ((s << 12) / 10000)) >> 12;
 setXYWH(&D_001D2CC0[0], w + 0xE3, 0xE1, D_001D2CC0[0].u1 - D_001D2CC0[0].u0, D_001D2CC0[0].v2 - D_001D2CC0[0].v0);
    }
    if (D_00096C98 != 0 || D_001CE9E0 != 0) {
        setXYWH(&D_001D2C70[1], 0xE2, 0xC8, D_001D2C70[1].u1 - D_001D2C70[1].u0, D_001D2C70[1].v2 - D_001D2C70[1].v0);
 s = D_001D2C70[1].u1 - D_001D2C70[1].u0;
 w = ((D_00096C90 + 10000) * ((s << 12) / 10000)) >> 12;
 setXYWH(&D_001D2CC0[1], w + 0xE3, 0xC8, D_001D2CC0[1].u1 - D_001D2CC0[1].u0, D_001D2CC0[1].v2 - D_001D2CC0[1].v0);
    }
}

/* ---------------- func_001C6C3C (src/pilot/frontend/func_001C6C3C.c) ---------------- */
void func_001C6C3C(void)
{
    if (D_00096C9C != 0) {
        if (D_00096C94 < 0) {
            D_00096C94 += 100;
            if (D_00096C94 > 0)
                D_00096C94 = 0;
            SetMasterVolume(D_00096C94);
            func_0004DB84(0, D_00096C94);
            func_001C6990();
        }
    }
}

/* ---------------- func_001C6CA8 (src/pilot/frontend/func_001C6CA8.c) ---------------- */
void func_001C6CA8(void)
{
    if (D_00096C9C != 0) {
        if (D_00096C94 >= -9999) {
            D_00096C94 -= 100;
            if (D_00096C94 < -10000)
                D_00096C94 = -10000;
            SetMasterVolume(D_00096C94);
            func_0004DB84(0, D_00096C94);
            func_001C6990();
        }
    }
}

/* ---------------- func_001C6D20 (src/pilot/frontend/func_001C6D20.c) ---------------- */
void func_001C6D20(void){ if (D_00096C98 != 0 && D_00096C90 < 0) { D_00096C90 += 100; if (D_00096C90 > 0) D_00096C90 = 0; func_0006F478(D_00096C90); func_001C6990(); } }

/* ---------------- func_001C6D78 (src/pilot/frontend/func_001C6D78.c) ---------------- */
void func_001C6D78(void)
{
    if (D_00096C98 != 0) {
        if (D_00096C90 >= -9999) {
            D_00096C90 -= 100;
            if (D_00096C90 < -10000)
                D_00096C90 = -10000;
            func_0006F478(D_00096C90);
            func_001C6990();
        }
    }
}
