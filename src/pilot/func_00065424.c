/* FAKEMATCH: volatile casts on s->f34 (load) and s->f38 (store) in the f30>10 block only order the two accesses for sched1 (lw f34 before sw f38, so 0x16 gets v1); real source form unknown. */
typedef struct { int vx, vy, vz, pad; } VEC;

typedef struct {
    unsigned char f0;
    char pad1[3];
    short f4;
    char pad6[2];
    short f8;
    short fA;
    char padC[4];
    short f10;
    char pad12[2];
    short f14;
    char pad16[0x40 - 0x16];
    int f40;
    int pad44;
    int f48;
    char pad4C[0xC8 - 0x4C];
    int fC8;
    int padCC;
    int fD0;
    char padD4[0xF0 - 0xD4];
    int fF0;
    int fF4;
    int fF8;
    char padFC[0x104 - 0xFC];
    int f104;
    int f108;
    char pad10C[0x12E - 0x10C];
    unsigned char f12E;
    char pad12F[0x17C - 0x12F];
    short f17C;
    char pad17E[0x18C - 0x17E];
} CAR;

typedef struct {
    char pad0[0xC];
    int fC;
    int f10;
    char pad14[0x20 - 0x14];
    int f20;
    int f24;
    int f28;
    int f2C;
    int f30;
    int f34;
    int f38;
    int f3C;
    int f40;
    int pad44;
    int f48;
    int f4C;
    int f50;
    int f54;
    char pad58[0x70 - 0x58];
    short f70;
    char pad72[0x75C - 0x72];
    int f75C;
    int pad760;
    int f764;
    char pad768[0x76C - 0x768];
    VEC cur;
    VEC prev;
    int f78C;
    int f790;
    int f794;
    int f798;
    int pad79C;
    int f7A0;
    CAR *car;
} OBJ;

typedef struct {
    unsigned char f0, f1, f2, f3, f4, f5;
    char pad6[0xD - 6];
    unsigned char fD;
} COP;

typedef struct {
    unsigned char f0, f1, f2, f3, f4, f5;
    char pad[0x50 - 6];
} ENT;

typedef struct {
    int f0;
    int f4;
    char pad8[0x18 - 8];
    int f18;
    int f1C;
} S7C;

typedef struct {
    char pad0[8];
    int f8;
    int fC;
} MAPINFO;

extern int D_000976F8;
extern int D_00097764;
extern int D_00097744;
extern int D_00097710;
extern int D_00096E68;
extern int D_00097708;
extern int D_00097740;
typedef struct { int v; } I4;
extern I4 D_00096E60;
extern int D_00097700;
extern long long D_00097004;
extern long long D_00096870;
extern long long D_000970D8;
extern int D_000C4C8C[];
extern int D_0009B1AC[];
extern int D_0008BBD8[];
extern int D_0008BBE4[];
extern ENT D_00098950[];
extern CAR D_0009B0A4;
extern MAPINFO D_0009E714;
extern VEC D_000C4C6C;
extern S7C D_000C4C7C;
extern OBJ D_000C4690[];

extern int func_000660D8(int a, int b, int c, int d);
extern int func_00064C90(OBJ *s);
extern void func_00064DEC(OBJ *s, int d, VEC *v);
extern void func_0006466C(OBJ *s);
extern void func_0001B720(CAR *car, int a);

