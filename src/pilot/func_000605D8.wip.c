typedef struct { int a[99]; } CAR;
typedef struct { char a[80]; } ENT;
typedef struct { int x0, x1, z0, z1; } R;
extern long long D_00097080;
extern long long D_000970D8;
#define F80 (*(int *)&D_00097080)
#define F70 (*(int *)&D_000970D8)
extern int D_000976C4;
extern ENT D_00098950[];
extern CAR D_0009B0A4[];
extern CAR *D_000C0250[];
extern R D_000C0280[];

void func_000605D8(void)
{
    int pos[3];
    int cnt;
    int i;
    int lim;
    int lim2;
    int big;
    CAR *car;
    CAR *p;
    R *rb;
    R *q;
    R *rl;
    int idx;

    cnt = 2;
    lim = 400;
    big = F80;
    if (big)
        lim = 1200;
    lim2 = 400;
    car = D_0009B0A4;
    rb = D_000C0280;
    D_000C0250[0] = car;
    pos[0] = car->a[50] + (car->a[16] >> 8);
    D_000C0280[0].x0 = pos[0] - lim;
    pos[2] = car->a[52] + (car->a[18] >> 8);
    rb->z0 = pos[2] - lim;
    rb->x1 = pos[0] + lim;
    rb->z1 = pos[2] + lim;
    if (big)
        lim2 = 1200;
    pos[0] = car->a[50] + (car->a[16] >> 9);
    pos[2] = car->a[52] + (car->a[18] >> 9);
    q = rb + 1;
    q[0].x0 = pos[0] - lim2;
    pos[2] = car->a[52] + (car->a[18] >> 9);
    q->z0 = pos[2] - lim2;
    q->x1 = pos[0] + lim2;
    q->z1 = pos[2] + lim2;
    i = F70 & 3;
    D_000C0250[1] = car;
    if (i < 25) {
        rl = rb;
        p = D_0009B0A4 - 1 + i;
        for (; i < 25; i += 4) {
            if (D_00098950[i].a[1]) {
                idx = cnt++;
                lim = 400;
                if (big)
                    lim = 1200;
                pos[0] = p->a[50] + (p->a[16] >> 8);
                pos[2] = p->a[52] + (p->a[18] >> 8);
                rl[idx].x0 = pos[0] - lim;
                rl[idx].z0 = pos[2] - lim;
                rl[idx].x1 = pos[0] + lim;
                rl[idx].z1 = pos[2] + lim;
                D_000C0250[idx] = p;
            }
            p += 4;
        }
    }
    D_000976C4 = cnt;
}
