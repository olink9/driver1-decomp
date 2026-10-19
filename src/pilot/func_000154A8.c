/* no Driver 2 counterpart (civ car creation) */
typedef struct {
    unsigned char id;   /* 0 */
    unsigned char f1;
    unsigned char f2;
    char p3, p4;
    unsigned char f5;
    char p6;
    unsigned char f7;
    unsigned char f8;   /* 8 */
    unsigned char f9;
    unsigned char fA;
    unsigned char fB;
    char pC[4];
    unsigned char x10;  /* 0x10 */
    unsigned char x11;
    short x12;
    char p14[3];
    unsigned char x17;
    int x18;
    int x1C;
    int x20;
    int x24;
    int x28;
    int p2C, p30;
    int x34;
    int x38;
    int x3C;
    int x40;
    int x44;
    int x48;
    int x4C;
} CIV;

typedef struct {
    unsigned char id;
    char p1;
    unsigned char x2;
    char p3[0xC8 - 3];
    int xC8;
    int padCC;
    int xD0;
    char padD4[0xF8 - 0xD4];
    int xF8;
    char padFC[0x18C - 0xFC];
} CAR;

typedef struct {
    int x0;
    int x4;
    int p8, pC;
    unsigned char x10;
    char p11[3];
} ROAD;

typedef struct {
    char p0[0xA];
    unsigned char xA;
    unsigned char xB;
    int pC;
} ROAD2;

typedef struct {
    int x;
    int z;
    int p8;
    short sC;
    short pE;
    short s10;
    short p12;
} INFO;

typedef struct { unsigned char v; char pad[7]; } B8;

extern CIV D_00098950[];
extern CAR D_0009B230[];
extern long long D_0009704C;
#define roads (*(ROAD **)&D_0009704C)
extern long long D_00097044;
#define roads2 (*(ROAD2 **)&D_00097044)
extern int D_00096780;
extern int D_0009677C;
extern long long D_0009D500;
extern long long D_0009D508;
extern long long D_0009B16C;
extern long long D_0009B174;
extern B8 D_00096898;
extern volatile long long D_000970D8;
extern long long D_00096834;
extern long long D_000970E8;
extern int D_00096FF4;
extern CAR *D_00096FC0;
extern int D_00096774;
extern int D_00096770;

extern int func_0004FFE8();
extern void func_00014FC4();
extern int func_00021820();
extern int func_00016F0C();
extern void func_00017C50();
extern void func_00020A64();
extern void func_0003EB0C();
extern void func_0001B720();
extern int func_00063B74();
extern unsigned char func_00015338();

static inline void ClearMem(unsigned char *p, int n)
{
    unsigned char *end = p + n;

    while (((int)p & 3) && p < end)
        *p++ = 0;
    while (p <= (unsigned char *)((int *)end - 1)) {
        *(int *)p = 0;
        p += 4;
    }
    while (p < end)
        *p++ = 0;
}

static inline int Nearest(int x, int z)
{
    CIV *o = D_00098950;
    int tx, tz, i, n, min, dx, dz, d;

    tx = x / 1500;
    tz = z / 1500;
    if (D_00096780 || D_0009677C)
        n = 24;
    else
        n = 22;
    min = 1024;
    for (i = 0; i < n; i++, o++) {
        if (o->f1) {
            d = o->x48 - tx;
            if (d < 0)
                d = tx - o->x48;
            dz = o->x4C - tz;
            if (dz < 0)
                dz = tz - o->x4C;
            d += dz;
            if (d < min)
                min = d;
        }
    }
    return min;
}

