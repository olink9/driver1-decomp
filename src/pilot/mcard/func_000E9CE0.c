typedef struct { char fname[0x18]; char title[0x24]; int size; unsigned char idx; unsigned char n; unsigned char f42; unsigned char f43; } E;
typedef struct { unsigned short s; unsigned char c2; char c3; void *p4; E e[15]; } H;
extern H D_000EE930[];
extern unsigned short D_00097538;
extern int D_000EF13C;
extern int D_000EF140;
extern int D_000EF144;
extern int D_000ED7D4;
extern char D_000E6728[];
extern char D_000E6828[];
extern char D_000E6A34[];
extern char D_000E6A54[];
extern char D_000E6AA8[];
extern char D_000E6AFC[];
extern char D_000E6B2C[];
extern char D_000E6B40[];
extern char D_000E6B70[];
extern char D_000E6B78[];
extern void VSync();
extern int func_00078E4C();
extern void func_00077ED8();
extern void func_00078114();
extern int func_0007932C();
extern int func_0007B218(); /* strncmp */
extern int func_0007B228(); /* strcmp */
extern void func_000E8DD4();
extern void func_000E7B2C();
extern void func_000EAE3C();
extern void func_000EC02C();
extern int func_000EB1F8();
extern int func_000EB728();
extern int func_000E8B80();
extern int func_000E806C();
extern int sprintf();
extern char *strcpy();

/* card unformatted? (b[1] == 0 after a retry) */
static inline int card_unformatted(int port)
{
    int a[2];
    int b[2];
    int n;
    int p16 = port << 4;

    b[0] = 0;
    b[1] = 0;
    a[0] = 0;
    a[1] = 0;
    n = 999;
    do {
        VSync(0);
        if (func_00078E4C(0, &a[0], &a[1]))
            break;
    } while (--n != -1);
    func_00077ED8(p16);
    func_00078E4C(0, &b[0], &b[1]);
    if (b[1] != 0)
        return 0;
    func_00078114(p16);
    func_00078E4C(0, &b[0], &b[1]);
    return b[1] == 0;
}

/* func_000E8CBC inlined. Here its poll is top-tested (n = 1000; while (--n != -1)): jump.c's exit-test copy marks
   the -1 REG_LOOP_TEST_P, so loop.c keeps it inside the retry loop (s3) instead of merging it with the first poll's
   -1 (s5); same 1000 polls as the do-while form */
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
        n = 1000;
        while (--n != -1) {
            VSync(0);
            if (func_00078E4C(0, &a[0], &a[1]))
                break;
        }
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

/* format the card */
static inline int format_card(int port)
{
    int r[2];
    int n;

    n = 999;
    r[0] = 0;
    r[1] = 0;
    do {
        VSync(0);
        if (func_00078E4C(0, &r[0], &r[1]))
            break;
    } while (--n != -1);
    D_000EE930[port].c3 = 1;
    if (func_0007932C(port << 4)) {
        func_000EAE3C(D_000E6828, 2);
        return 0;
    }
    func_000E8DD4(port);
    func_000E7B2C(port);
    D_000EE930[port].p4 = 0;
    return 1;
}

static inline int title_used(char *title)
{
    int i;
    int port = D_000EF13C;
    int cur = D_000EF140;

    for (i = 0; i < 15; i++) {
        if (i != cur && (D_000EE930[port].e[i].f43 & 2)
            && func_0007B228(D_000EE930[port].e[i].title, title) == 0)
            return 1;
    }
    return 0;
}

static inline int name_used(char *name)
{
    int i;
    int port = D_000EF13C;

    for (i = 0; i < 15; i++) {
        if ((D_000EE930[port].e[i].f43 & 1)
            && func_0007B228(D_000EE930[port].e[i].fname, name) == 0)
            return 1;
    }
    return 0;
}

/* save to the selected slot: format / overwrite checks, title entry (func_000EB728), unique file name, write
   (func_000E806C). The second loop reuses i on purpose (target keeps both counters in s4). */
int func_000E9CE0(int pad, char *buf, unsigned int size)
{
    char fname[0x18];
    char title[0x28];
    int i;
    int unf;
    int ovw;
    int same;

    func_000EC02C(pad);
    if (D_00097538 & 0x10)
        return 1;
    if (!(pad & 0x40))
        goto ret0;
    title[0] = 0;
    fname[0] = 0;
    ovw = 0;
    same = 0;
    if (D_000EE930[D_000EF13C].s & 4) {
        unf = 0;
        for (i = 0; i < 2; i++) {
            if (card_unformatted(D_000EF13C)) {
                unf = 1;
                break;
            }
            if (check_card(D_000EF13C, 1))
                goto removed;
        }
        if (!unf) {
            if (D_000EF13C == 0) {
                if (!func_000EB1F8(D_000EF13C, D_000E6A54))
                    return 0;
            } else {
                if (!func_000EB1F8(D_000EF13C, D_000E6AA8))
                    return 0;
            }
            if (!format_card(D_000EF13C))
                return 0;
        }
    } else if (D_000EE930[D_000EF13C].e[D_000EF140].f43 & 1) {
        if (D_000EF144 == 4 && D_000EE930[D_000EF13C].e[D_000EF140].n < 2 && D_000EE930[D_000EF13C].c2 == 0) {
        full:
            func_000EAE3C(D_000E6AFC, 12);
            return 0;
        }
        if (!func_000EB1F8(D_000EF13C, D_000E6B2C))
            goto ret0;
        ovw = 1;
        if (func_0007B218(D_000EE930[D_000EF13C].e[D_000EF140].fname, D_000E6728, 12) == 0) {
            strcpy(fname, D_000EE930[D_000EF13C].e[D_000EF140].fname);
            same = ovw;
        }
    }
    if (!ovw) {
        if (D_000EE930[D_000EF13C].s != 2)
            return 0;
        {
            unsigned int c2 = D_000EE930[D_000EF13C].c2;
            unsigned int n = size / 8192;
            if ((size % 8192) ? c2 < n + 1 : c2 < n)
                goto full;
        }
    }
    for (;;) {
        if (!func_000EB728(D_000EF13C, title))
            goto ret0;
        if (!title_used(title))
            break;
        func_000EAE3C(D_000E6B40, 2);
    }
    if (!same) {
        for (i = 0; i < 15; i++) {
            sprintf(fname, D_000E6B70, D_000E6728, i + 1, D_000ED7D4);
            if (!name_used(fname))
                break;
        }
    }
    if (check_card(D_000EF13C, 1)) {
    removed:
        func_000EAE3C(D_000E6A34, 2);
        return 0;
    }
    if (ovw) {
        if (!func_000E8B80(D_000EF13C, D_000EF140))
            goto ret0;
    }
    func_000EAE3C(D_000E6B78, 0);
    /* both outcomes return 0 in the target (beqz v1 -> v0 = 0, else fall into v0 = 0) */
    if (func_000E806C(D_000EF13C, fname, title, buf, size) == 0)
        return 0;
ret0:
    return 0;
}
