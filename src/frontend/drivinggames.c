/* Boundary evidence (pilot/TU1.md, FRONTEND section):
 * Driving Games menu func_001C4D04 + table D_001CE1A0 (Trail Blazer/Survival/Dirt Track...), city/mode screen
 * func_001C5268, Time Trial/Practice tables D_001CE33C/D_001CE408; func_001C4BC8 belongs here (called only by
 * func_001C5068/func_001C56A0). jtbl one block 0x1C04F0. END: no edge crosses 0x1C5A30 (span 60); its ""
 * (0x1C04E8) is duplicated by training.c (0x1C0520).
 */
/* drivinggames.c: 28 functions func_001C4BC8..func_001C5968 (0x1C4BC8-0x1C5A30)
 * Step 3: types from include/frontend.h (MENU_ITEM D_001CE1A0/D_001CE33C/D_001CE408, MENU_TEXT D_001CE120/
 * D_001CE2AC/D_001CE3D8); PL (a player record in main .bss) and ARG are file-local. No views.
 */

/* ---- types ---- */
#include "frontend.h"
typedef struct {
    char pad0[0xD0];
    char dD0[0x18];
    char dE8[0x1C];
    int w104;
    char pad108[0x2A0 - 0x108];
    int w2A0;
    char pad2A4[0x2E4 - 0x2A4];
    unsigned char b2E4;
} PL;
typedef struct { int a; char *b; } ARG;
/* ---- externs ---- */
extern PL D_000C6F04;
extern int D_0009D8DC[];
extern int D_000967D4;
extern int D_000967F4;
extern unsigned char D_000977BC;
extern char *func_0006E418();
extern void func_0006E39C();
extern void func_0006E644();
extern void func_00024754();
extern int D_001CE200;
extern MENU_ITEM D_001CE1A0[];
extern MENU_TEXT D_001CE120[];
extern void func_001C2644();
extern void func_001C2114();
extern void func_000514E0();
extern int func_001C26E4();
extern void func_001C19C4();
extern void func_001C1BD4();
extern void func_001C11E4();
extern char D_001CE134;
extern int D_001CDE5C;
extern int D_001CDE64;
extern char D_001CE144;
extern char D_001CE154;
extern char D_001CE164;
extern char D_001CE174;
extern int D_00096808;
extern int D_00096834;
extern int D_00096870;
extern int func_001C41D8();
extern char D_001CE184;
extern MENU_ITEM D_001CE33C[];
extern MENU_TEXT D_001CE2AC[];
extern char *D_001CE204[][8];
extern char *D_001CE2F4;
extern int D_001CE2A4;
extern int D_001CE2A8;
extern int D_001CE3BC;
extern int D_001CE3C0;
extern int D_000967E0;
extern int D_001CE3C4[];
extern void SetDispMask();
extern MENU_TEXT D_001CE3D8[];
extern int D_001D2DC8;
extern int D_001CE3D4;
extern MENU_ITEM D_001CE408[];

/* ---- functions defined in this file ---- */
void func_001C4BC8();
int func_001C4D04();
void func_001C4DB0();
void func_001C4DC0();
int func_001C4DCC();
void func_001C4E54();
void func_001C4E64();
int func_001C4E70();
void func_001C4EFC();
void func_001C4F0C();
int func_001C4F18();
void func_001C4FA4();
void func_001C4FB4();
int func_001C4FC0();
void func_001C504C();
void func_001C505C();
int func_001C5068();
void func_001C51B8();
void func_001C51C8();
int func_001C51D4();
int func_001C5268();
void func_001C5650();
void func_001C5674();
int func_001C56A0();
void func_001C57FC();
void func_001C5820();
int func_001C584C();
int func_001C5968();

/* ---------------- func_001C4BC8 (src/pilot/frontend/func_001C4BC8.c) ---------------- */
void func_001C4BC8(int n)
{
    unsigned char flag;
    ARG arg;

    D_000977BC = n;
    flag = D_000967D4;
    func_0006E39C(D_000C6F04.dE8, D_000967F4, func_0006E418(D_000967F4, n));
    func_00024754();
    if (D_0009D8DC[0] == 0 && D_0009D8DC[1] == 0 && D_0009D8DC[2] == 0 && D_0009D8DC[3] == 0
        && D_000C6F04.b2E4 == 0
        && !(flag != 0 && D_000967F4 == 0x16)
        && !((D_000967F4 == 0x12 || D_000967F4 == 0x16) && D_000C6F04.w104 == 0xFF0)) {
        arg.a = D_000C6F04.w2A0;
        arg.b = D_000C6F04.dD0;
        func_0006E644(&arg, D_000967F4, n);
    }
}

