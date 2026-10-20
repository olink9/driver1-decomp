typedef struct { char pad[0x18]; int x; int pad2; int z; char pad3[0xF8-0x24]; int rot; char pad4[0x18C-0xFC]; } C;
typedef struct { unsigned char f0, k, f2, f3, f4, on; char pad[0x50 - 6]; } P;
extern C D_0009B0A4[];
extern P D_00098950[];
extern short D_0008EAAA[];
extern long long D_000970D8;
extern void func_000555F4();

void func_00055820(void)
{
    int i;
    int s, c, dx, dz, vx, vy, q, k, ax;
    int idx;
    int big = 0x3FFF0;
     P *pl = D_00098950;

    *(volatile int *)&D_000970D8;
    *(volatile int *)&D_000970D8;
    idx = D_0009B0A4[0].rot & 0xFFF;
    s = D_0008EAAA[idx * 2];
    c = D_0008EAAA[idx * 2 - 1];
    for (i = 0; i < 22; i++, pl++) {
        k = pl->k;
        if (k && pl->on) {
            C *car = (C *)((i + 1) * 0x18C + (int)D_0009B0A4);
            dx = car->x - D_0009B0A4[0].x;
            dz = car->z - D_0009B0A4[0].z;
            vx = ((dx * s - dz * c + 0x800) >> 12) * 3 / 4;
            vy = -((dx * c + dz * s + 0x800) >> 12);
            if (vx < 0) ax = -vx; else ax = vx;
            if (ax < vy) {
                q = vx * 266 / vy;
                if (k == 4) {
                    
                    func_000555F4(q + 0x100, big / (vy / 8 + 600));
                }
            }
        }
    }
}
