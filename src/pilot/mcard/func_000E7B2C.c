typedef struct { char name[20]; int attr; int size; void *next; int head; char system[4]; } DIRENTRY;
typedef struct { char fname[0x18]; char title[0x24]; int size; unsigned char idx; unsigned char n; unsigned char f42; unsigned char f43; } E;
typedef struct { unsigned short s; unsigned char c2; char c3; void *p4; E e[15]; } H;
extern H D_000EE930[];
extern unsigned char D_000EFA00[];
extern int D_000EF144;
extern int D_000ED870;
extern char D_000E66FC[];
extern char D_000E6700[];
extern char D_000E6714[];
extern char D_000E6728[];
extern char D_000E6738[];
extern char D_000E674C[];
extern void VSync();
extern int func_00078E4C();
extern int func_00078BD8();
extern int func_0007B218(); /* strncmp */
extern char *strcpy();
extern void func_000E8DD4();
extern int func_000E8CBC();
extern int func_000E7F5C();
extern void func_000E8E84();
extern void func_000EAAFC();
extern void func_000E950C(), func_000E91B8(), func_000E90D8();

/* read card `port`'s directory into D_000EE930[port] (names, titles, block counts, icons) */
void func_000E7B2C(int port)
{
    DIRENTRY dir[15];
    int r[2];
    int count;
    int n;
    int i;
    int k;
    int nb;
    int j;
    int p16;

    k = 0;
    r[0] = 0;
    r[1] = 0;
    n = 999;
    do {
        VSync(0);
        if (func_00078E4C(0, &r[0], &r[1]))
            break;
    } while (--n != -1);
    p16 = port << 4;
    func_000E8DD4(port);
    if (func_00078BD8(p16, D_000E66FC, dir, &count, 0, 15) == 0 && (unsigned)count < 16) {
        D_000EE930[port].s = 2;
        D_000EE930[port].c2 = 15;
        D_000EE930[port].p4 = D_000E6714;
        for (i = 0; i < count; i++) {
            if (func_000E8CBC(port, 1))
                return;
            nb = dir[i].size / 8192 + (dir[i].size % 8192 != 0);
            strcpy(D_000EE930[port].e[k].fname, dir[i].name);
            D_000EE930[port].e[k].size = dir[i].size;
            D_000EE930[port].e[k].idx = k;
            D_000EE930[port].e[k].n = nb;
            if (func_0007B218(dir[i].name, D_000E6728, 12))
                D_000EE930[port].e[k].f43 = 1;
            else
                D_000EE930[port].e[k].f43 = 3;
            if (func_000E7F5C(port, dir[i].name, D_000EFA00, 0x200) == 0) {
                func_000E8DD4(port);
                D_000EE930[port].s = 8;
                D_000EE930[port].p4 = D_000E6700;
                return;
            }
            D_000EE930[port].e[k].f42 = D_000EFA00[2] & 3;
            func_000E8E84(D_000EFA00 + 4, D_000EE930[port].e[k].title);
            func_000EAAFC(D_000EFA00 + 0x80, D_000EFA00 + 0x60, port, k);
            for (j = 1; j < nb; j++) {
                *(&D_000EE930[port].e[0] + (k + j)) = *(&D_000EE930[port].e[0] + k);
                func_000EAAFC(D_000EFA00 + 0x80, D_000EFA00 + 0x60, port, k + j);
            }
            k += nb;
            D_000EE930[port].c2 -= nb;
            for (j = 0; j < 2; j++) {
                func_000E950C();
                func_000E91B8();
                func_000E90D8();
            }
        }
        if ((unsigned)(D_000EF144 - 3) < 4) {
            if (D_000EE930[port].c2 == 0)
                D_000EE930[port].p4 = D_000E6738;
            else if (D_000EE930[port].c2 < D_000ED870)
                D_000EE930[port].p4 = D_000E674C;
            else
                D_000EE930[port].p4 = 0;
            return;
        }
    } else {
        D_000EE930[port].s = 8;
        D_000EE930[port].p4 = D_000E6700;
        return;
    }
    D_000EE930[port].p4 = 0;
}
