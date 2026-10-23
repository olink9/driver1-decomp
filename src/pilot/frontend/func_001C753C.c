/* FAKEMATCH: unused `pad[8]` only sizes the stack frame (target frame 0x40). OFF printed once at a shared tail (goto), so loop.c hoists only ON. */
typedef struct { int state; char pad[0x34]; } E_8671C;
extern E_8671C D_0008671C[];
extern int D_000967D8, D_0009681C, D_00096820, D_001D2E48, D_001D2E50;
extern int D_0009D8DC[];
extern char *D_001CEA40[];
extern char D_001C079C[], D_001C07A0[], D_001C07A4[];
extern void func_000E6F14(), func_000E6F98();

void func_001C753C(void)
{
    int *p = D_0009D8DC;
    int y = 150;
    int i;
    char pad[8];

    for (i = 0; i < D_000967D8; i++) {
        if (D_0008671C[i].state == -2) {
            if (i == D_001D2E48)
                func_000E6F14(20, y, 1, 0, 0);
            else
                func_000E6F14(20, y, 0, 0, 0);
            func_000E6F98(D_001CEA40[i]);
            if (i == 1) {
                func_000E6F14(0x17C, y, 0, 0, 0);
                if (D_0009681C) {
                    func_000E6F98(D_001C079C);
                    goto next;
                }
            } else if (i == 2) {
                func_000E6F14(0x17C, y, 0, 0, 0);
                if (D_00096820) {
                    func_000E6F98(D_001C079C);
                    goto next;
                }
            } else if (i != 0) {
                func_000E6F14(0x17C, y, 0, 0, 0);
                if (*p == 1) {
                    func_000E6F98(D_001C079C);
                    goto next;
                }
            } else
                goto next;
            func_000E6F98(D_001C07A0);
        next:
            y += 0x24;
        }
        if (i >= 3)
            p++;
    }
    if (D_001D2E50 == 0) {
        func_000E6F14(320, 240, 1, 2, 0);
        func_000E6F98(D_001C07A4);
    }
}
