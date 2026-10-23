/* FAKEMATCH: pad0[0x18] is an unused local that only puts the poll array at sp+0x28 (frame 0x70); the real
   24-byte object is unknown (an inlined poll helper moves r but gives only 16 bytes) */
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
typedef struct {
    P_TAG tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    short x1, y1;
    short x2, y2;
    short x3, y3;
} POLY_F4;
typedef struct { P_TAG tag; int w[9]; } POLY_FT4;
typedef struct { char *primptr; P_TAG *ot; int f8; int id; } DB;
typedef struct { unsigned char a, b; } KEYMAP;
extern DB *current;
extern DB *D_00097524;
extern POLY_F4 D_000EF9A0;
extern POLY_FT4 D_000EF8D0[4];
extern KEYMAP D_000ED880[15];
extern KEYMAP D_000ED8A0[5];
extern unsigned char D_000BF6EC[];
extern int D_00096E34;
extern unsigned short D_0009752C;
extern unsigned short D_00097538;
extern int D_000EF144;
extern int D_000ED7D4;
extern int D_000ED7D8;
extern char D_000E6894[];
extern char D_000E68E0[];
extern char D_000E6930[];
extern char D_000E69E0[];
extern char D_000E6A34[];
extern char D_000E6BD8[];
extern int VSync();
extern int func_00078E4C();
extern void func_00077ED8();
extern void func_000E8DD4();
extern void func_000EAE3C();
extern void func_000EC550();
extern void func_0007457C();
extern int func_0007EC88(); /* strlen */
extern int func_0005D86C();
extern void func_000514E0();
extern void func_000EB5B8();
extern void func_000E90D8();
extern void SetTextColour();
extern int PrintString();
extern void DrawSync();
extern int sprintf();

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

