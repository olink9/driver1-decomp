typedef struct { char pad[0x18]; int x; int pad2; int z; char pad3[0xF8-0x24]; int rot; char pad4[0x18C-0xFC]; } C;
extern C D_0009B0A4[];
extern char D_00098950[];
extern short D_0008EAAA[];
extern long long D_000970D8;
extern void func_000555F4();

void func_00055820(void)
{
    int i, off;
    int s, c, dx, dz, vx, vy, q, k, ax;
    unsigned char *p;
    int idx;
    int big = 0x3FFF0;
    C *base = D_0009B0A4;

    p = D_00098950 + 5;
    off = 0x18C;
    *(volatile int *)&D_000970D8;
    *(volatile int *)&D_000970D8;
    idx = base->rot & 0xFFF;
    s = D_0008EAAA[idx * 2];
    c = D_0008EAAA[idx * 2 - 1];
    for (i = 0; i < 22; i++, p += 80, off += 0x18C) {
        k = p[-4];
        if (k && p[0]) {
            C *car = (C *)((char *)base + off);
            dx = car->x - base->x;
            dz = car->z - base->z;
            vx = (dx * s - dz * c + 0x800) >> 12;
            vx = vx * 3 / 4;
            vy = -((dx * c + dz * s + 0x800) >> 12);
            ax = abs(vx);
            if (ax < vy) {
                q = vx * 266 / vy;
                if (k == 4) {
                    q += 0x100;
                    func_000555F4(q, big / (vy / 8 + 600));
                }
            }
        }
    }
}
