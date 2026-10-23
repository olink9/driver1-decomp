typedef struct {
    void (*enter)();
    void (*leave)();
    int (*select)();
    unsigned char up, down, left, right;
} MENU;

extern int D_00096A08, D_00096C90, D_00096C94, D_001CDE5C, D_001CDECC, D_001CDED0, D_001CDE68, D_001CDC80;
extern int D_000967D8;
extern unsigned short D_00097538;
extern short D_0009752C;
extern unsigned char D_001CDEE0[];
extern int D_0008671C[][14];
extern int func_0006F6DC();
extern void func_0006F5CC(), func_0006F478(), StartSound();

int func_001C26E4(MENU *m, int *sel)
{
    int moved = 0;
    int ret = 0;
    int old = *sel;
    int mode;
    int i;

    D_001CDED0 = 0;
    mode = D_00096A08;
    if (mode != 1)
        return 0;
    if (!func_0006F6DC()) {
        func_0006F5CC(8, 4, 7);
        func_0006F478(D_00096C90);
    }
    if (D_00097538 & 0x10) {
        m[old].leave();
        if (D_001CDE5C != 1)
            StartSound(0, 0, 0, D_00096C94, 0x1000);
        return -1;
    }
    if (D_001CDECC) {
        if (D_0009752C & 0x8000) {
            m[m[*sel].up].enter();
        } else if ((unsigned short)D_0009752C & 0x2000) {
            m[m[*sel].down].enter();
        } else if (D_00097538 & 0x1000) {
            if (*sel != m[*sel].left)
                D_001CDED0 = 1;
            *sel = m[*sel].left;
            moved = 1;
        } else if (D_00097538 & 0x4000) {
            if (*sel != m[*sel].right)
                D_001CDED0 = 1;
            *sel = m[*sel].right;
            moved = 1;
        } else if (D_00097538 & 0x40) {
            StartSound(0, 0, 2, D_00096C94, 0x1000);
            ret = m[*sel].select();
        }
    } else {
        if (D_00097538 & 0x4) {
            for (i = 0; i < D_000967D8; i++) {
                if (D_0008671C[i][D_001CDEE0[i]] == 0)
                    D_001CDEE0[i]++;
                else
                    D_001CDEE0[i] = 0;
            }
        } else if (D_00097538 & 0x1) {
            for (i = 0; i < D_000967D8; i++) {
                if (D_0008671C[i][D_001CDEE0[i]] == 1)
                    D_001CDEE0[i]++;
                else
                    D_001CDEE0[i] = 0;
            }
        } else if (D_00097538 & 0x8) {
            for (i = 0; i < D_000967D8; i++) {
                if (D_0008671C[i][D_001CDEE0[i]] == 2)
                    D_001CDEE0[i]++;
                else
                    D_001CDEE0[i] = 0;
            }
        } else if (D_00097538 & 0x2) {
            for (i = 0; i < D_000967D8; i++) {
                if (D_0008671C[i][D_001CDEE0[i]] == 3)
                    D_001CDEE0[i]++;
                else
                    D_001CDEE0[i] = 0;
            }
        } else if (D_00097538 & 0x8000) {
            if (*sel != m[*sel].up)
                D_001CDED0 = 1;
            *sel = m[*sel].up;
            moved = 1;
        } else if (D_00097538 & 0x2000) {
            if (*sel != m[*sel].down)
                D_001CDED0 = 1;
            *sel = m[*sel].down;
            moved = 1;
        } else if (D_00097538 & 0x1000) {
            if (*sel != m[*sel].left)
                D_001CDED0 = 1;
            *sel = m[*sel].left;
            moved = 1;
        } else if (D_00097538 & 0x4000) {
            if (*sel != m[*sel].right)
                D_001CDED0 = 1;
            *sel = m[*sel].right;
            moved = 1;
        } else if (D_00097538 & 0x40) {
            StartSound(0, 0, 2, D_00096C94, 0x1000);
            ret = m[*sel].select();
        }
    }
    if (moved) {
        D_001CDE68 = 0;
        D_001CDC80 = 1;
        m[old].leave();
        m[*sel].enter();
        if (D_001CDED0)
            StartSound(0, 0, 0, D_00096C94, 0x1000);
    } else {
        D_001CDC80 = 0;
    }
    for (i = 0; i < D_000967D8; i++) {
        if (D_0008671C[i][D_001CDEE0[i]] == -1) {
            StartSound(0, 0, 2, D_00096C94, 0x1000);
            D_0008671C[i][D_001CDEE0[i]] = -2;
            D_0008671C[i][0] = -2;
        }
    }
    return ret;
}
