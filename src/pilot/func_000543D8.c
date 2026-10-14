typedef struct { char pad[0xC]; int flags; char pad2[0x2C - 0x10]; } ITEM;
typedef struct { char pad[0xC]; short val; } TXT;
typedef struct {
    char pad[0x130];
    ITEM items[6];
    char pad2[0x2BC - 0x238];
    int active;
    char pad3[0x2D0 - 0x2C0];
    int mode;
} HUD;
typedef struct { int a, b, c, d; } TMP;
extern HUD D_000C6F04;
extern int D_000C71C0[];
extern long long D_00096899;
extern long long D_00096820, D_00097708, D_000970D8, D_000967E8, D_00096DC4, D_00096834, D_00097704, D_000977AC;
extern unsigned char D_000975E0;
extern int D_000975DC;
extern TXT D_000BE774, D_000BE998;
extern void func_00067E58(), func_00057510(), func_00056A18(), func_00057AE4(), func_0005474C(), func_00055820(), func_00056BDC();

void func_000543D8(void)
{
    char pad[16];
    TMP tmp;
    ITEM *p;
    unsigned char c;
    if (D_000C6F04.active != 0) {
        for (p = D_000C6F04.items; p < &D_000C6F04.items[6]; p++) {
            if (p->flags & 1) {
                if (D_000C6F04.mode == 2) {
                    func_00067E58(p, &tmp);
                    func_00057510(&tmp);
                } else {
                    func_00056A18(p);
                }
            }
        }
    }
    if (*(unsigned char *)&D_00096899) {
        if (*(int *)&D_00096820)
            *(int *)&D_00097708 = 0;
        return;
    }
    c = *(int *)&D_000970D8 % 16;
    if (c >= 8)
        c = 16 - (*(int *)&D_000970D8 % 16);
    D_000975E0 = c;
    if (*(int *)&D_000967E8) {
        *(int *)&D_00096DC4 = 1;
        func_00057AE4();
        return;
    }
    *(int *)&D_00096DC4 = 0;
    if (*(int *)&D_00096834 != 6 && *(int *)&D_00096834 != 7)
        func_0005474C();
    if (*(int *)&D_00097704 && *(int *)&D_00097708 > 0x292)
        func_00055820();
    func_00056BDC(&D_000BE774);
    if (*(int *)&D_00096820) {
        *(int *)&D_00097708 = 0;
    } else {
        D_000BE998.val = *(int *)&D_00097708;
        func_00056BDC(&D_000BE998);
    }
    if (D_000975DC && D_000C71C0[0] == 0)
        func_00056A18(*(int *)&D_000977AC);
}
