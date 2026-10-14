#include "common.h"
#include "gte.h"

/* Driver 2: SetShadowPoints */
typedef struct { char c[8]; } S8 __attribute__((aligned(1)));
typedef struct { int x, y, z, flags; } SP;
typedef struct { char pad[0x88]; int x, y, z; } Q;

extern S8 D_00086698[];
extern int D_0009D8DC[];
extern int D_00096FF8;
extern void func_0001EEB4();

void func_0001D3EC(unsigned char *car)
{
    SVECTOR disp;
    VECTOR v;
    VECTOR a;
    VECTOR b;
    int j;
    Q *q;

    gte_SetRotMatrix(car + 4);
    gte_SetTransVec0(car + 0x18);
    for (j = 0; j < 4; j++) {
        *(S8 *)&disp = D_00086698[j];
        if (D_0009D8DC[1] != 0) {
            disp.vx >>= 2;
            disp.vy >>= 2;
            disp.vz >>= 2;
        }
        q = (Q *)(car + j * 16);
        gte_ldv0(&disp);
        gte_rtv0tr();
        gte_stlvnl(&v);
        D_00096FF8 = 0;
        func_0001EEB4(&v, &a, &b, car[0]);
        q->x = b.vx;
        q->y = b.vy;
        q->z = b.vz;
        q->y &= ~1;
        if (D_00096FF8)
            q->y |= 1;
    }
}
