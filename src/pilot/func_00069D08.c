/* func_00069D08 - no Driver 2 counterpart (cop/target proximity check) */

typedef struct { int vx, vy, vz, pad; } VEC;

typedef struct {
    char pad0[0xD0];
    int fD0;
    char pad1[0x104 - 0xD4];
    int f104;
    char pad2[0x114 - 0x108];
    int f114;
    char pad3[0x11C - 0x118];
    int f11C;
    int f120;
    char pad4[0x298 - 0x124];
    int f298;
    char pad5[0x2A0 - 0x29C];
    int f2A0;
    char pad6[0x2CC - 0x2A4];
    int f2CC;
    int f2D0;
} S1;

typedef struct {
    unsigned int f0;
    unsigned int f4;
    int f8;
    int fC;
    VEC *f10;
} S0;

extern int D_0009B16C[];
extern short rcossin_tbl[];
extern short D_0008EAAA[];
extern int D_00097794;
extern long long D_0009770C;
extern long long D_00097708;
extern long long D_00097738;
extern long long D_0009773C;
extern long long D_000975DC;
extern int car_data[];
extern int D_000C6F04[];
extern int D_0009B1AC[];
extern int ratan2(int, int);

int func_00069D08(S1 *a, S0 *b)
{
    int dist;
    VEC d;
    VEC *p;
    int dx, dz;
    int v;
    int t;
    int angle;

    dist = 0x7fffffff;
    if (b->fC & 1) {

    if (a->f2CC) {
        if (b->f8) {
            b->f8--;
        } else {
            a->f104 = 7;
            a->f298--;
            b->fC = 0;
        }
    }

    p = b->f10;
    dx = D_0009B16C[0] - p->vx;
    if (dx < 0)
        dx = p->vx - D_0009B16C[0];
    d.vx = dx;
    dz = D_0009B16C[2] - p->vz;
    if (dz < 0)
        dz = p->vz - D_0009B16C[2];
    d.vz = dz;

    angle = ratan2(dz, dx);

    if ((unsigned)((angle & 0x7ff) - 0x200) > 0x400) {
        t = D_0008EAAA[(angle & 0xfff) * 2];
        v = d.vx;
        if (v > 0x7ffff)
            t = ((v << 9) / t) << 3;
        else
            t = (v << 12) / t;
    } else {
        t = rcossin_tbl[(angle & 0xfff) * 2];
        v = d.vz;
        if (v > 0x7ffff)
            t = ((v << 9) / t) << 3;
        else
            t = (v << 12) / t;
    }
    dist = t;

    if (a->f104 != 2)
        return dist;

    if (dist < b->f4) {
        if (D_00097794 && *(int *)&D_0009770C > 0 && *(int *)&D_00097708 > 0x292) {
            if (!(b->fC & 4))
                a->f114 = 300;
            b->fC |= 4;
        } else {
            b->fC &= ~4;
            if ((a->f11C & 1) && a->f2A0 > a->f120) {
                if (!(b->fC & 2))
                    a->f114 = 300;
                b->fC |= 2;
                if (dist < b->f0) {
                    *(int *)&D_00097708 += 0x22;
                    *(int *)&D_00097738 = car_data[0xC8 / 4];
                    *(int *)&D_0009773C = car_data[0xD0 / 4];
                    if (*(int *)&D_00097708 > 0x1000)
                        *(int *)&D_00097708 = 0x1000;
                }
            } else {
                b->fC &= ~2;
                if (D_000C6F04[0x2C0 / 4] && D_0009B1AC[0] > D_000C6F04[0x2C4 / 4]) {
                    if (!(b->fC & 8))
                        a->f114 = 300;
                    b->fC |= 8;
                } else {
                    b->fC &= ~8;
                    if (dist < b->f0) {
                        a->f298--;
                        if (a->f2D0 == 2) {
                            a->fD0++;
                            a->f2A0 += 0x4000;
                        }
                        a->f104 = 0x7fff;
                        b->fC = 0;
                        *(int *)&D_000975DC = 0;
                        dist = 0x7fffffff;
                    }
                }
            }
        }
    } else {
        if (!D_00097794 || *(int *)&D_0009770C <= 0)
            b->fC &= ~4;
        if (!(a->f11C & 1) || a->f2A0 <= a->f120)
            b->fC &= ~2;
        if (!D_000C6F04[0x2C0 / 4] || D_0009B1AC[0] <= D_000C6F04[0x2C4 / 4])
            b->fC &= ~8;
    }
    }
    return dist;
}