void func_00065424(COP *cp)
{
    OBJ *s;
    CAR *car;
    VEC target;
    int have;
    VEC cd;
    int pad[2]; /* unused: sets the frame layout */
    int fs[2];
    int px, pz;
    int dx, dz;
    int dist;
    int r;
    int dir;
    CAR *pl;
    int spd;
    unsigned int mode;
    int b, c, lim, d2, n2;
    int x, z, t, k, sc, d, n;

    s = &D_000C4690[cp->f5];
    car = s->car;
    if (D_000976F8 == 0) {
        car->fF0 = 0;
        return;
    }
    if ((s->f40 = car->f17C) > 0x4650 && D_000C4C8C[0] == 0) {
        s->f798 = 1;
        car->fF0 = 0;
        if (cp->fD == 0)
            cp->fD = 1;
        D_00098950[s->car->f0 - 1].f5 = 0;
        s->f78C = 0;
        if (--D_00097764 == 0 && D_00097744 < 0x100)
            D_00097744 = 0x100;
        cp->f1 = 2;
        cp->f2 = 1;
        return;
    }
    s->f798 = 0;
    s->f75C = car->fC8;
    s->f764 = car->fD0;
    s->f28 = car->f108;
    s->fC = car->fF8;
    pl = &D_0009B0A4;
    dir = (car->f104 + 0x800) >> 12;
    px = car->fC8;
    pz = car->fD0;
    dx = pl->f40 - car->f40;
    dz = pl->f48 - car->f48;
    cd.vx = car->f4;
    cd.vy = car->fA;
    have = 0;
    D_00097710 = 0;
    cd.vz = car->f10;
    if (car->f108 < 5 && s->f24 != 0)
        s->f30++;
    else
        s->f30 = 0;
    if (s->f30 > 10 && s->f38 == 0) {
        int q = *(volatile int *)&s->f34;
        *(volatile int *)&s->f38 = 0x16;
        s->f34 = q + 1;
        s->f24 = s->f24 < 0 ? 300 : -300;
        s->f30 = 0;
        s->f20 = ((s->f34 * 5 & 6) - 3) << 7;
    }
    dist = func_000660D8(px, pz, dx, dz);
    s->f7A0 = (dist < 2000 || (s->f790 == 0 && dist < 0x20D0));
    if (s->f38 > 0) {
        s->f38--;
    } else {
        if (dist < 12000) {
            int ax = (dx + 0x800) >> 12;
            t = ((px - pl->fC8) * ax - (pl->fD0 - pz) * ((dz + 0x800) >> 12)) / (ax * ax + ((dz + 0x800) >> 12) * ((dz + 0x800) >> 12) + 1);
            if (D_00096E68 == 0)
                t = t * 3 / 4;
            t = t < 0 ? 0 : (t > 60 ? 60 : t);
            D_000C4C6C.vx = D_0009B0A4.fC8 + ((D_0009B0A4.f40 * t + 0x800) >> 12);
            D_000C4C6C.vz = D_0009B0A4.fD0 + ((D_0009B0A4.f48 * t + 0x800) >> 12);
            if (D_00096E68 == 0) {
                D_000C4C6C.vx -= D_0009B0A4.f8 / 16;
                D_000C4C6C.vz -= D_0009B0A4.f14 / 16;
            } else {
                D_000C4C6C.vx += D_0009B0A4.f4 / 32;
                D_000C4C6C.vz += D_0009B0A4.f10 / 32;
            }
        } else {
            D_000C4C6C.vx = D_0009B0A4.fC8;
            D_000C4C6C.vz = D_0009B0A4.fD0;
        }
        if (s->f7A0) {
            target.vx = D_000C4C6C.vx;
            target.vz = D_000C4C6C.vz;
            s->f790 = 0;
            s->f48 = D_0009B0A4.fC8;
            s->f4C = D_0009B0A4.fD0;
            x = (s->f48 + D_0009E714.f8) / 1500;
            k = D_0009E714.fC + 750;
            z = (k - s->f4C) / 1500;
            s->f50 = x;
            s->f54 = z;
            have = 1;
        } else if (s->f790) {
            r = func_00064C90(s);
            func_00064DEC(s, r, &target);
            if (abs(((target.vx - px) * cd.vx + (target.vz - pz) * cd.vz) >> 12) > 0xD48)
                s->f790 = 0;
            else if (s->f790) {
                func_00064DEC(s, r + 1000 + car->f108 * 10, &target);
                have = 1;
            }
        }
    }
    if (!have)
        target = D_000C4C6C;
    target.vx = (s->cur.vx * 2 - s->prev.vx + target.vx) >> 1;
    target.vz = (s->cur.vz * 2 - s->prev.vz + target.vz) >> 1;
    s->prev = s->cur;
    s->cur = target;
    if (*(int *)&D_00097004 == 0)
        have = 0;
    if (have) {
        spd = (cd.vx * car->f40 + cd.vz * car->f48 + 0x800) >> 12;
        fs[0] = ((target.vx - px) * cd.vx + (target.vz - pz) * cd.vz) >> 12;
        fs[1] = ((px - target.vx) * cd.vz + (target.vz - pz) * cd.vx) >> 12;
        fs[0] -= spd / 198;
        if (fs[0] > 0) {
            if (fs[0] * 2 >= fs[1]) {
                mode = 3;
                if (fs[0] < fs[1] * 4 && s->f7A0)
                    mode = 1;
            } else
                mode = 0;
        } else {
            if (-(fs[0] * 2) >= fs[1]) {
                mode = 2;
                if (fs[1] * 4 > -fs[0] && s->f7A0)
                    mode = 1;
            } else
                mode = 0;
        }
        fs[0] += spd / 198;
        b = s->f24;
        if (dir > 0)
            b ^= 1;
        car->f12E = b != 0;
        switch (mode) {
        case 0:
            if (D_00097710)
                s->f24 = D_0008BBE4[*(int *)&D_00096870];
            else
                s->f24 = D_0008BBD8[*(int *)&D_00096870];
            if (s->f10 <= D_000C4C7C.f1C)
                s->f24 = (s->f24 * (D_00097740 + ((D_00097708 * D_000C4C7C.f4) >> 12))) >> 12;
            else
                s->f24 = (s->f24 * (D_000C4C7C.f18 + ((D_00097708 * D_000C4C7C.f4) >> 12))) >> 12;
            if (D_00096E60.v && s->f7A0) {
                int base = D_0009B1AC[0] + 10;
                if (dist > 4000)
                    s->f24 = base + (s->f24 - base) * (dist - 4000) / 4000;
                else
                    s->f24 = base;
            }
            s->f20 = (fs[0] << 9) / fs[1];
            break;
        case 1:
            s->f24 = -100;
            if ((fs[0] ^ dir) < 0)
                c = -0x11C;
            else
                c = 0x11C;
            s->f20 = c;
            break;
        case 2:
        case 3:
            c = 0x6E;
            if (D_00096E68 && s->f7A0 && s->f24 < 0)
                c = -0x6E;
            s->f24 = c;
            if ((fs[0] ^ dir) < 0)
                c = -0x200;
            else
                c = 0x200;
            s->f20 = c;
            break;
        }
        car->f12E = 1;
    } else if (s->f38 <= 0) {
        s->f24 = 0;
        s->f20 = 0;
    }
    if (s->f3C)
        s->f3C--;
    if (dist < 0x1000 && s->f24 > 0 && D_00096E68 == 0) {
        lim = D_0009B1AC[0] - 20;
        if (lim < 20)
            lim = 20;
        s->f24 = (dist * s->f24 + (0x1000 - dist) * lim + 0x800) >> 12;
    }
    n = *(int *)&D_00096870;
    sc = (n + 8) << 10;
    d = s->f24 - dir;
    if (D_00097710)
        sc += (n + 4) << 10;
    sc = (sc * (D_00097700 + ((D_00097708 * D_000C4C7C.f0) >> 12))) >> 12;
    if (d < -50)
        car->fF0 = -sc;
    else if (d > 50)
        car->fF0 = sc;
    else
        car->fF0 = d * sc / 50;
    if (car->f108 < 0x30)
        car->fF0 = (car->fF0 * (car->f108 + 0x50)) >> 7;
    if (car->f12E == 1)
        car->fF0 >>= 1;
    d2 = s->f20 - car->fF4;
    d2 = d2 < -200 ? -200 : (d2 > 200 ? 200 : d2);
    car->fF4 += d2;
    n2 = *(int *)&D_000970D8;
    s->f2C--;
    if (n2 % 4 == cp->f5 - 1) {
        if (s->f2C < 0)
            s->f790 = 0;
        func_0006466C(s);
        if (s->f790 && s->f794 == 1) {
            car->fF8 = s->f70;
            func_0001B720(car, 0);
            s->f794 = 0;
            s->f3C = 0;
        }
    }
}
