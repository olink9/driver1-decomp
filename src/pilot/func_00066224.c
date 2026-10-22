/* unknown (no Driver 2 counterpart): player car shake / bump physics, smoke on high damage */
typedef struct { int vx, vy, vz, pad; } VECTOR;

typedef struct {
    char pad0[0x40];
    int x;
    int pad44;
    int z;
    char pad4c[0xC8 - 0x4C];
    int px, py, pz;
    char padd4[0xF8 - 0xD4];
    int dir;
    char padfc[0x108 - 0xFC];
    int smoke;
    char pad10c[0x114 - 0x10C];
    int dmg[6];
    char pad12c[3];
    unsigned char flag12f;
    char pad130[0x17C - 0x130];
    unsigned short f17c;
    char pad17e[396 - 0x17E];
} CAR_DATA;

extern CAR_DATA car_data[];
extern int D_0009B1AC[];
extern short D_0008EAAA[];
extern int D_0008BC34[];
extern char D_0008BC70[];
extern long long D_00096878;
extern long long D_00096E7C;
extern long long CameraCnt;
extern int D_00097768;
extern int D_0009776C;
extern int D_00097778;
extern int D_0009777C;
extern int D_00097780;
extern VECTOR D_000C6CD8;

extern void Setup_Smoke();
extern void func_0004D8B0();
extern void Setup_Debris();
extern int func_000776E4();

static inline void bump(int v)
{
    int s;

    s = abs(v) >> 9;
    if (s >= 16 && *(int *)&D_00096878 == 0)
        D_00097768 += s - 8;
}

void func_00066224(void)
{
    VECTOR vel;
    VECTOR v;
    VECTOR pos;
    int ang;
    int dx, dz;
    int cs, sn;
    int aa, ab, d, lim;
    int r;
    int i;

    if (D_0009776C == 1)
        D_0009776C = 0;

    if (D_00097768 < 100) {
        ang = -car_data[0].dir & 0xfff;
        cs = D_0008EAAA[ang * 2];
        sn = D_0008EAAA[ang * 2 - 1];
        dx = (D_00097780 - car_data[0].x) >> 4;
        dz = (D_0009777C - car_data[0].z) >> 4;
        D_00097780 = car_data[0].x;
        D_0009777C = car_data[0].z;

        D_0008BC34[0] += (dx * cs + dz * sn + 0x800) >> 12;
        aa = abs(D_0008BC34[0]);
        D_0008BC34[2] += (dx * sn - dz * cs + 0x800) >> 12;
        ab = abs(D_0008BC34[2]);
        if (ab < aa)
            d = aa + (ab >> 1);
        else
            d = ab + (aa >> 1);
        d = 0x2000 - d;
        if (d < 0xE0)
            d = 0xE0;
        lim = (d << 5) >> 8;

        if (abs(D_0008BC34[0]) < lim)
            D_0008BC34[0] = 0;
        if (abs(D_0008BC34[2]) < lim)
            D_0008BC34[2] = 0;

        if (D_0008BC34[0]) {
            D_0008BC34[0] = (D_0008BC34[0] < 0) ? D_0008BC34[0] + lim : D_0008BC34[0] - lim;
        }
        if (D_0008BC34[2]) {
            if (D_0008BC34[2] < 0)
                D_0008BC34[2] += lim;
            else
                D_0008BC34[2] -= lim;
        }

        D_0008BC34[4] += D_0008BC34[0] / 256;
        D_0008BC34[6] += D_0008BC34[2] / 256;

        if (D_0008BC34[4] < 0) {
            bump(D_0008BC34[0]);
            D_0008BC34[4] = 0;
            D_0008BC34[0] = 0;
            D_0008BC34[2] /= 4;
        }
        if (D_0008BC34[6] < 0) {
            bump(D_0008BC34[2]);
            D_0008BC34[6] = 0;
            D_0008BC34[0] /= 4;
            D_0008BC34[2] = 0;
        }
        if (D_0008BC34[9] - D_0008BC34[11] < D_0008BC34[4]) {
            bump(D_0008BC34[0]);
            D_0008BC34[0] = 0;
            D_0008BC34[4] = D_0008BC34[9] - D_0008BC34[11];
            D_0008BC34[2] /= 4;
        }
        if (D_0008BC34[10] - D_0008BC34[12] < D_0008BC34[6]) {
            bump(D_0008BC34[2]);
            D_0008BC34[6] = D_0008BC34[10] - D_0008BC34[12];
            D_0008BC34[0] /= 4;
            D_0008BC34[2] = 0;
        }

        if (D_00097768 >= 100) {
            D_00097768 = 100;
            D_0009776C = 1;
        }

        if (D_00097768 > 60 && D_00097768 < 80)
            r = *(int *)&CameraCnt & 0xf;
        else if (D_00097768 >= 80 && D_00097768 < 90)
            r = *(int *)&CameraCnt & 0x7;
        else if (D_00097768 < 90)
            return;
        else
            r = *(int *)&CameraCnt & 0x3;

        if (r == 0)
            Setup_Smoke(&D_000C6CD8, 100, 500, 1, D_0009B1AC[0], D_0008BC70, 0);
    } else {
        if (D_00097778 == 0) {
            func_0004D8B0(2, 0, 0, D_000C6CD8.vx, D_000C6CD8.vy, D_000C6CD8.vz);
            car_data[0].dmg[0] = 0xfff;
            car_data[0].dmg[1] = 0xfff;
            car_data[0].dmg[2] = 0xfff;
            car_data[0].dmg[3] = 0xfff;
            car_data[0].dmg[4] = 0xfff;
            car_data[0].dmg[5] = 0xfff;
            car_data[0].flag12f = 1;
            car_data[0].f17c = *(unsigned short *)&D_00096E7C + 2;
        }
        pos.vx = car_data[0].px + func_000776E4() % 400 - 200;
        pos.vy = func_000776E4() % 400 - car_data[0].py - 200;
        pos.vz = car_data[0].pz + func_000776E4() % 400 - 200;

        if (!(*(int *)&CameraCnt & 1))
            Setup_Smoke(&pos, 100, 500, 1, car_data[0].smoke, D_0008BC70, 0);
        else
            Setup_Smoke(&D_000C6CD8, 50, 200, 4, 10, D_0008BC70, 0);

        if (D_00097778 < 9) {
            for (i = 0; i < 13; i++) {
                v.vx = pos.vx;
                v.vy = -pos.vy;
                v.vz = pos.vz;
                vel.vx = (func_000776E4() & 0x1ff) - 256;
                vel.vy = ((-66000 - (func_000776E4() & 0x7fff)) >> 10) - 600;
                vel.vz = (func_000776E4() & 0x1ff) - 256;
                Setup_Debris(&v, &vel, 5, 0);
            }
        }
        D_00097778++;
    }
}