int func_000154A8(int id, int road, unsigned char flag, int x, int z)
{
    CIV *cp;
    CAR *car;
    int ty, mode;
    int dir, odir;
    int pos[2];
    INFO info;
    int pos2[3];
    CIV *o;
    int tx, tz, i, cnt, min, dx, dz;
    int one;

    ty = 0;
    mode = ty;
    cp = &D_00098950[id];
    car = &D_0009B230[id];
    ClearMem((unsigned char *)cp, 0x50);
    cp->id = id;
    cp->f1 = 0;
    if (road != -1)
        ty = roads[road].x10;
    ClearMem((unsigned char *)car, 0x18C);
    car->id = id + 1;

    if (road != -1) {
        if (ty == 1) {
            unsigned char odir;
            if (D_00096780 || D_0009677C) {
                dir = 3;
                if (roads[road].x0 * 1500 < *(int *)&D_0009D500)
                    dir = 1;
            } else {
                dir = 3;
                if (roads[road].x0 * 1500 < *(int *)&D_0009B16C)
                    dir = 1;
            }
            if (dir == 1)
                odir = 3;
            else
                odir = 1;
            switch (roads2[road].xB) {
            case 0x81:
                flag = 1;
                odir = flag;
                break;
            case 0x83:
                flag = 1;
                odir = 3;
                break;
            }
            if (flag == 1)
                cp->x24 = odir;
            else
                cp->x24 = dir;
        } else {
            if (D_00096780 || D_0009677C)
                dir = (-roads[road].x4 * 1500 < *(int *)&D_0009D508) << 1;
            else
                dir = (-roads[road].x4 * 1500 < *(int *)&D_0009B174) << 1;
            odir = (dir == 0) << 1;
            switch (roads2[road].xB) {
            case 0x80:
                flag = 1;
                odir = 0;
                break;
            case 0x82:
                flag = 1;
                odir = 2;
                break;
            }
            if (flag == 1)
                cp->x24 = odir;
            else
                cp->x24 = dir;
        }
    }

    cp->f7 = 0;
    cp->f8 = 0;
    cp->f2 = 0;
    cp->x28 = road;
    if (road == -1)
        mode = 1;
    else if (roads2[road].xA & 1) {
        if (!(func_0004FFE8(id) & 7))
            mode = 2;
    }

    if (mode) {
        if (road != -1) {
            func_00014FC4(road, cp->x24, cp->x34, 1, 0xFF, pos);
            if (cp->x24 & 1)
                z = pos[1];
            else
                x = pos[0];
            switch (cp->x24) {
            case 0:
                x += 470;
                break;
            case 2:
                x -= 470;
                break;
            case 1:
                z -= 470;
                break;
            case 3:
                z += 470;
                break;
            }
            if (Nearest(x, z) < 2) {
                func_00017C50(id);
                return 0;
            }
            func_00021820(x, z, cp->x24, &info);
            if (info.z < 4500) {
                func_00017C50(id);
                return 0;
            }
            if (info.x < 3000) {
                func_00017C50(id);
                return 0;
            }
        } else {
            if (Nearest(x, z) < 2) {
                func_00017C50(id);
                return 0;
            }
        }
        if (func_0004FFE8(id) & 3)
            cp->f7 = 1;
        cp->f1 = 1;
        cp->fA = 0;
        cp->fB = 0xFF;
        cp->x48 = D_0009B230[cp->id].xC8 / 1500;
        cp->x4C = D_0009B230[cp->id].xD0 / 1500;
    } else {
        cp->f1 = 2;
        cp->f8 = 1;
        if (func_00021820(x, z, cp->x24, &info)) {
            func_00017C50(id);
            return 0;
        }
        if (!func_00016F0C(cp, &info)) {
            func_00017C50(cp->id);
            return 0;
        }
        func_00014FC4(cp->x28, cp->x24, cp->x34, 0, 0xFF, pos2);
        if (cp->x24 & 1)
            z = pos2[1];
        else
            x = pos2[0];
        o = D_00098950;
        tx = x / 1500;
        tz = z / 1500;
        cnt = 0;
        min = 1024;
        for (i = 0; i < 22; i++, o++) {
            if (o->f1) {
                dx = o->x48 - tx >= 0 ? o->x48 - tx : tx - o->x48;
                dz = o->x4C - tz >= 0 ? o->x4C - tz : tz - o->x4C;
                if (dx < 5 && dz < 5) {
                    dx += dz;
                    if (dx < min)
                        min = dx;
                    cnt++;
                }
            }
        }
        pos2[2] = min;
        if (cnt >= 11 || (D_00096898.v != 3 && min < 6)) {
            func_00017C50(id);
            return 0;
        }
        cp->x18 = info.x;
        cp->x1C = info.z;
        cp->x20 = info.sC;
        cp->x12 = info.s10;
    }

    car->xC8 = x;
    car->xD0 = z;
    func_00020A64(car);
    if (road != -1)
        car->xF8 = cp->x24 << 10;
    else
        car->xF8 = flag << 10;
    cp->x40 = cp->x44 = 0x7FFFFFFF;
    cp->x38 = 0;
    cp->x3C = 0x82;
    if (mode)
        cp->x10 = *(int *)&D_000970D8 % 2 + 2;
    else
        cp->x10 = func_0004FFE8(id) % 2 + 2;
    if (cp->x10 == 3 && *(int *)&D_00096834 == 5)
        cp->x11 = 5;
    else
        cp->x11 = *(int *)&D_000970D8 % 4 + 1;
    cp->f5 = 0;
    if (D_00096FF4 == 1 && !mode && func_00063B74(cp, car)) {
        D_00096FF4 = 0;
        D_00096FC0 = car;
    }
    one = 1;
    if (cp->f5) {
        if (D_00096774 == one) {
            if (D_00096770 == one && (func_0004FFE8(id) & 1)) {
                cp->x10 = one;
                cp->x11 = one;
            } else {
                cp->x10 = 2;
                cp->x11 = 1;
            }
        } else {
            cp->x10 = one;
            cp->x11 = one;
        }
    }
    car->x2 = cp->x10;
    func_0003EB0C(id + 1, cp->x10);
    func_0001B720(car, 1);
    cp->x48 = D_0009B230[cp->id].xC8 / 1500;
    cp->x4C = D_0009B230[cp->id].xD0 / 1500;
    cp->f9 = func_00015338(cp);
    cp->x17 = 2;
    if (*(int *)&D_000970E8)
        cp->x17 = 0x42;
    return 1;
}
