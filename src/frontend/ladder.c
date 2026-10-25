/* Boundary evidence (pilot/TU1.md, FRONTEND section):
 * entry func_001C9048 called from main 0x6F098 (main's ladder.c), LEFTARRO/RIGHTARR, MCARD.BIN (duplicated in
 * fecore/options/ladview -> own object). START: .rodata pad 0x1C09C4. END: .rodata pad 0x1C0A14 after
 * func_001C9404's jtbl. .rodata (3 literals + 2 jtbls) ONE block 0x1C09C8+0x4C. func_001C96C8/func_001C9724
 * (called only by func_001C9344) could still belong here: medium.
 */
/* ladder.c: 4 functions func_001C9048..func_001C9404 (0x1C9048-0x1C96C8)
 * Step 3: types from include/frontend.h + structs.h (DB *current/D_00097524, DB_ENV D_000BDFA4[2]); no views.
 */

/* ---- types ---- */
#include "frontend.h"

/* ---- externs ---- */
extern int D_001CEF58;
extern int D_001CEF54;
extern int D_00096C90;
extern unsigned int D_001CEF84;
extern int D_001CEF38;
extern int D_000BFA78;
extern int D_001CF28C;
extern int D_001CEF50;
extern int D_001CEF5C;
extern int D_001CEF88;
extern int D_001CEF60;
extern int D_001D3110;
extern int D_001D3118;
extern int D_00096804;
extern unsigned char D_00096F1C;
extern unsigned char D_000967BC;
extern char D_001D30F0[];
extern char D_001D3100[];
extern char D_001CEF30[];
extern void func_0004D5F8();
extern void func_001CB190();
extern void CloseShutters();
extern void func_001C9A0C();
extern void GetTextureDetails();
extern int func_001C9A18();
extern void func_001CB4A0();
extern void SetTextColour();
extern void func_000514E0();
extern void func_001CB380();
extern void func_001CB95C();
extern void func_001C977C();
extern void DrawSync();
extern void func_000513D4();
extern void func_0006F6E8();
extern int VSync();
extern int D_000BFA68;
extern int D_000BFA6C;
extern int D_000BFA70;
extern int D_000BFA74;
extern void func_001CA270();
extern unsigned short D_00097538;
extern void func_001C96C8();
extern void func_001C9724();
extern void func_001C9928();
extern DB *current;
extern DB *D_00097524;
extern DB_ENV D_000BDFA4[];
extern int D_00096834;
extern char D_000868DC[];
extern int func_001CAF70();
extern int func_001CA600();
extern void func_001CA298();
extern void func_001CD5A8();
extern void SetDispMask();
extern void func_00033AF8();
extern void Loadfile();
extern void func_000EC280();
extern void func_000E7508();
extern void func_0003A068();
extern void func_0006EB48();
extern void func_0006F57C();

/* ---- functions defined in this file ---- */
int func_001C9048();
void func_001C9298();
int func_001C9344();
int func_001C9404();

/* ---------------- func_001C9048 (src/pilot/frontend/func_001C9048.c) ---------------- */
int func_001C9048(int a0)
{
    int n = 0;

    D_001CEF58 = a0;
    D_00096F1C = 0;
    D_000967BC = 1;
    D_001CEF54 = D_00096C90;
    func_0004D5F8(n);
    func_001CB190();
    CloseShutters(2, 320, 512);
    D_001CEF84 = 0;
    func_001C9A0C(5);
    D_001CEF38 = D_000BFA78;
    D_001CEF84 = 0;
    GetTextureDetails("LEFTARRO", D_001D30F0);
    GetTextureDetails("RIGHTARR", D_001D3100);
    D_001CF28C = -1;
    D_001CEF50 = 0;
    func_001CB4A0(1, func_001C9A18(n));
    D_001CEF5C = 0;
    SetTextColour(255, 255, 255);
    for (;;) {
        func_000514E0();
        if (func_001C9344() == -1)
            break;
        func_001CB380();
        if (D_001CEF5C == 1) {
            D_00096C90 += D_001D3118;
            if (D_00096C90 >= D_001D3110) {
                D_00096C90 = D_001D3110;
                D_001CEF5C = 0;
            }
        }
        if (D_001CEF88 == 0) {
            if (n == 1)
                func_001C9298();
            if (D_001CEF88 == 0)
                func_001CB95C(D_001CEF30);
        }
        n = D_001CEF88;
        if (n == 0)
            func_001C977C();
        while ((unsigned)(VSync(-1) - D_001CEF60) < 2)
            ;
        D_001CEF60 = VSync(-1);
        DrawSync(0);
        func_000513D4();
    }
    D_00096804 = 0;
    D_00096C90 = D_001CEF54;
    func_0006F6E8();
    return D_001CF28C;
}