/* ---------------- func_001C4D04 (src/pilot/frontend/func_001C4D04.c) ---------------- */
int func_001C4D04(void)
{
    int r;
    D_001CE200 = 0;
    func_001C2644(D_001CE1A0, 6, 0);
    func_001C2114(0, D_001CE120, 1);
    for (;;) {
        func_000514E0();
        r = func_001C26E4(D_001CE1A0, &D_001CE200);
        if (r != 0) break;
        func_001C19C4();
        func_001C1BD4(D_001CE120);
        func_001C11E4();
    }
    if (r == -1) return 0;
    return r;
}

/* ---------------- func_001C4DB0 (src/pilot/frontend/func_001C4DB0.c) ---------------- */
void func_001C4DB0(void) { D_001CE134 = 1; }

/* ---------------- func_001C4DC0 (src/pilot/frontend/func_001C4DC0.c) ---------------- */
void func_001C4DC0(void) { D_001CE134 = 0; }

/* ---------------- func_001C4DCC (src/pilot/frontend/func_001C4DCC.c) ---------------- */
int func_001C4DCC(void)
{
    func_001C2114(0, 0, 0);
    D_001CDE64 = 0;
    D_001CDE5C = 6;
    func_001C5268();
    D_001CDE5C = 6;
    D_001CE200 = 0;
    func_001C2644(D_001CE1A0, 6, 0);
    func_001C2114(0, D_001CE120, 1);
    return 0;
}

/* ---------------- func_001C4E54 (src/pilot/frontend/func_001C4E54.c) ---------------- */
void func_001C4E54(void) { D_001CE144 = 1; }

/* ---------------- func_001C4E64 (src/pilot/frontend/func_001C4E64.c) ---------------- */
void func_001C4E64(void) { D_001CE144 = 0; }

/* ---------------- func_001C4E70 (src/pilot/frontend/func_001C4E70.c) ---------------- */
int func_001C4E70(void)
{
    func_001C2114(0, 0, 0);
    D_001CDE64 = 0;
    D_001CDE5C = 6;
    func_001C5268();
    D_001CDE5C = 6;
    D_001CE200 = 1;
    func_001C2644(D_001CE1A0, 6, 1);
    func_001C2114(0, D_001CE120, 1);
    return 0;
}

/* ---------------- func_001C4EFC (src/pilot/frontend/func_001C4EFC.c) ---------------- */
void func_001C4EFC(void) { D_001CE154 = 1; }

/* ---------------- func_001C4F0C (src/pilot/frontend/func_001C4F0C.c) ---------------- */
void func_001C4F0C(void) { D_001CE154 = 0; }

/* ---------------- func_001C4F18 (src/pilot/frontend/func_001C4F18.c) ---------------- */
int func_001C4F18(void)
{
    func_001C2114(0, 0, 0);
    D_001CDE64 = 0;
    D_001CDE5C = 6;
    func_001C5268();
    D_001CDE5C = 6;
    D_001CE200 = 2;
    func_001C2644(D_001CE1A0, 6, 2);
    func_001C2114(0, D_001CE120, 1);
    return 0;
}

/* ---------------- func_001C4FA4 (src/pilot/frontend/func_001C4FA4.c) ---------------- */
void func_001C4FA4(void) { D_001CE164 = 1; }

/* ---------------- func_001C4FB4 (src/pilot/frontend/func_001C4FB4.c) ---------------- */
void func_001C4FB4(void) { D_001CE164 = 0; }

/* ---------------- func_001C4FC0 (src/pilot/frontend/func_001C4FC0.c) ---------------- */
int func_001C4FC0(void)
{
    func_001C2114(0, 0, 0);
    D_001CDE64 = 0;
    D_001CDE5C = 6;
    func_001C5268();
    D_001CDE5C = 6;
    D_001CE200 = 3;
    func_001C2644(D_001CE1A0, 6, 3);
    func_001C2114(0, D_001CE120, 1);
    return 0;
}

/* ---------------- func_001C504C (src/pilot/frontend/func_001C504C.c) ---------------- */
void func_001C504C(void) { D_001CE174 = 1; }

/* ---------------- func_001C505C (src/pilot/frontend/func_001C505C.c) ---------------- */
void func_001C505C(void) { D_001CE174 = 0; }

