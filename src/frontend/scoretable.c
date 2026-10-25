/* Boundary evidence (pilot/TU1.md, FRONTEND section):
 * score-table display func_001C7730 / func_001C7900 (Best Lap, %d flags) + table D_001CEC38. START: text pad. END:
 * .rodata pad 0x1C087C; "%d" duplicated in ladoptions.c. .rodata literals one block 0x1C084C.
 */
/* scoretable.c: 8 functions func_001C7730..func_001C7900 (0x1C7730-0x1C7CA8)
 * Step 3: types from include/frontend.h (MENU_TEXT D_001CEA88, MENU_ITEM D_001CEC38); REC (a time-table row) is
 * file-local. No views.
 */

/* ---- types ---- */
#include "frontend.h"
typedef struct { char name[8]; unsigned int time; } REC;

/* ---- externs ---- */
extern int D_001CDE5C;
extern int D_001CEC5C;
extern int D_001CEC58;
extern int D_001CDC80;
extern char D_001CEA60[];
extern MENU_TEXT D_001CEA88[];
extern MENU_ITEM D_001CEC38[];
extern void func_001C2114();
extern void func_000514E0();
extern int func_001C26E4();
extern void func_001C19C4();
extern void func_001C1BD4();
extern void func_001C11E4();
extern int D_001CDED0;
extern int D_001D2F28;
extern int D_001CECB8;
extern char *D_001CE09C[];
extern char *D_001CEC60[];
extern char D_001C07C0[];
extern char D_001C0870[];
extern char D_001D2E68[];
extern char D_001D3038[][32];
extern char D_001CEC80[][9];
extern REC *D_001D2E58;
extern int sprintf();
extern char *func_0006E418();
extern unsigned char func_0006ACC8(char *buf, int ticks, unsigned char flag);

/* ---- functions defined in this file ---- */
int func_001C7730();
void func_001C786C();
void func_001C78A0();
int func_001C78A8();
void func_001C78B0();
void func_001C78F0();
int func_001C78F8();
void func_001C7900();

/* ---------------- func_001C7730 (src/pilot/frontend/func_001C7730.c) ---------------- */
int func_001C7730(void)
{
    int r;
    int prev;
    D_001CDE5C = 5;
    D_001CEC5C = 0;
    func_001C7900();
    D_001CEC58 = 0;
    func_001C2114(D_001CEA60, D_001CEA88, 1);
    for (;;) {
        prev = D_001CEC5C;
        func_000514E0();
        r = func_001C26E4(D_001CEC38, &D_001CEC58);
        if (r != 0) break;
        if (prev != D_001CEC5C)
            func_001C7900();
        if (D_001CDC80 != 0) {
            func_001C19C4();
            func_001C1BD4(D_001CEA88);
            func_001C11E4();
            func_001C19C4();
            func_001C1BD4(D_001CEA88);
            func_001C11E4();
        }
    }
    D_001CDE5C = 6;
    func_001C2114(D_001CEA60, D_001CEA88, 0);
    if (r == -1) return 0;
    return r;
}

/* ---------------- func_001C786C (src/pilot/frontend/func_001C786C.c) ---------------- */
void func_001C786C(void){ D_001CEC5C = D_001CEC5C - 1; if (D_001CEC5C < 0) D_001CEC5C = 0; else D_001CDED0 = 1; }

/* ---------------- func_001C78A0 (src/pilot/frontend/func_001C78A0.c) ---------------- */
void func_001C78A0(void){}

/* ---------------- func_001C78A8 (src/pilot/frontend/func_001C78A8.c) ---------------- */
int func_001C78A8(void){return 0;}

/* ---------------- func_001C78B0 (src/pilot/frontend/func_001C78B0.c) ---------------- */
void func_001C78B0(void){ D_001CEC5C = D_001CEC5C + 1; if (D_001CEC5C >= D_001D2F28) D_001CEC5C = D_001D2F28 - 1; else D_001CDED0 = 1; }

/* ---------------- func_001C78F0 (src/pilot/frontend/func_001C78F0.c) ---------------- */
void func_001C78F0(void){}

/* ---------------- func_001C78F8 (src/pilot/frontend/func_001C78F8.c) ---------------- */
int func_001C78F8(void){return 0;}

/* ---------------- func_001C7900 (src/pilot/frontend/func_001C7900.c) ---------------- */
void func_001C7900(void)
{
    char buf[256];
    unsigned int k;
    int i;

    switch (D_001CECB8) {
    case 0x11:
        k = 0;
        D_001D2F28 = 8;
        break;
    case 0x12:
        k = 1;
        D_001D2F28 = 8;
        break;
    case 0x13:
        k = 2;
        D_001D2F28 = 4;
        break;
    case 0x16:
        k = 3;
        D_001D2F28 = 4;
        break;
    default:
        return;
    }
    if (k < 2)
        sprintf(buf, "%s %d", D_001CE09C[D_001CEC5C >> 1], (D_001CEC5C & 1) + 1);
    else if (k == 2)
        sprintf(buf, "%s", D_001CE09C[D_001CEC5C]);
    else
        sprintf(buf, "%d", D_001CEC5C + 1);
    sprintf(D_001D2E68, "%s %s: ", D_001CEC60[k], buf);
    D_001CEA88[0].text = D_001D2E68;
    {
        REC *r = (REC *)func_0006E418(D_001CECB8, D_001CEC5C);
        D_001D2E58 = r;
    }
    for (i = 0; i < 5; i++) {
        if (D_001CECB8 == 0x11) {
            sprintf(D_001D3038[i], "%d flags", D_001D2E58[i].time >> 25);
            func_0006ACC8(D_001CEC80[i], D_001D2E58[i].time & 0x1FFFFFF, 1);
        } else {
            func_0006ACC8(D_001CEC80[i], D_001D2E58[i].time, 1);
            sprintf(D_001D3038[i], D_001C07C0);
        }
    }
    for (i = 0; i < 5; i++) {
        D_001CEA88[i * 4 + 2].text = D_001D2E58[i].name;
        D_001CEA88[i * 4 + 3].text = D_001CEC80[i];
        D_001CEA88[i * 4 + 4].text = D_001D3038[i];
    }
    if (D_001CECB8 == 0x16) {
        k = i * 4 + 1;
        func_0006ACC8(D_001CEC80[i], D_001D2E58[i].time, 1);
        D_001CEA88[k].text = D_001C0870;
        D_001CEA88[k + 1].text = D_001D2E58[i].name;
        D_001CEA88[k + 2].text = D_001CEC80[i];
    } else {
        k = i * 4 + 1;
        D_001CEA88[k].text = D_001C07C0;
        D_001CEA88[k + 1].text = D_001C07C0;
        D_001CEA88[k + 2].text = D_001C07C0;
    }
}

