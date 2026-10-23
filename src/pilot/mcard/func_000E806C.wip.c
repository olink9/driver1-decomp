/* WIP 99.84 (fnscore): only diff left is in the case-4 verify (after the 0x110600 read-back): the target keeps
   `q = buf + 0x200` in a2, reads the header with `lw 0(a2)` and steps `addiu a2,a2,8`; here cse1 knows
   q == buf+0x200 and folds both (lw 0x200(s2), q = s2+0x208). Tried: q = buf; q += 0x200 / &buf[0x200] /
   int* header pointer / (S40 *)q + 1 / static inline header test / unconditional q += 8 / E7F5C as a static
   inline (frame grows to 0x50; fold stays). Everything else matches, including the goto-built compare loop
   (`for` gives bnez-back instead of the target's beqz-out + j-top). */
typedef struct { char name[0x18]; char title[0x24]; int size; unsigned char idx; unsigned char n; unsigned char f42; unsigned char f43; } E;
typedef struct { unsigned short s; unsigned char c2; char c3; void *p4; E e[15]; } H;
typedef struct {
    unsigned char b0, b1, b2;
    unsigned f24:1, f25:1, f26:2, f28:2, f30:1, f31:1;
    unsigned g0:1, g1:1, g2:1, g3:1;
} S40;
typedef struct { char c[32]; } CLUT;
typedef struct { char c[128]; } ICON;
typedef struct { CLUT clut; char pad[12]; ICON icon; char pad2[20]; } ICONSET;
typedef struct { int w[0x5E4 / 4]; } BIG;

extern H D_000EE930[];
extern unsigned char D_000EFA00[];
extern int D_000EF144;
extern char *D_000ED86C;
extern ICONSET D_000ED554, D_000ED614, D_000ED6D4;
extern S40 D_00097640;
extern int D_0009D8DC[];
extern int D_00096834, D_000967F4, D_00096808, D_00096854, D_00096858, D_00096870, D_00096784, D_0009681C, D_00096820;
extern char D_000E676C[], D_000E6790[], D_000E67B4[], D_000E67EC[];
extern void VSync();
extern int func_00078E4C();
extern int func_00078F68();
extern int func_000789B8();
extern int func_00078798();
extern int func_000E8CBC();
extern void func_000E8F68();
extern void func_000EAE3C();
extern void memset();

