typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
typedef struct {
    P_TAG tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    short x1, y1;
    short x2, y2;
    short x3, y3;
} POLY_F4;
typedef struct { char *primptr; P_TAG *ot; } DB;
extern DB *current;
extern POLY_F4 D_000EF9A0;
extern int D_00096E34;
extern short D_00097538;
extern char D_000E6A34[];
extern char D_000E6A54[];
extern char D_000E6AA8[];
extern char D_000E6B2C[];
extern char D_000E6BC0[];
extern char D_000E6BC4[];
extern void VSync();
extern int func_00078E4C();
extern void func_00077ED8();
extern void func_000E8DD4();
extern void func_000EAE3C(), func_000514E0(), func_000EC260(), func_000E91B8(), func_0005D164(), func_000E90D8();
extern int func_0007B228();
extern int PrintString();

/* func_000E8CBC inlined (called with flag = 0) */
static inline int check_card(int port, int flag)
{
    int a[2];
    int b[2];
    int n;
    int arg;
    int r;
    int p16 = port << 4;

    b[0] = 0;
    b[1] = 0;
    if (flag) {
        a[0] = 0;
        a[1] = 0;
        n = 999;
        do {
            VSync(0);
            if (func_00078E4C(0, &a[0], &a[1]))
                break;
        } while (--n != -1);
        func_00077ED8(p16);
        arg = 0;
    } else {
        arg = 1;
    }
    r = func_00078E4C(arg, &b[0], &b[1]);
    switch (r) {
    case 0:
        return 0;
    case -1:
        func_00077ED8(p16);
        return 0;
    case 1:
        if (b[0] == 1 && b[1]) {
            func_000E8DD4(port);
            return 1;
        }
        return 0;
    default:
        return 0;
    }
}

/* Yes/No question box; returns 1 for Yes */
int func_000EB1F8(int port, char *msg)
{
    int sel = 0;
    int xoff = sel;
    unsigned char rgb[4] = { 0x80, 0x80, 0x80, 0 };
    int r[2];
    int n;
    POLY_F4 *p;
    POLY_F4 *q;

    r[0] = 0;
    r[1] = 0;
    n = 999;
    do {
        VSync(0);
        if (func_00078E4C(0, &r[0], &r[1]))
            break;
    } while (--n != -1);
    p = &D_000EF9A0;
    ((P_TAG *)p)->len = 5;
    p->code = 0x28;
    p->r0 = 0;
    p->g0 = 0;
    p->b0 = 0;
    if (msg == D_000E6A54 || msg == D_000E6AA8) {
        p->x0 = 0x60;
        p->y0 = 0x5C;
        p->x1 = 0x198;
        p->y1 = 0x5C;
        p->x2 = 0x60;
        p->y2 = 0xAC;
        p->x3 = 0x198;
        p->y3 = 0xAC;
    } else {
        p->x0 = 0x60;
        p->y0 = 0x5C;
        p->x1 = 0x198;
        p->y1 = 0x5C;
        p->x2 = 0x60;
        p->y2 = 0x94;
        p->x3 = 0x198;
        p->y3 = 0x94;
    }
    q = &D_000EF9A0;
    q->code = (q->code & ~2) | 1;
    if (func_0007B228(msg, D_000E6B2C) == 0)
        xoff += 0x3C;
    for (;;) {
        if (check_card(port, 0)) {
            func_000EAE3C(D_000E6A34, 2);
            return 0;
        }
        func_000514E0();
        if (D_00097538 & 0xA000) {
            sel ^= 1;
            func_000EC260();
        }
        if (D_00097538 & 0x40) {
            func_000EC260();
            break;
        }
        func_000E91B8();
        D_000EF9A0.tag.addr = current->ot[3].addr;
        current->ot[3].addr = (unsigned int)&D_000EF9A0;
        D_00096E34 = 1;
        func_0005D164(msg, xoff + 0x7C, 0x64, 0x1000, 0x1000, 1);
        if (sel == 0) {
            if (msg == D_000E6A54 || msg == D_000E6AA8)
                PrintString(D_000E6BC0, 0xE2, 0x98);
            else
                PrintString(D_000E6BC0, 0xE2, 0x80);
        } else {
            if (msg == D_000E6A54 || msg == D_000E6AA8)
                PrintString(D_000E6BC4, 0xE2, 0x98);
            else
                PrintString(D_000E6BC4, 0xE2, 0x80);
        }
        func_000E90D8();
        D_00096E34 = 4;
    }
    n = 999;
    r[0] = 0;
    r[1] = 0;
    do {
        VSync(0);
        if (func_00078E4C(0, &r[0], &r[1]))
            break;
    } while (--n != -1);
    return sel;
}
