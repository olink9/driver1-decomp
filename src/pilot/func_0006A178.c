typedef struct {
    char p0[0x104];
    int f104;
    int pad108;
    int f10c;
    char p1[0x29C - 0x110];
    int s29c, s2a0;
    char p2[0x2E3 - 0x2A4];
    unsigned char b2e3;
} CAR;
typedef struct { int vx, vy, vz; } V3;
typedef struct { unsigned lo, hi; int pad[2]; V3 *pos; } TH;

extern int D_0009686C, D_00096E78;
extern int D_000BE9DC[];
extern long long D_00096898, D_00097668, D_000970D8, D_0009D5B4, D_00097088;
extern int D_0009B0A4[];

void func_0006A178(CAR *car, TH *th)
{
    V3 v;
    unsigned d;
    int t;
    int *q;
    V3 *pp = th->pos;
    v.vx = D_0009B0A4[50] - pp->vx;
    v.vz = D_0009B0A4[52] - pp->vz;
    d = v.vx * v.vx + v.vz * v.vz;
    if (D_0009686C != 0) {
        D_000BE9DC[0] = 1;
        if (car->b2e3 != 0)
            car->b2e3--;
        if (*(unsigned char *)&D_00096898 == 3) {
            t = ((**(int **)&D_00097668 - *(int *)&D_000970D8) << 14) / 30;
            q = &car->s29c;
            if (t < 0)
                t = 0;
            q[1] = t;
            car->s29c = 4;
        }
        if (*(short *)&D_0009D5B4 >= D_00096E78) {
            *(int *)&D_00097088 = 1;
            car->f104 = 0x7FFF;
            if (car->b2e3 != 0)
                car->f10c = 5;
            else
                car->f10c = 6;
        } else if (**(int **)&D_00097668 < *(int *)&D_000970D8) {
            if (d < th->lo) {
                car->f104 = 0x7FFF;
            } else {
                car->f104 = 0xFF0;
                car->f10c = 3;
            }
        }
    }
    if (th->hi < d) {
        car->f104 = 0xFF0;
        car->f10c = 3;
    }
}
