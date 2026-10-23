/* Driver 2: bcollided2d */
typedef struct {
    int vx, vy, vz, pad;
} VEC;

typedef struct {
    VEC x;          /* 0x00 */
    VEC axis[2];    /* 0x10 */
    VEC vel;        /* 0x30 */
    int theta;      /* 0x40 */
    int length[2];  /* 0x44 */
    int dist[2];    /* 0x4C */
    int limit[2];   /* 0x54 */
    int pad;        /* 0x5C */
} CDATA2D;

extern short rcossin_tbl[];
extern long long D_000974AC;

#define ABS(x) (((x) < 0) ? -(x) : (x))
#define FIXEDH(x) (((x) + 2048) >> 12)

int bcollided2d(CDATA2D *body)
{
    int dtheta;
    int ac, as;
    int i, j, k;
    int xover, zover;
    int tmp;
    int FE;
    int needOverlap;
    VEC delta;

    dtheta = body[1].theta - body[0].theta;
    ac = rcossin_tbl[(dtheta + 1024 & 2047) * 2];
    as = rcossin_tbl[(dtheta & 2047) * 2];

    needOverlap = 0;
    if (body[0].length[1] < 0) {
        needOverlap = 1;
        body[0].length[1] = 5;
    }

    for (i = 1; i != -1; i--) {
        body[i].axis[0].vx = rcossin_tbl[(body[i].theta & 4095) * 2];
        body[i].axis[1].vx = body[i].axis[0].vz = rcossin_tbl[(body[i].theta & 4095) * 2 + 1];
        body[i].axis[1].vz = -body[i].axis[0].vx;
    }

    delta.vx = body[0].x.vx - body[1].x.vx;
    delta.vz = body[0].x.vz - body[1].x.vz;

    for (k = 0, i = 1; i != -1; i--) {
        for (j = 1; j != -1; j--) {
            body[i].dist[j] = FIXEDH(body[i].axis[j].vx * delta.vx + body[i].axis[j].vz * delta.vz);
            body[i].limit[j] = body[i].length[j] + FIXEDH(body[k].length[j] * ac + body[k].length[1 - j] * as);

            if (body[i].dist[j] > body[i].limit[j] || body[i].dist[j] < -body[i].limit[j])
                return 0;
        }
        k = i & 1;
    }

    if (needOverlap) {
        FE = ABS(abs(body[1].dist[0]) - abs(body[1].limit[0]));
        FE *= 4096;
        tmp = abs(FIXEDH(body->axis[0].vx * body[1].axis[0].vx + body->axis[0].vz * body[1].axis[0].vz));

        if (tmp > 10)
            xover = FE / tmp;
        else
            xover = -1;

        FE = ABS(abs(body[1].dist[1]) - abs(body[1].limit[1]));
        FE *= 4096;
        tmp = abs(FIXEDH(body->axis[0].vx * body[1].axis[1].vx + body->axis[0].vz * body[1].axis[1].vz));

        if (tmp > 10)
            zover = FE / tmp;
        else
            zover = xover;

        if (xover > -1 && zover >= xover)
            *(int *)&D_000974AC = xover;
        else
            *(int *)&D_000974AC = zover;
    }
    return 1;
}
