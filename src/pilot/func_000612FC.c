#include "common.h"

int SquareRoot0(int a);

int func_000612FC(VECTOR *a, VECTOR *b)
{
    VECTOR d;

    d.vx = a->vx - b->vx;
    d.vy = a->vy - b->vy;
    d.vz = a->vz - b->vz;
    return SquareRoot0(d.vx * d.vx + d.vy * d.vy + d.vz * d.vz);
}