/* ---------------- func_001C9298 (src/pilot/frontend/func_001C9298.c) ---------------- */
void func_001C9298(void)
{
    switch (D_001CEF84) {
    case 1: D_001CEF38 = D_000BFA68; break;
    case 2: D_001CEF38 = D_000BFA6C; break;
    case 3: D_001CEF38 = D_000BFA70; break;
    case 4: D_001CEF38 = D_000BFA74; break;
    case 0: D_001CEF38 = D_000BFA78; break;
    }
    func_001CA270(255, 255, 255, 64);
}

/* ---------------- func_001C9344 (src/pilot/frontend/func_001C9344.c) ---------------- */
/* func_001C9344: menu input handler. One shared final return 0 (nested ifs, no early return 0s) - the -1 path tests\n   the call result in v0 directly; reorg copies the ret0 insn into the other branch slots. */
int func_001C9344(void)
{
    if (D_001CEF88 == 0) {
        unsigned short b = D_00097538;
        if (!(b & 0x10)) {
            if (b & 0x8000) {
                func_001C9724();
            } else if (b & 0x2000) {
                func_001C96C8();
            } else if (b & 0x40) {
                if (func_001C9404())
                    return -1;
            }
        }
    } else {
        int a = 0;
        if (((short)D_00097538 & 0x8000) == 0) {
            if ((D_00097538 & 0x2000) == 0) return 0;
            a = 1;
        }
        func_001C9928(a);
    }
    return 0;
}

/* ---------------- func_001C9404 (src/pilot/frontend/func_001C9404.c) ---------------- */
int func_001C9404(void)
{
    int s;

    switch (D_001CEF84) {
    case 1:
        if (func_001CAF70() != 1)
            return 0;
        func_001C9A18(6);
        func_001CB4A0(0, func_001C9A18(6));
        while (D_001CEF88 != 0) {
            func_001CB380();
            func_001CA298();
            func_001CB95C(D_001CEF30);
            DrawSync(0);
            func_000513D4();
        }
        D_001CF28C = -1;
        return 1;
    case 2:
        func_001CB4A0(0, func_001C9A18(7));
        while (D_001CEF88 != 0) {
            func_001CB380();
            func_001CA298();
            func_001CB95C(D_001CEF30);
            DrawSync(0);
            func_000513D4();
        }
        D_001CF28C = -2;
        return -1;
    case 3:
        func_001CD5A8();
        return 0;
    case 4:
        D_001D3110 = D_00096C90;
        if (func_001CA600() == 1)
            return -1;
        D_001D3118 = 10000 / func_001C9A18(4);
        D_001CEF5C = 1;
        break;
    case 0:
        s = D_00096834;
        func_001CB4A0(0, func_001C9A18(5));
        SetDispMask(0);
        func_00033AF8();
        func_000513D4();
        func_000513D4();
        DrawSync(0);
        Loadfile("MCARD.BIN", 0xE6600);
        func_000EC280(1);
        func_000E7508(4, D_000868DC, 0x1E00);
        SetDispMask(0);
        D_00096834 = 4;
        current = (DB *)D_000BDFA4;
        D_00097524 = (DB *)(D_000BDFA4 + 1);
        func_0003A068();
        func_00033AF8();
        D_00096834 = s;
        func_0006EB48();
        D_001CEF84 = 0;
        func_001C9A0C(5);
        func_001CB4A0(1, func_001C9A18(0));
        func_0006F57C();
        break;
    }
    return 0;
}
