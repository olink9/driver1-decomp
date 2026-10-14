typedef struct {
    char pad0[0xC8];
    int x;          /* 0xC8 */
    char pad1[4];
    int z;          /* 0xD0 */
    char pad2[0xF8 - 0xD4];
    int ang;        /* 0xF8 */
    char pad3[0x108 - 0xFC];
    int speed;      /* 0x108 */
    char pad4[396 - 0x10C];
} CAR;
typedef struct {
    char pad0[0xC];
    int ang;        /* 0x0C */
    char pad1[0x28 - 0x10];
    int speed;      /* 0x28 */
    char pad2[0x3C - 0x2C];
    int timer;      /* 0x3C */
    char pad3[0x75C - 0x40];
    int x;          /* 0x75C */
    char pad4[4];
    int z;          /* 0x764 */
    char pad5[0x798 - 0x768];
    int state;      /* 0x798 */
    char pad6[1960 - 0x79C];
} COP;

extern CAR D_0009B230[];
extern COP D_000C4690[];
extern short D_0008EAA8[];
extern int ratan2();
#define ABS(x) ((x) < 0 ? -(x) : (x))

int func_00063E54(COP *cop, unsigned char *p)
{
    int x1, z1, x2, z2;
    int dx, dz;
    int rdx, rdz;
    int n;
    int d;
    int ang;
    int cang;
    int diff;
    int steps;
    int speed, carspeed;
    int cdx, cdz, kdx, kdz;
    CAR *car;

    x1 = cop->x;
    z1 = cop->z;
    car = &D_0009B230[p[0]];
    x2 = car->x;
    z2 = car->z;
    rdx = x2 - x1;
    rdz = z2 - z1;
    dx = abs(rdx);
    dz = abs(rdz);
    if (dz < dx)
        d = dx + dz / 2;
    else
        d = dz + dx / 2;
    if (d >= 15000)
        return 0;
    if (p[5] != 0) {
        if (D_000C4690[p[5]].state == 1)
            return 0;
        ang = ratan2(rdx, rdz) & 0xFFF;
        if (ABS(((ang - cop->ang + 0x800) & 0xFFF) - 0x800) < 0x400) {
            ang = car->ang;
            speed = cop->speed;
            cang = cop->ang;
            carspeed = car->speed;
            steps = 0;
            if (speed != 0)
                steps = d / speed;
            if (steps < 4) {
                cdx = (speed * D_0008EAA8[(cang & 0xFFF) * 2]) >> 12;
                cdz = (speed * D_0008EAA8[(cang & 0xFFF) * 2 + 1]) >> 12;
                kdx = (carspeed * D_0008EAA8[(ang & 0xFFF) * 2]) >> 12;
                kdz = (carspeed * D_0008EAA8[(ang & 0xFFF) * 2 + 1]) >> 12;
            } else {
                steps = steps / 4;
                cdx = (speed * D_0008EAA8[(cang & 0xFFF) * 2]) >> 10;
                cdz = (speed * D_0008EAA8[(cang & 0xFFF) * 2 + 1]) >> 10;
                kdx = (carspeed * D_0008EAA8[(ang & 0xFFF) * 2]) >> 10;
                kdz = (carspeed * D_0008EAA8[(ang & 0xFFF) * 2 + 1]) >> 10;
            }
            if (steps > 20)
                steps = 20;
            while (steps != 0) {
                x1 += cdx;
                z1 += cdz;
                x2 += kdx;
                z2 += kdz;
                dx = ABS(x1 - x2);
                dz = ABS(z1 - z2);
                if (dz < dx)
                    d = dx + dz / 2;
                else
                    d = dz + dx / 2;
                if (d < 1200)
                    goto hit;
                steps--;
            }
        }
        return 0;
hit:
        cop->timer = 10;

    }
    return 1;
}
