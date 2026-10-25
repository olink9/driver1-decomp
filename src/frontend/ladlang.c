/* Boundary evidence (pilot/TU1.md, FRONTEND section):
 * menu built from main's language strings D_000BF7EC[92..] (func_001CD0F8) + 37 toggle handlers in table
 * D_001CF468. END: text pad at 0x1CD5A4.
 */
/* ladlang.c: 38 functions func_001CD0F8..func_001CD59C (0x1CD0F8-0x1CD5A8)
 * Step 3: types from include/frontend.h: D_001CF468 is a MENU_TEXT table (the colour-only records were views
 * of it). No views.
 */

/* ---- types ---- */
#include "frontend.h"

/* ---- externs ---- */
extern MENU_TEXT D_001CF468[];
extern char *D_000BF7EC[];
extern int D_001CF6C0;
extern int D_001CF6BC;
extern int D_001CF6C8[];
extern int D_00096ABC;
extern int D_00096AC0;
extern int D_00096A88;
extern int D_001CDECC;

/* ---- functions defined in this file ---- */
void func_001CD0F8();
void func_001CD228();
void func_001CD230();
int func_001CD250();
void func_001CD270();
void func_001CD278();
int func_001CD298();
void func_001CD2C4();
void func_001CD2CC();
int func_001CD2EC();
void func_001CD318();
void func_001CD320();
int func_001CD340();
void func_001CD360();
void func_001CD368();
int func_001CD388();
void func_001CD3D8();
void func_001CD3E0();
int func_001CD400();
void func_001CD42C();
void func_001CD434();
int func_001CD454();
void func_001CD474();
void func_001CD47C();
int func_001CD49C();
void func_001CD4BC();
void func_001CD4C4();
int func_001CD4E4();
void func_001CD504();
void func_001CD514();
int func_001CD53C();
void func_001CD544();
void func_001CD554();
int func_001CD57C();
void func_001CD584();
void func_001CD58C();
void func_001CD594();
void func_001CD59C();

/* ---------------- func_001CD0F8 (src/pilot/frontend/func_001CD0F8.c) ---------------- */
void func_001CD0F8(void)
{
    D_001CF468[0].text = D_000BF7EC[92];
    D_001CF468[1].text = D_000BF7EC[93];
    D_001CF468[2].text = D_000BF7EC[94];
    D_001CF468[3].text = D_000BF7EC[95];
    D_001CF468[4].text = D_000BF7EC[96];
    D_001CF468[5].text = D_000BF7EC[97];
    D_001CF468[6].text = D_000BF7EC[98];
    D_001CF468[7].text = D_000BF7EC[99];
    D_001CF468[8].text = D_000BF7EC[100];
    D_001CF468[9].text = D_000BF7EC[101];
    D_001CF468[10].text = D_000BF7EC[102];
    D_001CF468[11].text = D_000BF7EC[103];
    D_001CF468[12].text = D_000BF7EC[D_001CF6C0];
    D_001CF468[13].text = D_000BF7EC[D_001CF6C0];
    D_001CF468[14].text = D_000BF7EC[D_001CF6C0];
    D_001CF468[15].text = D_000BF7EC[D_001CF6C0];
    D_001CF468[16].text = D_000BF7EC[D_001CF6C8[D_001CF6BC]];
    D_001CF468[17].text = D_000BF7EC[D_001CF6C0];
    D_001CF468[18].text = D_000BF7EC[D_001CF6C0];
    D_001CF468[19].text = D_000BF7EC[D_001CF6C0];
    D_001CF468[20].text = D_000BF7EC[D_001CF6C0];
}

/* ---------------- func_001CD228 (src/pilot/frontend/func_001CD228.c) ---------------- */
void func_001CD228(void){}

/* ---------------- func_001CD230 (src/pilot/frontend/func_001CD230.c) ---------------- */
void func_001CD230(void)
{
    MENU_TEXT *p = D_001CF468;
    p += 1;
    p->col.r = 255;
    p->col.g = 255;
    p->col.b = 255;
}

/* ---------------- func_001CD250 (src/pilot/frontend/func_001CD250.c) ---------------- */
int func_001CD250(void)
{
    func_001CD0F8();
    return 0;
}

/* ---------------- func_001CD270 (src/pilot/frontend/func_001CD270.c) ---------------- */
void func_001CD270(void){}

/* ---------------- func_001CD278 (src/pilot/frontend/func_001CD278.c) ---------------- */
void func_001CD278(void)
{
    MENU_TEXT *p = D_001CF468;
    p += 2;
    p->col.r = 255;
    p->col.g = 255;
    p->col.b = 255;
}

/* ---------------- func_001CD298 (src/pilot/frontend/func_001CD298.c) ---------------- */
int func_001CD298(void){ D_00096ABC ^= 1; func_001CD0F8(); return 0; }

/* ---------------- func_001CD2C4 (src/pilot/frontend/func_001CD2C4.c) ---------------- */
void func_001CD2C4(void){}

/* ---------------- func_001CD2CC (src/pilot/frontend/func_001CD2CC.c) ---------------- */
void func_001CD2CC(void)
{
    MENU_TEXT *p = D_001CF468;
    p += 3;
    p->col.r = 255;
    p->col.g = 255;
    p->col.b = 255;
}

/* ---------------- func_001CD2EC (src/pilot/frontend/func_001CD2EC.c) ---------------- */
int func_001CD2EC(void){ D_00096AC0 ^= 1; func_001CD0F8(); return 0; }

/* ---------------- func_001CD318 (src/pilot/frontend/func_001CD318.c) ---------------- */
void func_001CD318(void){}

