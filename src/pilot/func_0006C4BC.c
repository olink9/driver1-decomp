typedef struct {
    void (*enter)();
    void (*leave)();
    int (*action)();
    unsigned char left, right, up, down;
} MENU;
typedef struct { int a, b; char *text; int d; } MTEXT;

extern long long D_000967E8, D_00097538, D_00096CB0, D_0009752C, D_00096C94, D_00096C90, D_0009763C;
extern int D_00096F14;
extern unsigned char D_00096EFC, D_00096EFD;
extern MTEXT *D_000977C0;
extern int D_00096F08, D_00096F0C, D_00096F10, D_00096F68, D_000977C4, D_000977C8, D_000977CC, D_000977D0;
extern char D_000977B4[8], D_000977B8[8], D_00096F6C[];
extern MENU D_0008D064[];
extern void SetMasterVolume();
extern void func_0004E454();
extern void func_0006C8D4();
extern int sprintf();

static inline int func_0006C438(void) {
    D_00096F68 = 1;
    D_00096F10 = -1;
    D_000977CC = D_000977D0;
    D_000977C0 = (MTEXT *)D_000977C4;
    D_00096F08 = D_00096F0C;
    func_0006C8D4(D_000977D0, D_000977C8, D_00096F0C);
    *(int *)&D_0009763C = 0;
    return D_00096F08;
}

int func_0006C4BC(MENU *menu, int sel) {
    int old = sel;
    unsigned short pad;

    if (*(int *)&D_000967E8 != 0 && *(short *)&D_00097538 != 0) {
        *(int *)&D_000967E8 = 0;
        *(int *)&D_00096CB0 = 1;
        return 9;
    }
    if (D_00096F14 == 0) {
    } else if (D_00096F14 == 1) {
        pad = *(unsigned short *)&D_0009752C;
        if (pad & 0x2000) {
            if (*(int *)&D_00096C94 < 0) {
                SetMasterVolume(*(int *)&D_00096C94 += 100);
                sprintf(D_000977B4, D_00096F6C, *(int *)&D_00096C94 / 100 + 100);
                D_000977C0[D_00096EFC].text = D_000977B4;
            }
        } else if (pad & 0x8000) {
            if (*(int *)&D_00096C94 > -10000) {
                SetMasterVolume(*(int *)&D_00096C94 -= 100);
                sprintf(D_000977B4, D_00096F6C, *(int *)&D_00096C94 / 100 + 100);
                D_000977C0[D_00096EFC].text = D_000977B4;
            }
        }
    } else if (D_00096F14 == 2) {
        pad = *(unsigned short *)&D_0009752C;
        if (pad & 0x2000) {
            if (*(int *)&D_00096C90 < 0) {
                func_0004E454(*(int *)&D_00096C90 += 100);
                sprintf(D_000977B8, D_00096F6C, *(int *)&D_00096C90 / 100 + 100);
                D_000977C0[D_00096EFD].text = D_000977B8;
            }
        } else if (pad & 0x8000) {
            if (*(int *)&D_00096C90 > -10000) {
                func_0004E454(*(int *)&D_00096C90 -= 100);
                sprintf(D_000977B8, D_00096F6C, *(int *)&D_00096C90 / 100 + 100);
                D_000977C0[D_00096EFD].text = D_000977B8;
            }
        }
    }
    if (D_00096F14 == 0 && (*(unsigned short *)&D_00097538 & 0x800))
        sel = menu[sel].action();

    pad = *(unsigned short *)&D_00097538;
    if (pad & 0x10) {
        if ((MENU *)D_000977CC == D_0008D064)
            return func_0006C438();
    } else if (pad & 0x1000) {
        sel = menu[sel].up;
    } else if (pad & 0x4000) {
        sel = menu[sel].down;
    } else if (D_00096F14 == 0) {
        if (pad & 0x40)
            sel = menu[sel].action();
        else if (pad & 0x8000)
            sel = menu[sel].left;
        else if (pad & 0x2000)
            sel = menu[sel].right;
    }
    if (D_00096F68) {
        D_00096F68 = 0;
        return sel;
    }
    if (old != sel) {
        menu[old].leave();
        menu[sel].enter();
    }
    return sel;
}
