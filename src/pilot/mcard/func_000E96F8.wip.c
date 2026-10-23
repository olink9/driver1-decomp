/* WIP 99.76: control flow, calls, schedule all match; only s4/s5 are swapped (inlined func_000E7F5C `port` copy vs the
   `dst` param). global-alloc priority: dst 9 refs/267 insns = 1011, port 2 refs/20 = 1000 -> dst picks s4 first. Target
   needs port in (1011, 1176) (c = D_000ED86C copy is 1176) or dst <= 1000. Tried: all 24 helper param orders (port after
   b fixes regs but schedules the copy after b's address), caller-local card before/after the clamp, `port <<= 4` (4000,
   overshoots to s1), loop via separate `p` (p gets t3), dst[i], ?: clamp, strncmp inline helper, nested/early returns.
   (ov1-wip M) Exact numbers from .greg: dst 81 = 9 refs/267 -> 1011, port 236 = 2/20 -> 1000 (int compare, tie -> lower
   pseudo). Target sched order of port's range is identical to ours (copy is first in the post-clamp block, reorg puts it
   in the beqz slot), so port's 20 is probably right and dst must be <1000 but >size (82: 4/109 -> 734): 8 refs at ~267,
   or 9 refs at >=271 insns. dst refs = param copy 1 + 2 struct-copy pointer copies + loop sb (2) + loop addiu (4).
   Tried: copy loop as goto loop (dst 6 refs -> drops below size, +1 insn), dst[i] (no giv reduction, worse), helper
   param order (b, port) (copy lands after b's addiu), int/unsigned/void variants of the helper params/return. */
typedef struct { char fname[0x18]; char title[0x24]; unsigned int size; unsigned char idx; unsigned char n; unsigned char f42; unsigned char f43; } E;
typedef struct { unsigned short s; unsigned char c2; char c3; void *p4; E e[15]; } H;
typedef struct { unsigned char data[0x5E0]; int sum; } SAVE;
typedef struct { int a, b; } S8;
extern H D_000EE930[];
extern int D_000EF13C;
extern int D_000EF140;
extern int D_000EF144;
extern unsigned char *D_000ED86C;
extern int D_000967F0;
extern S8 D_00097640;
extern char D_000E676C[];
extern char D_000E6894[], D_000E68A8[], D_000E68E0[], D_000E68F4[], D_000E6930[], D_000E6944[];
extern char D_000E697C[], D_000E69B0[], D_000E69E0[], D_000E69F4[];
extern void VSync();
extern int func_00078E4C();
extern int func_00078798();
extern int func_0007B218(); /* strncmp */
extern void func_000EAE3C();
extern void func_000EC02C();
extern int func_000EC4DC();

/* inlined copy of func_000E7F5C (read a file from the card) */
static inline int load(int port, void *b, void *c, int d)
{
    int r[2];
    int n;
    int ret;

    r[0] = 0;
    r[1] = 0;
    n = 999;
    do {
        VSync(0);
        if (func_00078E4C(0, &r[0], &r[1]))
            break;
    } while (--n != -1);
    if (func_00078798(port << 4, b, c, 0, d) == 1) {
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
            return 1;
    }
    func_000EAE3C(D_000E676C, 2);
    return 0;
}

/* load screen input: on X, check the selected save belongs to the current mode, read it and copy it into dst */
int func_000E96F8(int pad, unsigned char *dst, unsigned int size)
{
    int ok;
    int i;
    unsigned char f;

    func_000EC02C(pad);
    if (pad & 0x10)
        return 1;
    if (pad & 0x40) {
        f = D_000EE930[D_000EF13C].e[D_000EF140].f43;
        if (!(f & 1))
            return 0;
        if (f & 2) {
            ok = 1;
            switch (D_000EF144) {
            case 0:
                if (func_0007B218(D_000EE930[D_000EF13C].e[D_000EF140].title, D_000E6894, 0x12)) {
                    func_000EAE3C(D_000E68A8, 2);
                    ok = 0;
                }
                break;
            case 2:
                if (func_0007B218(D_000EE930[D_000EF13C].e[D_000EF140].title, D_000E68E0, 0x12)) {
                    func_000EAE3C(D_000E68F4, 2);
                    ok = 0;
                }
                break;
            case 1:
                if (func_0007B218(D_000EE930[D_000EF13C].e[D_000EF140].title, D_000E6930, 0x12)) {
                    func_000EAE3C(D_000E6944, 2);
                    ok = 0;
                }
                break;
            }
            if (ok) {
                func_000EAE3C(D_000E697C, 0);
                if (D_000EE930[D_000EF13C].e[D_000EF140].size < size)
                    size = D_000EE930[D_000EF13C].e[D_000EF140].size;
                load(D_000EF13C, &D_000EE930[D_000EF13C].e[D_000EF140], D_000ED86C, size);
                if (D_000EF144 == 0) {
                    if (func_0007B218(D_000EE930[D_000EF13C].e[D_000EF140].title, D_000E6894, 0x12) == 0) {
                        D_000967F0 = 1;
                        D_000ED86C += 0x200;
                        if (func_000EC4DC(D_000ED86C, 0x5E0) != ((SAVE *)D_000ED86C)->sum) {
                            func_000EAE3C(D_000E676C, 2);
                            D_000967F0 = 0;
                            return 0;
                        }
                        *(SAVE *)dst = *(SAVE *)D_000ED86C;
                        func_000EAE3C(D_000E69B0, 1);
                        return 1;
                    } else {
                        func_000EAE3C(D_000E68A8, 2);
                        return 0;
                    }
                } else if (D_000EF144 == 2) {
                    if (func_0007B218(D_000EE930[D_000EF13C].e[D_000EF140].title, D_000E68E0, 0x12) == 0) {
                        D_000967F0 = 1;
                        D_000ED86C += 0x200;
                        if (func_000EC4DC(D_000ED86C, 0x5E0) != ((SAVE *)D_000ED86C)->sum) {
                            func_000EAE3C(D_000E676C, 2);
                            D_000967F0 = 0;
                            return 0;
                        }
                        *(SAVE *)dst = *(SAVE *)D_000ED86C;
                        func_000EAE3C(D_000E69B0, 1);
                        return 1;
                    } else {
                        func_000EAE3C(D_000E68F4, 2);
                        return 0;
                    }
                } else if (D_000EF144 == 1) {
                    if (func_0007B218(D_000EE930[D_000EF13C].e[D_000EF140].title, D_000E6930, 0x12) == 0) {
                        D_000967F0 = 1;
                        D_000ED86C += 0x200;
                        D_00097640 = *(S8 *)D_000ED86C;
                        D_000ED86C += 8;
                        for (i = 0; i < 0x3444; i++) {
                            *dst++ = *D_000ED86C;
                            D_000ED86C++;
                        }
                        func_000EAE3C(D_000E69B0, 1);
                        return 1;
                    } else {
                        func_000EAE3C(D_000E6944, 2);
                        return 0;
                    }
                } else {
                    func_0007B218(D_000EE930[D_000EF13C].e[D_000EF140].title, D_000E69E0, 0x12);
                    return 0;
                }
            }
        } else {
            func_000EAE3C(D_000E69F4, 2);
        }
    }
    return 0;
}
