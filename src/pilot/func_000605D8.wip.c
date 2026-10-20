/* r3-14 (99.36): statement order from a dependency-safe permutation sweep + loop rects via D_000C0280[idx]
   (loop.c's hoisted base becomes the 'addu a1,t3' copy). Left: only big (F80 copy) vs the car field loads:
   target big=t5 above a[52]/a[16]/a[50]=t6/t7/t8, ours fields t5/t6/t7 and big t8. Global-alloc priority:
   big 5 refs/103 insns = .097 vs fields 3 refs/22-24 = .125-.136; big would need 7-8 refs. ?:, if/else,
   block-local loop lim, asm("" : "+r"(big)) placements all tried. */
typedef struct { int a[99]; } CAR;
typedef struct { char a[80]; } ENT;
typedef struct { int x0, x1, z0, z1; } R;
typedef struct { int vx, vy, vz, pad; } VEC;
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
    CAR *cb;
    R *rb;
    R *q;
    R *rl;
    int idx;

    car = D_0009B0A4;
    cnt = 2;
    lim = 400;
    big = F80;
    if (big) lim = 1200;
    lim2 = 400;
    pos[0] = car->a[50] + (car->a[16] >> 8);
    D_000C0250[0] = car;
    pos[2] = car->a[52] + (car->a[18] >> 8);
    rb = D_000C0280;
    D_000C0280[0].x0 = pos[0] - lim;
    rb->z0 = pos[2] - lim;
    rb->x1 = pos[0] + lim;
    rb->z1 = pos[2] + lim;
    if (big) lim2 = 1200;
    i = F70 & 3;
    pos[0] = car->a[50] + (car->a[16] >> 9);
    pos[2] = car->a[52] + (car->a[18] >> 9);
    q = rb + 1;
    q[0].x0 = pos[0] - lim2;
    q->z0 = pos[2] - lim2;
    q->x1 = pos[0] + lim2;
    q->z1 = pos[2] + lim2;
    D_000C0250[1] = car;
    if (i < 25) {
        rl = rb;
        cb = car - 1;
        for (; i < 25; i += 4) {
            if (D_00098950[i].a[1]) {
                idx = cnt++;
                p = &cb[i];
                lim = 400;
                if (big)
                    lim = 1200;
                pos[0] = p->a[50] + (p->a[16] >> 8);
                pos[2] = p->a[52] + (p->a[18] >> 8);
                D_000C0280[idx].x0 = pos[0] - lim;
                D_000C0280[idx].z0 = pos[2] - lim;
                D_000C0280[idx].x1 = pos[0] + lim;
                D_000C0280[idx].z1 = pos[2] + lim;
                D_000C0250[idx] = p;
            }
        }
    }
    D_000976C4 = cnt;
}

