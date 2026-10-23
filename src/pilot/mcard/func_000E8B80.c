typedef struct { int a; short b; char pad[0x3A]; unsigned char idx; unsigned char n; char f42; char f43; } E;
typedef struct { short s; char c2; char c3; void *p4; E e[15]; } H;
extern H D_000EE930[];
extern char D_000E6810[];
extern void VSync();
extern int func_00078E4C();
extern int func_00079170();
extern void func_000EAE3C();

int func_000E8B80(int port, int slot)
{
    int r[2];
    int n;
    int i;

    r[0] = 0;
    r[1] = 0;
    n = 999;
    do {
        VSync(0);
        if (func_00078E4C(0, &r[0], &r[1]))
            break;
    } while (--n != -1);
    D_000EE930[port].c3 = 1;
    if (func_00079170(port << 4, &D_000EE930[port].e[slot])) {
        func_000EAE3C(D_000E6810, 2);
        return 0;
    }
    for (i = 0; i < D_000EE930[port].e[slot].n; i++)
        D_000EE930[port].e[D_000EE930[port].e[slot].idx + i].f43 = 0;
    return 1;
}
