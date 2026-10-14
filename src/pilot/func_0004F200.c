extern long long D_00096AA8;
extern long long D_000977A8;
extern long long D_000977A4;
extern long long D_00097118;
extern int D_000974F0, D_000974F4, D_000974DC;

void func_0004F200(int lim) {
    int sh, t;
    if ((*(unsigned char *)&D_00096AA8) == 1) lim = 0x60; else lim = 0x80;
    if ((*(int *)&D_000977A8) | (*(int *)&D_000977A4)) {
        if ((*(int *)&D_000977A8)) {
            D_000974F0 = ((*(int *)&D_00097118) >> 5) + 0x29;
            D_000974F4 = ((*(int *)&D_00097118) >> 5) + 0x1C;
            D_000974DC = D_000974F4;
        } else {
            sh = (*(int *)&D_00097118) >> 5;
            t = sh - 15;
            D_000974F0 = lim - t;
            D_000974F4 = lim - sh;
            D_000974DC = D_000974F4;
        }
        if (D_000974F0 < 26) D_000974F0 = 26;
        else if (D_000974F0 > lim) D_000974F0 = lim;
        if (D_000974F4 < 26) D_000974F4 = 26;
        else if (D_000974F4 > lim) D_000974F4 = lim;
        if (D_000974DC < 26) D_000974DC = 26;
        else if (D_000974DC > lim) D_000974DC = lim;
    } else {
        if ((*(unsigned char *)&D_00096AA8) != 0) { D_000974DC = 0x3C; D_000974F4 = 0x3C; D_000974F0 = 0x3C; }
        else { D_000974DC = 0x80; D_000974F4 = 0x80; D_000974F0 = 0x80; }
    }
}
