/* box/box separating-axis test (no Driver 2 counterpart found) */
typedef struct { int vx, vy, vz, pad; } VEC4;

typedef struct {
    VEC4 axis[3];
    VEC4 pos;
    int ext[3];
} BOXA;

typedef struct {
    char pad[0x30];
    VEC4 pos;
    char pad2[0xC];
    VEC4 axis[3];
} BOXB;

void func_00067A44(BOXA *a, BOXB *b, int ax, int *out, int flip)
{
    VEC4 n;
    VEC4 d;
    VEC4 p;
    int dist;
    int i;
    int t;

    n.vx = a->axis[ax].vx;
    n.vy = a->axis[ax].vy;
    n.vz = a->axis[ax].vz;
    p.vx = b->pos.vx;
    p.vy = b->pos.vy;
    p.vz = b->pos.vz;
    d.vx = b->pos.vx - a->pos.vx;
    d.vy = b->pos.vy - a->pos.vy;
    d.vz = b->pos.vz - a->pos.vz;
    dist = (d.vx * n.vx + d.vy * n.vy + d.vz * n.vz + 0x800) >> 12;
    if (dist < 0) {
        n.vx = -n.vx;
        n.vy = -n.vy;
        n.vz = -n.vz;
    } else {
        dist = -dist;
    }
    dist += a->ext[ax];
    for (i = 0; i < 3; i++) {
        t = (b->axis[i].vx * n.vx + b->axis[i].vy * n.vy + b->axis[i].vz * n.vz + 0x800) >> 12;
        if (t < 0) {
            p.vx += b->axis[i].vx;
            p.vy += b->axis[i].vy;
            p.vz += b->axis[i].vz;
            dist -= t;
        } else {
            p.vx -= b->axis[i].vx;
            p.vy -= b->axis[i].vy;
            p.vz -= b->axis[i].vz;
            dist += t;
        }
    }
    if (dist < out[0]) {
        out[0] = dist;
        if (dist >= 0) {
            out[1] = p.vx;
            out[2] = p.vy;
            out[3] = p.vz;
            if (flip < 0) {
                n.vx = -n.vx;
                n.vy = -n.vy;
                n.vz = -n.vz;
            }
            out[5] = n.vx;
            out[6] = n.vy;
            out[7] = n.vz;
        }
    }
}