/* write save file `name` on card `port`: header (title, icon), then `size` bytes of `data`; verify by reading back */
int func_000E806C(int port, char *name, char *title, char *data, int size)
{
    int r[2];
    int n;
    int ret;
    int nb;
    int p16;
    int i;
    int err;
    char *save;
    char *dsave;

    r[0] = 0;
    r[1] = 0;
    n = 999;
    do {
        VSync(0);
        if (func_00078E4C(0, &r[0], &r[1]))
            break;
    } while (--n != -1);
    D_000EE930[port].c3 = 1;
    p16 = port << 4;
    nb = (size + 0x200) / 8192 + ((size + 0x200) % 8192 != 0);
    if (func_00078F68(p16, name, nb)) {
        func_000EAE3C(D_000E6790, 2);
        return 0;
    }
    if (func_000E8CBC(port, 1)) {
        func_000EAE3C(D_000E67B4, 2);
        return 0;
    }
    memset(D_000EFA00, 0, 0x200);
    D_000EFA00[0] = 'S';
    D_000EFA00[1] = 'C';
    D_000EFA00[2] = 0x11;
    D_000EFA00[3] = nb;
    func_000E8F68(title, D_000EFA00 + 4);
    if (D_000EF144 == 4) {
        *(CLUT *)(D_000EFA00 + 0x60) = D_000ED554.clut;
        *(ICON *)(D_000EFA00 + 0x80) = D_000ED554.icon;
    } else if (D_000EF144 == 3) {
        *(CLUT *)(D_000EFA00 + 0x60) = D_000ED614.clut;
        *(ICON *)(D_000EFA00 + 0x80) = D_000ED614.icon;
    } else {
        *(CLUT *)(D_000EFA00 + 0x60) = D_000ED6D4.clut;
        *(ICON *)(D_000EFA00 + 0x80) = D_000ED6D4.icon;
    }
    if (func_000789B8(p16, name, D_000EFA00, 0, 0x200) != 1) {
        func_000EAE3C(D_000E6790, 2);
        return 0;
    }
    n = 999;
    r[0] = 0;
    r[1] = 0;
    do {
        VSync(0);
        ret = func_00078E4C(0, &r[0], &r[1]);
        if (ret)
            break;
    } while (--n != -1);
    if (ret != 1) {
        func_000EAE3C(D_000E6790, 2);
        return 0;
    }
    if (func_000E8CBC(port, 1)) {
        func_000EAE3C(D_000E67B4, 2);
        return 0;
    }
    if (D_000EF144 == 3 || D_000EF144 == 6) {
        *(BIG *)D_000ED86C = *(BIG *)data;
    } else if (D_000EF144 == 4) {
        dsave = data;
        D_00097640.b0 = D_00096834;
        D_00097640.b1 = D_000967F4;
        D_00097640.b2 = D_00096808;
        D_00097640.f24 = D_00096854;
        D_00097640.f25 = D_00096858;
        D_00097640.f26 = D_00096784;
        D_00097640.f28 = D_00096870;
        D_00097640.f30 = D_0009681C;
        D_00097640.f31 = D_00096820;
        D_00097640.g0 = D_0009D8DC[0];
        D_00097640.g1 = D_0009D8DC[1];
        D_00097640.g2 = D_0009D8DC[2];
        D_00097640.g3 = D_0009D8DC[4];
        save = D_000ED86C;
        *(S40 *)save = D_00097640;
        D_000ED86C += 8;
        for (i = 0; i < 0x3444; i++)
            *D_000ED86C++ = *data++;
        D_000ED86C = save;
        data = dsave;
    }
    if (func_000789B8(p16, name, D_000ED86C, 0x200, size) != 1) {
        func_000EAE3C(D_000E6790, 2);
        return 0;
    }
    n = 999;
    r[0] = 0;
    r[1] = 0;
    do {
        VSync(0);
        ret = func_00078E4C(0, &r[0], &r[1]);
        if (ret)
            break;
    } while (--n != -1);
    if (ret != 1) {
        func_000EAE3C(D_000E6790, 2);
        return 0;
    }
    if (func_000E8CBC(port, 1)) {
        func_000EAE3C(D_000E67B4, 2);
        return 0;
    }
    if (D_000EF144 == 3 || D_000EF144 == 6) {
        unsigned char *buf;
        unsigned char *q;
        buf = (unsigned char *)0x10E600;
        n = 999;
        r[0] = 0;
        r[1] = 0;
        do {
            VSync(0);
            if (func_00078E4C(0, &r[0], &r[1]))
                break;
        } while (--n != -1);
        if (func_00078798(port << 4, name, buf, 0, size) == 1) {
            n = 999;
            r[0] = 0;
            r[1] = 0;
            do {
                VSync(0);
                ret = func_00078E4C(0, &r[0], &r[1]);
                if (ret)
                    break;
            } while (--n != -1);
            if (ret == 1)
                goto ok1;
        }
        func_000EAE3C(D_000E676C, 2);
ok1:
        q = buf + 0x200;
        i = err = 0;
        do {
            if (*q++ != *data++)
                err = 1;
            i++;
        } while (i < sizeof(BIG) && !err);
        if (err == 1)
            func_000EAE3C(D_000E6790, 2);
        else
            func_000EAE3C(D_000E67EC, 1);
    } else if (D_000EF144 == 4) {
        unsigned char *buf;
        unsigned char *q;
        err = 0;
        buf = (unsigned char *)0x110600;
        n = 999;
        r[0] = 0;
        r[1] = 0;
        do {
            VSync(0);
            if (func_00078E4C(0, &r[0], &r[1]))
                break;
        } while (--n != -1);
        if (func_00078798(port << 4, name, buf, 0, size) == 1) {
            n = 999;
            r[0] = 0;
            r[1] = 0;
            do {
                VSync(0);
                ret = func_00078E4C(0, &r[0], &r[1]);
                if (ret)
                    break;
            } while (--n != -1);
            if (ret == 1)
                goto ok2;
        }
        func_000EAE3C(D_000E676C, 2);
ok2:
        q = buf + 0x200;
        if ((*(int *)q & 0x3FFFFFF) == (*(int *)&D_00097640 & 0x3FFFFFF)) {
            q += 8;
            i = 0;
        top:
            if (err)
                goto done;
            if (*q != *data)
                err = 1;
            q++;
            data++;
            if (++i >= 0x3444)
                goto done;
            goto top;
        } else {
            err = 1;
        }
    done:
        if (err)
            func_000EAE3C(D_000E6790, 2);
        else
            func_000EAE3C(D_000E67EC, 1);
    }
    return 1;
}
