/* func_0006D688 - cubic curve evaluation (basis matrix D_0008D0AC); the four sums are formed first, then divided by T */
extern int D_00096F74;
extern int D_0008D0AC[16];
typedef struct { int vx, vy, vz, pad; } VEC4;
#define T D_00096F74
#define t2 (t * t)
#define T2 (T * T)
int func_0006D688(VEC4 *p, int *out, int dir, int t)
{
    int t3, T3;
    int c0, c1, c2, c3;
    int x, y, z, d;
    t3 = (t2 * t);
    T3 = (T2 * T);
    c0 = t3 * D_0008D0AC[0] + t2 * D_0008D0AC[4] * T + t * D_0008D0AC[8] * T2 + D_0008D0AC[12] * T3;
    c1 = t3 * D_0008D0AC[1] + t2 * D_0008D0AC[5] * T + t * D_0008D0AC[9] * T2 + D_0008D0AC[13] * T3;
    c2 = t3 * D_0008D0AC[2] + t2 * D_0008D0AC[6] * T + t * D_0008D0AC[10] * T2 + D_0008D0AC[14] * T3;
    c3 = t3 * D_0008D0AC[3] + t2 * D_0008D0AC[7] * T + t * D_0008D0AC[11] * T2 + D_0008D0AC[15] * T3;
    c0 /= T;
    c1 /= T;
    c2 /= T;
    c3 /= T;
    if (dir == 0) {
        x = c0 * p[0].vx + c1 * p[1].vx + c2 * p[2].vx + c3 * p[3].vx;
        y = c0 * p[0].vy + c1 * p[1].vy + c2 * p[2].vy + c3 * p[3].vy;
        z = c0 * p[0].vz + c1 * p[1].vz + c2 * p[2].vz + c3 * p[3].vz;
    } else {
        x = c0 * p[0].vx + c1 * p[-1].vx + c2 * p[-2].vx + c3 * p[-3].vx;
        y = c0 * p[0].vy + c1 * p[-1].vy + c2 * p[-2].vy + c3 * p[-3].vy;
        z = c0 * p[0].vz + c1 * p[-1].vz + c2 * p[-2].vz + c3 * p[-3].vz;
    }
    d = D_00096F74 * D_00096F74;
    x = (x >> 1) / d;
    y = (y >> 1) / d;
    z = (z >> 1) / d;
    out[0] = x;
    out[1] = y;
    out[2] = z;
    return t;
}