/* ---------------- func_001CD320 (src/pilot/frontend/func_001CD320.c) ---------------- */
void func_001CD320(void)
{
    MENU_TEXT *p = D_001CF468;
    p += 4;
    p->col.r = 255;
    p->col.g = 255;
    p->col.b = 255;
}

/* ---------------- func_001CD340 (src/pilot/frontend/func_001CD340.c) ---------------- */
int func_001CD340(void)
{
    func_001CD0F8();
    return 0;
}

/* ---------------- func_001CD360 (src/pilot/frontend/func_001CD360.c) ---------------- */
void func_001CD360(void){}

/* ---------------- func_001CD368 (src/pilot/frontend/func_001CD368.c) ---------------- */
void func_001CD368(void)
{
    MENU_TEXT *p = D_001CF468;
    p += 5;
    p->col.r = 255;
    p->col.g = 255;
    p->col.b = 255;
}

/* ---------------- func_001CD388 (src/pilot/frontend/func_001CD388.c) ---------------- */
int func_001CD388(void){ D_001CF6BC = (D_001CF6BC + 1) % 3; func_001CD0F8(); return 0; }

/* ---------------- func_001CD3D8 (src/pilot/frontend/func_001CD3D8.c) ---------------- */
void func_001CD3D8(void){}

/* ---------------- func_001CD3E0 (src/pilot/frontend/func_001CD3E0.c) ---------------- */
void func_001CD3E0(void)
{
    MENU_TEXT *p = D_001CF468;
    p += 6;
    p->col.r = 255;
    p->col.g = 255;
    p->col.b = 255;
}

/* ---------------- func_001CD400 (src/pilot/frontend/func_001CD400.c) ---------------- */
int func_001CD400(void){ D_00096A88 ^= 1; func_001CD0F8(); return 0; }

/* ---------------- func_001CD42C (src/pilot/frontend/func_001CD42C.c) ---------------- */
void func_001CD42C(void){}

/* ---------------- func_001CD434 (src/pilot/frontend/func_001CD434.c) ---------------- */
void func_001CD434(void)
{
    MENU_TEXT *p = D_001CF468;
    p += 7;
    p->col.r = 255;
    p->col.g = 255;
    p->col.b = 255;
}

/* ---------------- func_001CD454 (src/pilot/frontend/func_001CD454.c) ---------------- */
int func_001CD454(void)
{
    func_001CD0F8();
    return 0;
}

/* ---------------- func_001CD474 (src/pilot/frontend/func_001CD474.c) ---------------- */
void func_001CD474(void){}

/* ---------------- func_001CD47C (src/pilot/frontend/func_001CD47C.c) ---------------- */
void func_001CD47C(void)
{
    MENU_TEXT *p = D_001CF468;
    p += 8;
    p->col.r = 255;
    p->col.g = 255;
    p->col.b = 255;
}

/* ---------------- func_001CD49C (src/pilot/frontend/func_001CD49C.c) ---------------- */
int func_001CD49C(void)
{
    func_001CD0F8();
    return 0;
}

/* ---------------- func_001CD4BC (src/pilot/frontend/func_001CD4BC.c) ---------------- */
void func_001CD4BC(void){}

/* ---------------- func_001CD4C4 (src/pilot/frontend/func_001CD4C4.c) ---------------- */
void func_001CD4C4(void)
{
    MENU_TEXT *p = D_001CF468;
    p += 9;
    p->col.r = 255;
    p->col.g = 255;
    p->col.b = 255;
}

/* ---------------- func_001CD4E4 (src/pilot/frontend/func_001CD4E4.c) ---------------- */
int func_001CD4E4(void)
{
    func_001CD0F8();
    return 0;
}

/* ---------------- func_001CD504 (src/pilot/frontend/func_001CD504.c) ---------------- */
void func_001CD504(void) {
    D_001CDECC = 1;
}

/* ---------------- func_001CD514 (src/pilot/frontend/func_001CD514.c) ---------------- */
/* q is a single-set local so its p+10 gets sched1's birthing boost and stays after the store
   (a twice-set p += 10 has no boost and loses the tie to the store). */
void func_001CD514(void)
{
    MENU_TEXT *p = D_001CF468;
    MENU_TEXT *q;
    D_001CDECC = 0;
    q = p + 10;
    q->col.r = 255;
    q->col.g = 255;
    q->col.b = 255;
}

/* ---------------- func_001CD53C (src/pilot/frontend/func_001CD53C.c) ---------------- */
int func_001CD53C(void){return 0;}

/* ---------------- func_001CD544 (src/pilot/frontend/func_001CD544.c) ---------------- */
void func_001CD544(void) {
    D_001CDECC = 1;
}

/* ---------------- func_001CD554 (src/pilot/frontend/func_001CD554.c) ---------------- */
/* q is a single-set local so its p+11 gets sched1's birthing boost and stays after the store
   (a twice-set p += 11 has no boost and loses the tie to the store). */
void func_001CD554(void)
{
    MENU_TEXT *p = D_001CF468;
    MENU_TEXT *q;
    D_001CDECC = 0;
    q = p + 11;
    q->col.r = 255;
    q->col.g = 255;
    q->col.b = 255;
}

/* ---------------- func_001CD57C (src/pilot/frontend/func_001CD57C.c) ---------------- */
int func_001CD57C(void){return 0;}

/* ---------------- func_001CD584 (src/pilot/frontend/func_001CD584.c) ---------------- */
void func_001CD584(void){}

/* ---------------- func_001CD58C (src/pilot/frontend/func_001CD58C.c) ---------------- */
void func_001CD58C(void){}

/* ---------------- func_001CD594 (src/pilot/frontend/func_001CD594.c) ---------------- */
void func_001CD594(void){}

/* ---------------- func_001CD59C (src/pilot/frontend/func_001CD59C.c) ---------------- */
void func_001CD59C(void){}
