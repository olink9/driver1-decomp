// dist
#include "common.h"
extern int SquareRoot0(int);
int dist(VECTOR *pos1, VECTOR *pos2) {
    int dx;
    int dz;
    dx = (pos1->vx - pos2->vx) / 16;
    dz = (pos1->vz - pos2->vz) / 16;
    return SquareRoot0(dx * dx + dz * dz) << 4;
}
