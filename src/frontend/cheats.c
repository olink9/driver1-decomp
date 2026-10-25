/* Boundary evidence (pilot/TU1.md, FRONTEND section):
 * cheat screen func_001C70F0 (jtbl) + func_001C753C (ANTIPODEAN...CREDITS via D_001CEA40, ON/OFF, "Sorry - No
 * Cheats Activated"). END: text pad at 0x1C772C. .rodata 2 pieces: the hole 0x1C0748-0x1C079C is its own
 * D_001CEA40 strings (data, still asm).
 */
/* cheats.c: 2 functions func_001C70F0..func_001C753C (0x1C70F0-0x1C7730)
 * Step 3: types from include/frontend.h; D_0008671C (one 14-int button sequence per cheat, [0] = state) spelled as in fecore.c. No views.
 */

/* ---- types ---- */
#include "frontend.h"

/* ---- externs ---- */
extern int D_0008671C[][14];
extern int D_000967D8;
extern int D_000967F4;
extern int D_00096800;
extern int D_00096808;
extern int D_0009681C;
extern int D_00096820;
extern int D_00096834;
extern int D_0009D8DC[];
extern int D_001CDE5C;
extern int D_001D2E48;
extern int D_001D2E50;
extern volatile unsigned short D_00097538;
extern void func_000514E0();
extern void func_00024754();
extern void func_001C2114();
extern void func_001C19C4();
extern void func_001C11E4();
extern char *D_001CEA40[];
extern void func_000E6F14();
extern void func_000E6F98();

/* ---- functions defined in this file ---- */
int func_001C70F0();
void func_001C753C();

/* ---------------- func_001C70F0 (src/pilot/frontend/func_001C70F0.c) ---------------- */
int func_001C70F0(void)
{
    unsigned char found = 0;
    int i;
    int old;
    int ret;
    unsigned short pad;

    D_001D2E50 = 0;
    D_001D2E48 = 0;
    D_0008671C[6][0] = -1;
    for (i = 0; i < D_000967D8; i++) {
        if (D_0008671C[i][0] == -2) {
            if (!found) {
                found = 1;
                D_001D2E48 = i;
            }
            D_001D2E50++;
        }
    }
    D_001CDE5C = 6;
    func_001C2114(0, 0, 1);
    for (;;) {
        func_000514E0();
        if (D_00097538 & 0x4000) {
            old = D_001D2E48;
            if (++D_001D2E48 >= 8)
                D_001D2E48 = 0;
            while (D_0008671C[D_001D2E48][0] != -2) {
                if (old == D_001D2E48)
                    break;
                if (++D_001D2E48 >= 8)
                    D_001D2E48 = 0;
            }
        } else if (D_00097538 & 0x1000) {
            old = D_001D2E48;
            if (--D_001D2E48 < 0)
                D_001D2E48 = 7;
            while (D_0008671C[D_001D2E48][0] != -2) {
                if (old == D_001D2E48)
                    break;
                if (--D_001D2E48 < 0)
                    D_001D2E48 = 7;
            }
        }
        pad = D_00097538;
        if (pad & 0x40) {
            if (D_001D2E50 > 0) {
                switch (D_001D2E48) {
                case 0:
                    D_00096820 = 0;
                    D_0009681C = 0;
                    D_00096834 = 7;
                    D_00096800 = 1;
                    D_00096808 = 5;
                    D_0009D8DC[0] = 0;
                    D_0009D8DC[1] = 0;
                    D_0009D8DC[2] = 0;
                    D_0009D8DC[3] = 0;
                    D_0009D8DC[4] = 0;
                    D_000967F4 = 7;
                    func_00024754();
                    D_001D2E50 = 0;
                    for (i = 0; i < D_000967D8; i++) {
                        if (D_0008671C[i][0] == -2) {
                            if (!found) {
                                found = 1;
                                D_001D2E48 = i;
                            }
                            D_001D2E50++;
                        }
                    }
                    break;
                case 3:
                    D_0009D8DC[0] ^= 1;
                    break;
                case 1:
                    D_0009681C ^= 1;
                    break;
                case 2:
                    D_00096820 ^= 1;
                    break;
                case 4:
                    D_0009D8DC[1] ^= 1;
                    break;
                case 5:
                    D_0009D8DC[2] ^= 1;
                    break;
                case 7:
                    D_0009D8DC[4] ^= 1;
                    break;
                }
            }
        } else if (pad & 0x10) {
            ret = -1;
            break;
        }
        func_001C19C4();
        func_001C11E4();
        func_001C753C(0, 50);
    }
    D_001CDE5C = 1;
    return ret;
}

/* ---------------- func_001C753C (src/pilot/frontend/func_001C753C.c) ---------------- */
/* FAKEMATCH: unused `pad[8]` only sizes the stack frame (target frame 0x40). OFF printed once at a shared tail (goto), so loop.c hoists only ON. */
void func_001C753C(void)
{
    int *p = D_0009D8DC;
    int y = 150;
    int i;
    char pad[8];

    for (i = 0; i < D_000967D8; i++) {
        if (D_0008671C[i][0] == -2) {
            if (i == D_001D2E48)
                func_000E6F14(20, y, 1, 0, 0);
            else
                func_000E6F14(20, y, 0, 0, 0);
            func_000E6F98(D_001CEA40[i]);
            if (i == 1) {
                func_000E6F14(0x17C, y, 0, 0, 0);
                if (D_0009681C) {
                    func_000E6F98("ON");
                    goto next;
                }
            } else if (i == 2) {
                func_000E6F14(0x17C, y, 0, 0, 0);
                if (D_00096820) {
                    func_000E6F98("ON");
                    goto next;
                }
            } else if (i != 0) {
                func_000E6F14(0x17C, y, 0, 0, 0);
                if (*p == 1) {
                    func_000E6F98("ON");
                    goto next;
                }
            } else
                goto next;
            func_000E6F98("OFF");
        next:
            y += 0x24;
        }
        if (i >= 3)
            p++;
    }
    if (D_001D2E50 == 0) {
        func_000E6F14(320, 240, 1, 2, 0);
        func_000E6F98("Sorry - No Cheats Activated");
    }
}
