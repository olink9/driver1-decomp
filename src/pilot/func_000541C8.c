/* InitOverheadMap */
typedef struct { int a, b, width, height, e, f; } OVMAP;
extern OVMAP D_0008B080[];
extern long long D_00096834;
extern int D_00096D24;
extern int D_00096D28;
extern int D_00096D2C;
extern int D_00096D30;
extern int D_000975D0;
extern int D_000975D4;
extern int D_000975D8;
extern unsigned char D_000BEEAC[4][4];
extern void func_000563CC();
extern void func_00056278();

void func_000541C8(void)
{
    int c, d;
    int tpage;

    func_000563CC();
    D_00096D2C = D_0008B080[*(int *)&D_00096834].width;
    D_00096D30 = D_0008B080[*(int *)&D_00096834].height;
    tpage = 0;
    D_000975D8 = D_00096D2C / 32;
    for (c = 0; c < 4; c++) {
        for (d = 0; d < 4; d++) {
            D_000BEEAC[d][c] = tpage;
            func_00056278(tpage, (D_00096D24 >> 5) + d, (D_00096D28 >> 5) + c);
            tpage++;
        }
    }
    D_000975D0 = D_00096D24 & 0x1f;
    D_000975D4 = D_00096D28 & 0x1f;
}