/* ---------------- func_001C5068 (src/pilot/frontend/func_001C5068.c) ---------------- */
int func_001C5068(void)
{
    int old;
    func_001C2114(0, 0, 0);
    D_001CDE64 = 0;
    D_001CDE5C = 2;
    D_001CDE64 = 3;
    D_000967F4 = 0x13;
    if (func_001C41D8()) {
        switch (D_00096834) {
        case 0:
            D_00096808 = 100;
            break;
        case 1:
            D_00096808 = 101;
            break;
        case 3:
            D_00096808 = 102;
            break;
        case 5:
            D_00096808 = 103;
            break;
        }
        D_000967F4 = 0x13;
        old = D_00096870;
        D_00096870 = 2;
        func_001C4BC8(D_00096808 - 100);
        D_00096870 = old;
    }
    D_001CDE5C = 6;
    D_001CE200 = 4;
    func_001C2644(D_001CE1A0, 6, 4);
    func_001C2114(0, D_001CE120, 1);
    return 0;
}

/* ---------------- func_001C51B8 (src/pilot/frontend/func_001C51B8.c) ---------------- */
void func_001C51B8(void) { D_001CE184 = 1; }

/* ---------------- func_001C51C8 (src/pilot/frontend/func_001C51C8.c) ---------------- */
void func_001C51C8(void) { D_001CE184 = 0; }

/* ---------------- func_001C51D4 (src/pilot/frontend/func_001C51D4.c) ---------------- */
int func_001C51D4(void)
{
    func_001C2114(0, 0, 0);
    D_001CDE64 = 0;
    D_001CDE5C = 6;
    D_001CDE64 = 3;
    func_001C5968();
    D_001CDE5C = 6;
    D_001CE200 = 5;
    func_001C2644(D_001CE1A0, 6, 5);
    func_001C2114(0, D_001CE120, 1);
    return 0;
}

/* ---------------- func_001C5268 (src/pilot/frontend/func_001C5268.c) ---------------- */
/* = D_001CE2AC[4].text */
int func_001C5268(void)
{
    int i, r;

    D_001CE3BC = 0;
    for (i = 0; i < 8; i++) {
        D_001CE2AC[i].col.r = 0x80;
        D_001CE2AC[i].col.g = 0x80;
        D_001CE2AC[i].col.b = 0x80;
    }
    for (i = 0; i < 8; i++) {
        D_001CE33C[i].up = i - 1;
        D_001CE33C[i].down = i + 1;
    }
    D_001CE33C[0].up = 7;
    D_001CE33C[7].down = 0;
    if (D_001CE200 == 1) {
        D_001CE2A4 = 180;
        D_001CE2A8 = 36;
        for (i = 0; i < 4; i++)
            D_001CE2AC[i].y = D_001CE2A4 + i * D_001CE2A8;
        for (i = 0; i < 4; i++)
            D_001CE2AC[i].text = D_001CE204[1][i];
        D_001CE2AC[4].text = 0;
        D_001CE33C[0].up = 3;
        D_001CE33C[3].down = 0;
        D_001CE33C[D_000967E0 - 1].down = 0;
        D_001CE33C[0].up = D_000967E0 - 1;
        for (i = D_000967E0; i < 4; i++) {
            D_001CE2AC[i].col.r = 0x30;
            D_001CE2AC[i].col.g = 0x30;
            D_001CE2AC[i].col.b = 0x30;
        }
    } else if (D_001CE200 == 5) {
        D_001CE2A4 = 180;
        D_001CE2A8 = 34;
        for (i = 0; i < 4; i++)
            D_001CE2AC[i].y = D_001CE2A4 + i * D_001CE2A8;
        for (i = 0; i < 5; i++)
            D_001CE2AC[i].text = D_001CE204[4][i];
        D_001CE2F4 = 0;
        D_001CE33C[0].up = 3;
        D_001CE33C[3].down = 0;
    } else {
        D_001CE2A4 = 150;
        D_001CE2A8 = 30;
        for (i = 0; i < 8; i++)
            D_001CE2AC[i].y = D_001CE2A4 + i * D_001CE2A8;
        for (i = 0; i < 8; i++)
            D_001CE2AC[i].text = D_001CE204[D_001CE200][i];
        for (i = D_000967E0 * 2; i < 8; i++) {
            D_001CE2AC[i].col.r = 0x30;
            D_001CE2AC[i].col.g = 0x30;
            D_001CE2AC[i].col.b = 0x30;
        }
        D_001CE33C[D_000967E0 * 2 - 1].down = 0;
        D_001CE33C[0].up = D_000967E0 * 2 - 1;
    }
    func_001C2644(D_001CE33C, 4, D_001CE3BC);
    func_001C2114(0, D_001CE2AC, 1);
    for (;;) {
        func_000514E0();
        D_001CE3C0 = D_001CE3BC;
        r = func_001C26E4(D_001CE33C, &D_001CE3BC);
        if (r != 0)
            break;
        func_001C19C4();
        func_001C1BD4(D_001CE2AC);
        func_001C11E4();
    }
    if (r == -1)
        return 0;
    return r;
}

