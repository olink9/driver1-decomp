typedef struct { short x; unsigned short id; int val; } ENTRY;
typedef struct { ENTRY e[4]; } ENTRY4;
/* byte-offset indexing (sel << 3) is what gives the p-first addu */
#define ENT(base, i) ((ENTRY *)((char *)(base) + ((i) << 3)))
typedef struct { int id; int arg; int val; } TUNE;
typedef struct { int a, b, c, d; } ITEM16;

extern TUNE D_0008D0EC[];
extern TUNE D_0008D1A0[];
extern char D_00011650[], D_00011664[], D_00011674[];
extern long long D_00096834, D_000967F4, D_00096808, D_00096814, D_00097538;
extern int D_00096F90;
extern ITEM16 *D_000977F0;
extern void func_0004E170();
extern void func_0004E348();
extern int func_00079AE0();
extern void func_0006D610();
extern void func_00079AF0();
extern void func_0006D56C();
extern void func_0006F57C();
extern void func_0006F5CC();
extern int func_0006F6DC();
extern void func_000514E0();
extern void func_0006F728();
extern void func_00074158();
extern void func_00050E28();
extern int func_1C9048();
extern void func_00024754();
extern void func_0004DF28();

ITEM16 *func_0006EEC4(ENTRY *p) {
    ENTRY4 copy;
    int sel, found, v, save, old, i;

    copy = *(ENTRY4 *)p;
    func_0004E170();
    func_0004E348();
    *(int *)&D_00096834 = D_00096F90;
    for (i = 0; D_0008D0EC[i].id != -1; i++) {
        if (copy.e[0].id == D_0008D0EC[i].id) {
            save = func_00079AE0();
            func_0006D610(D_0008D0EC[i].arg);
            func_00079AF0(save);
            func_0006D56C();
            func_0006F57C();
            v = D_0008D0EC[i].val;
            if (v < 100)
                func_0006F5CC(8, v / 8, v % 8);
            else
                func_0006F5CC(16, 0, v - 100);
            while (func_0006F6DC()) {
                func_000514E0();
                if (*(short *)&D_00097538)
                    func_0006F728();
            }
            func_00074158(0);
            func_00050E28(D_00011650, 0x1FE000);
            return &D_000977F0[p->val];
        }
    }
    for (;;) {
        *(int *)&D_00096834 = D_00096F90;
        func_00050E28(D_00011664, 0x1C0000);
        func_00050E28(D_00011674, 0xE6600);
        sel = func_1C9048(&copy);
        if (sel >= 0) {
            found = 0;
            for (i = 0; D_0008D1A0[i].id != -1 && !found; i++) {
                if (ENT(p, sel)->id == D_0008D1A0[i].id) {
                    save = func_00079AE0();
                    func_0006D610(D_0008D1A0[i].arg);
                    func_00079AF0(save);
                    func_0006D56C();
                    func_0006F57C();
                    v = D_0008D1A0[i].val;
                    if (v < 100)
                        func_0006F5CC(8, v / 8, v % 8);
                    else
                        func_0006F5CC(16, 0, v - 100);
                    while (func_0006F6DC()) {
                        func_000514E0();
                        if (*(short *)&D_00097538)
                            func_0006F728();
                    }
                    func_00050E28(D_00011650, 0x1FE000);
                    found = 1;
                }
            }
            if (ENT(p, sel)->id == 0x13) {
                save = func_00079AE0();
                func_0006D610(0x21);
                func_00079AF0(save);
                func_0006D56C();
            }
            func_00050E28(D_00011650, 0x1FE000);
            return &D_000977F0[ENT(p, sel)->val];
        }
        if (sel == -1)
            break;
        old = *(int *)&D_00096814;
        *(int *)&D_000967F4 = 0x17;
        *(int *)&D_00096814 = 3;
        if (D_00096F90 == 0) *(int *)&D_00096808 = 0x84;
        else if (D_00096F90 == 1) *(int *)&D_00096808 = 0x87;
        else if (D_00096F90 == 2) *(int *)&D_00096808 = 0x88;
        else if (D_00096F90 == 3) *(int *)&D_00096808 = 0x8B;
        func_00024754();
        *(int *)&D_00096814 = old;
        *(int *)&D_000967F4 = 0x20;
        func_0004DF28();
        func_0004E348();
    }
    return 0;
}
