/* FAKEMATCH: unused char pad[80] only sizes the 0x70 frame (target reserves 0x50 B of locals it never touches). */
typedef struct { int x0, pad0, z0, pad1, x1, pad2, z1, pad3; char rest[0x18]; } PATHSEG;
typedef struct { char a[0xC8]; int x, y, z; } CAR;

extern int pauseflag;
extern int D_00096874;
extern CAR car_data[];
extern int D_001E42B0;
extern int D_001E35B0;
extern int D_001E3750;
extern PATHSEG D_001E3758[];
extern unsigned char D_001E42B8;
extern unsigned char D_001E4020;
extern int D_001E40A8;
extern int D_001E4098;
extern int D_001E4018;
extern int D_001E4298;
extern char D_001E3508[];
extern int sprintf();
extern void func_001E2CC8();

void func_001E2AB4(void)
{
    char pad[80];
    int best;
    int i;
    int cx, cz, d, dx, dz;
    PATHSEG *p;

    best = 99999999;
    if (pauseflag != 0 || D_00096874 != 0)
        return;
    if (D_001E42B0 > 0)
        D_001E42B0--;
    if (D_001E35B0 != 0)
        i = 40;
    else {
        if (D_001E3750 == 0)
            return;
        i = D_001E3750 - 1;
    }
    cx = car_data[0].x;
    cz = car_data[0].z;
    while (--i != -1) {
        p = &D_001E3758[i];
        dx = cx - p->x0;
        if (dx < 0)
            dx = -dx;
        dz = cz - p->z0;
        if (dz < 0)
            dz = -dz;
        d = dx + dz;
        dx = cx - p->x1;
        if (dx < 0)
            dx = -dx;
        dz = cz - p->z1;
        if (dz < 0)
            dz = -dz;
        d += dx + dz;
        if (d < best) {
            best = d;
            D_001E42B8 = i;
        }
    }
    func_001E2CC8();
    if (best < 800 && D_001E42B8 != D_001E4020) {
        D_001E4020 = D_001E42B8;
        if (D_001E40A8 == 0) {
            sprintf(D_001E3508, "Good.  Keep on the path");
            D_001E4098 = 25;
            D_001E40A8 = -1;
        }
        D_001E4018++;
    }
    if (best > 3500) {
        sprintf(D_001E3508, "Don't stray from the path!");
        D_001E4098 = 25;
        if (D_001E42B0 == 0) {
            D_001E42B0 = 15;
            D_001E4298++;
        }
    }
}