/* ---------------- func_001C5650 (src/pilot/frontend/func_001C5650.c) ---------------- */
void func_001C5650(void)
{
    D_001CE2AC[D_001CE3BC].font = 1;
}

/* ---------------- func_001C5674 (src/pilot/frontend/func_001C5674.c) ---------------- */
void func_001C5674(void)
{
    int i = D_001CE3C0;
    if (i != -1)
        D_001CE2AC[i].font = 0;
}

/* ---------------- func_001C56A0 (src/pilot/frontend/func_001C56A0.c) ---------------- */
/* func_001C56A0. FAKEMATCH: mul10()/add60() inline helpers stop fold from reassociating i*10 + (n+60) and keep the hi(D_00096808) before both operands (preexpand_calls order). Jump table checked: 0->0x14/+110, 1->0x15/+70, 2->0x12, 3->0x11, 4->none, 5->0x16/+155|+151. */
static inline int add60(int x) { return x + 60; }
static inline int mul10(int x) { return x * 10; }
int func_001C56A0(void)
{
    int i;
    int n;
    func_001C2114(0, 0, 0);
    SetDispMask(0);
    i = D_001CE200;
    if (D_001CE3C4[i] != 0) {
        D_001CDE64 = 0;
        n = D_001CE3BC;
        if (i == 0) {
            D_00096808 = n + 110;
        } else {
            D_00096808 = mul10(i) + add60(n);
        }
        switch (D_001CE200) {
        case 0:
            D_000967F4 = 0x14;
            D_00096808 = n + 110;
            break;
        case 1:
            D_000967F4 = 0x15;
            D_00096808 = n + 70;
            break;
        case 2:
            D_000967F4 = 0x12;
            break;
        case 3:
            D_000967F4 = 0x11;
            break;
        case 4:
            break;
        case 5:
            D_000967F4 = 0x16;
            if (D_000967D4)
                D_00096808 = n + 155;
            else
                D_00096808 = n + 151;
            break;
        }
        func_001C4BC8(n);
        D_001CDE5C = 6;
    }
    return 1;
}

/* ---------------- func_001C57FC (src/pilot/frontend/func_001C57FC.c) ---------------- */
void func_001C57FC(void)
{
    D_001CE3D8[D_001D2DC8].font = 1;
}

/* ---------------- func_001C5820 (src/pilot/frontend/func_001C5820.c) ---------------- */
void func_001C5820(void)
{
    int i = D_001CE3D4;
    if (i != -1)
        D_001CE3D8[i].font = 0;
}

/* ---------------- func_001C584C (src/pilot/frontend/func_001C584C.c) ---------------- */
int func_001C584C(void)
{
    int m = D_001D2DC8;
    if (m == 0) {
        D_000967D4 = 1;
        D_001CDE5C = 6;
        func_001C2114(0, 0, 0);
        D_001CDE64 = 0;
        D_001CDE5C = 6;
        func_001C5268();
        D_001CDE5C = 6;
        D_001D2DC8 = 0;
        D_001CE200 = 5;
        func_001C2644(D_001CE408, 2, 0);
    } else if (m == 1) {
        D_000967D4 = 0;
        func_001C2114(0, 0, 0);
        D_001CDE64 = 0;
        D_001CDE5C = 6;
        func_001C5268();
        D_001CDE5C = 6;
        D_001D2DC8 = m;
        D_001CE200 = 5;
        func_001C2644(D_001CE408, 2, 1);
    } else {
        return 0;
    }
    func_001C2114(0, D_001CE3D8, 1);
    return 0;
}

/* ---------------- func_001C5968 (src/pilot/frontend/func_001C5968.c) ---------------- */
int func_001C5968(void)
{
    int r;
    D_001D2DC8 = 0;
    func_001C2644(D_001CE408, 2, 0);
    func_001C2114(0, D_001CE3D8, 1);
    for (;;) {
        func_000514E0();
        D_001CE3D4 = D_001D2DC8;
        r = func_001C26E4(D_001CE408, &D_001D2DC8);
        if (r != 0) break;
        func_001C19C4();
        func_001C1BD4(D_001CE3D8);
        func_001C11E4();
    }
    if (r == -1) {
        D_000967D4 = 0;
        return 0;
    }
    return r;
}

