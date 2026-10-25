/* Boundary evidence (pilot/TU1.md, FRONTEND section):
 * menu loop func_001CCD58 + table D_001CF3F8, .data 0x1CF368-0x1CF460. END: no call/data edge crosses 0x1CD0F8
 * (span 60). Medium.
 */
/* ladmenu.c: 16 functions func_001CCD58..func_001CD0B8 (0x1CCD58-0x1CD0F8)
 * Step 3: types from include/frontend.h: D_001CF368 is a MENU_TEXT table (int-index text stores and
 * colour records were views of it), D_000BF7EC the language string table (char *[]). No views.
 */

/* ---- types ---- */
#include "frontend.h"

/* ---- externs ---- */
extern int D_001CF438;
extern char D_001CF3F8[];
extern MENU_TEXT D_001CF368[];
extern void func_000514E0();
extern int VSync();
extern int func_001CB790();
extern void func_001CB380();
extern void func_001CA298();
extern void func_001CB95C();
extern int DrawSync();
extern void func_000513D4();
extern int D_001CF444;
extern char *D_000BF7EC[];
extern int D_001CF43C[];
extern int D_001CF448;
extern int D_001CF460;
extern int D_001CF45C;
extern int D_001CF44C[];

/* ---- functions defined in this file ---- */
void func_001CCD58();
void func_001CCDF4();
void func_001CCE18();
void func_001CCEDC();
void func_001CCFCC();
void func_001CCFD4();
int func_001CCFF4();
void func_001CD018();
void func_001CD020();
int func_001CD040();
void func_001CD048();
void func_001CD050();
int func_001CD070();
void func_001CD090();
void func_001CD098();
int func_001CD0B8();

/* ---------------- func_001CCD58 (src/pilot/frontend/func_001CCD58.c) ---------------- */
void func_001CCD58(void)
{
    func_001CCDF4();
    D_001CF438 = 0;
    do {
        func_000514E0();
        VSync(0);
        D_001CF438 = func_001CB790(D_001CF3F8, D_001CF438);
        func_001CCEDC();
        func_001CB380();
        func_001CA298();
        func_001CB95C(D_001CF368);
        DrawSync(0);
        func_000513D4();
    } while (D_001CF438 != -1);
}

/* ---------------- func_001CCDF4 (src/pilot/frontend/func_001CCDF4.c) ---------------- */
void func_001CCDF4(void)
{
    D_001CF444 = 0;
    func_001CCE18();
}

/* ---------------- func_001CCE18 (src/pilot/frontend/func_001CCE18.c) ---------------- */
void func_001CCE18(void)
{
    if (D_001CF444 == 0) {
        D_001CF368[2].text = D_000BF7EC[114];
        D_001CF368[7].text = D_000BF7EC[116];
    } else {
        D_001CF368[2].text = D_000BF7EC[115];
        D_001CF368[7].text = D_000BF7EC[D_001CF43C[D_001CF448]];
    }
    D_001CF368[0].text = D_000BF7EC[108];
    D_001CF368[1].text = D_000BF7EC[109];
    D_001CF368[3].text = D_000BF7EC[110];
    D_001CF368[4].text = D_000BF7EC[111];
    D_001CF368[5].text = D_000BF7EC[112];
    D_001CF368[6].text = D_000BF7EC[113];
}

/* ---------------- func_001CCEDC (src/pilot/frontend/func_001CCEDC.c) ---------------- */
void func_001CCEDC(void)
{
    if (D_001CF460 == 0) {
        D_001CF45C += 16;
        if (D_001CF45C >= 256) {
            D_001CF45C = 255;
            D_001CF460 = 1;
        }
    } else {
        D_001CF45C -= 16;
        if (D_001CF45C < 0) {
            D_001CF45C = 0;
            D_001CF460 = 0;
        }
    }
    if (D_001CF438 != -1) {
        D_001CF368[D_001CF44C[D_001CF438]].col.r = D_001CF45C;
        D_001CF368[D_001CF44C[D_001CF438]].col.g = D_001CF45C;
        D_001CF368[D_001CF44C[D_001CF438]].col.b = D_001CF45C;
    }
}

/* ---------------- func_001CCFCC (src/pilot/frontend/func_001CCFCC.c) ---------------- */
void func_001CCFCC(void){}

/* ---------------- func_001CCFD4 (src/pilot/frontend/func_001CCFD4.c) ---------------- */
void func_001CCFD4(void)
{
    MENU_TEXT *p = D_001CF368;
    p += 1;
    p->col.r = 255;
    p->col.g = 255;
    p->col.b = 255;
}

/* ---------------- func_001CCFF4 (src/pilot/frontend/func_001CCFF4.c) ---------------- */
int func_001CCFF4(void)
{
    D_001CF444 = 0;
    func_001CCE18();
    return 0;
}

/* ---------------- func_001CD018 (src/pilot/frontend/func_001CD018.c) ---------------- */
void func_001CD018(void){}

/* ---------------- func_001CD020 (src/pilot/frontend/func_001CD020.c) ---------------- */
void func_001CD020(void)
{
    MENU_TEXT *p = D_001CF368;
    p += 4;
    p->col.r = 255;
    p->col.g = 255;
    p->col.b = 255;
}

/* ---------------- func_001CD040 (src/pilot/frontend/func_001CD040.c) ---------------- */
int func_001CD040(void){return 0;}

/* ---------------- func_001CD048 (src/pilot/frontend/func_001CD048.c) ---------------- */
void func_001CD048(void){}

/* ---------------- func_001CD050 (src/pilot/frontend/func_001CD050.c) ---------------- */
void func_001CD050(void)
{
    MENU_TEXT *p = D_001CF368;
    p += 5;
    p->col.r = 255;
    p->col.g = 255;
    p->col.b = 255;
}

/* ---------------- func_001CD070 (src/pilot/frontend/func_001CD070.c) ---------------- */
int func_001CD070(void)
{
    func_001CCE18();
    return 0;
}

/* ---------------- func_001CD090 (src/pilot/frontend/func_001CD090.c) ---------------- */
void func_001CD090(void){}

/* ---------------- func_001CD098 (src/pilot/frontend/func_001CD098.c) ---------------- */
void func_001CD098(void)
{
    MENU_TEXT *p = D_001CF368;
    p += 6;
    p->col.r = 255;
    p->col.g = 255;
    p->col.b = 255;
}

/* ---------------- func_001CD0B8 (src/pilot/frontend/func_001CD0B8.c) ---------------- */
int func_001CD0B8(void){ if (D_001CF444 != 0) D_001CF448 ^= 1; func_001CCE18(); return 0; }