/* on-screen keyboard: edit the save title; returns 1 on END, 0 on cancel */
int func_000EB728(int port, char *name)
{
    char pad0[0x18]; /* FAKEMATCH: unused, sizes the frame */
    int r[2];
    int n;
    int ret;
    int len;
    int cur;
    int rpt;
    int done;
    int blink;
    unsigned char blinkon;
    int i;
    int k;
    int found;
    unsigned short pad;
    POLY_F4 *p;

    rpt = 0;
    done = rpt;
    blinkon = rpt;
    cur = 0x21;
    blink = rpt;
    ret = 0;
    r[0] = 0;
    r[1] = 0;
    n = 999;
    do {
        VSync(0);
        if (func_00078E4C(0, &r[0], &r[1]))
            break;
    } while (--n != -1);
    func_000EC550(0x13, 0x11);
    func_000EC550(0x14, 0x12);
    func_0007457C(current->ot, 0x1080);
    func_0007457C(D_00097524->ot, 0x1080);
    len = func_0007EC88(name);
    p = &D_000EF9A0;
    ((P_TAG *)p)->len = 5;
    p->code = 0x28;
    p->r0 = 0;
    p->g0 = 0;
    p->b0 = 0;
    p->x0 = 0x48;
    p->y0 = 0x5C;
    p->x1 = 0xF8;
    p->y1 = 0x5C;
    p->x2 = 0x48;
    p->y2 = 0x94;
    p->x3 = 0xF8;
    p->y3 = 0x94;
    p->code = 0x29;
    if (len == 0) {
        switch (D_000EF144) {
        case 6:
            sprintf(name, D_000E68E0);
            break;
        case 3:
            sprintf(name, D_000E6894);
            break;
        case 4:
            sprintf(name, D_000E6930);
            break;
        case 5:
            sprintf(name, D_000E69E0);
            break;
        }
        len += 18;
    }
    while (!done) {
        if (check_card(port, 0)) {
            func_000EAE3C(D_000E6A34, 2);
            return 0;
        }
        if (rpt > 0)
            rpt--;
        func_000514E0();
        pad = D_0009752C;
        if (pad & 0x4000) {
            if (rpt <= 0) {
                found = 0;
                for (i = 0; i < 15; i++) {
                    if (cur == D_000ED880[i].b) {
                        if (cur < 0x74)
                            cur = 0x80;
                        else
                            cur = 0x81;
                        found = 1;
                    }
                    rpt = 8;
                }
                if (!found) {
                    k = 0;
                    if (cur == 0x80) {
                        cur = 0x21;
                    } else if (cur == 0x81) {
                        cur = 0x34;
                    } else {
                        do {
                            k++;
                            cur++;
                            do {
                                if (func_0005D86C(D_000BF6EC[cur]))
                                    goto dn_ok;
                                cur++;
                            } while (cur < 0x81);
                            cur = 0x81;
                        dn_ok:
                            ;
                        } while (k < 15);
                    }
                    rpt = 8;
                }
            }
        } else if (pad & 0x1000) {
            if (rpt <= 0) {
                found = 0;
                for (i = 0; i < 15; i++) {
                    if (cur == D_000ED880[i].a) {
                        cur = D_000ED880[i].b;
                        found = 1;
                    }
                    rpt = 8;
                }
                if (!found) {
                    k = 15;
                    if (cur == 0x80) {
                        cur = 0x6C;
                    } else if (cur == 0x81) {
                        cur = 0x7A;
                    } else {
                        do {
                            k--;
                            cur--;
                            do {
                                if (func_0005D86C(D_000BF6EC[cur]))
                                    goto up_ok;
                                cur--;
                            } while (cur >= 0);
                            cur = 0x81;
                        up_ok:
                            ;
                        } while (k > 0);
                    }
                    rpt = 8;
                }
            }
        } else if (pad & 0x8000) {
            if (rpt <= 0) {
                found = 0;
                for (i = 0; i < 5; i++) {
                    if (cur == D_000ED8A0[i].a) {
                        cur = D_000ED8A0[i].b;
                        found = 1;
                    }
                    rpt = 8;
                }
                if (!found) {
                    if (cur == 0x81) {
                        cur = 0x80;
                    } else if (cur == 0x80) {
                        cur = 0x81;
                    } else {
                        cur--;
                        if (cur < 0)
                            cur = 0x81;
                        if (cur < 0x80) {
                            do {
                                if (func_0005D86C(D_000BF6EC[cur]))
                                    goto lt_ok;
                                cur--;
                            } while (cur >= 0);
                            cur = 0x81;
                        }
                    }
                lt_ok:
                    rpt = 8;
                }
            }
        } else if (pad & 0x2000) {
            if (rpt <= 0) {
                found = 0;
                for (i = 0; i < 5; i++) {
                    if (cur == D_000ED8A0[i].b) {
                        cur = D_000ED8A0[i].a;
                        found = 1;
                    }
                    rpt = 8;
                }
                if (!found) {
                    if (cur == 0x81) {
                        cur = 0x80;
                    } else if (cur == 0x80) {
                        cur = 0x81;
                    } else {
                        cur++;
                        if (cur > 0x81)
                            cur = 0;
                        if (cur < 0x80) {
                            do {
                                if (func_0005D86C(D_000BF6EC[cur]))
                                    goto rt_ok;
                                cur++;
                            } while (cur < 0x81);
                            cur = 0x81;
                        }
                    }
                rt_ok:
                    rpt = 8;
                }
            }
        } else {
            rpt = 0;
        }
        if (D_00097538 & 0x40) {
            if (cur == 0x80) {
                if (len > 18) {
                    name[len] = 0;
                    len--;
                }
            } else if (cur == 0x81) {
                if (len > 18) {
                    ret = 1;
                    done = 1;
                }
            } else if (len < 31) {
                name[len] = cur;
                len++;
                name[len] = 0;
            }
        } else if (D_00097538 & 0x10) {
            break;
        }
        D_000EF8D0[current->id].tag.addr = current->ot[12].addr;
        current->ot[12].addr = (unsigned int)&D_000EF8D0[current->id];
        (D_000EF8D0 + 2)[current->id].tag.addr = current->ot[12].addr;
        current->ot[12].addr = (unsigned int)&(D_000EF8D0 + 2)[current->id];
        func_000EB5B8(cur);
        blink++;
        if (blink > 32) {
            blink = 0;
            blinkon ^= 1;
        }
        if (len < 32) {
            if (blinkon == 1)
                name[len] = 0x2D;
            else
                name[len] = 0;
            name[len + 1] = 0;
        }
        D_00096E34 = 1;
        SetTextColour(0x80, 0x80, 0x80);
        PrintString(D_000E6BD8, 0x20, 0x4A);
        SetTextColour(0xD6, 0, 8);
        PrintString(name, 0x20, 0xC6);
        SetTextColour(0xFF, 0xFF, 0xFF);
        D_00096E34 = 4;
        func_000E90D8();
    }
    name[len] = 0;
    r[0] = 0;
    r[1] = 0;
    n = 999;
    do {
        VSync(0);
        if (func_00078E4C(0, &r[0], &r[1]))
            break;
    } while (--n != -1);
    SetTextColour(0xD6, 0x9D, 0);
    DrawSync(0);
    func_000EC550(0xE, 0x11);
    func_000EC550(0xF, 0x12);
    func_0007457C(current->ot, 0x1080);
    func_0007457C(D_00097524->ot, 0x1080);
    n = 0;
    D_000ED7D4 = VSync(-1) % 1024;
    while (D_000ED7D4 == D_000ED7D8) {
        D_000ED7D4 = (VSync(-1) + n) % 1024;
        n++;
    }
    return ret;
}
