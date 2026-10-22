/* Driver 2: bFindCollisionPoint (BCOLLIDE.C) */
typedef struct { int vx, vy, vz, pad; } VECTOR;

typedef struct {
    VECTOR x;
    VECTOR axis[2];
    VECTOR vel;
    int theta;
    int length[2];
    int dist[2];
    int limit[2];
    int avel;
} CDATA2D;

typedef struct {
    VECTOR hit;
    VECTOR surfNormal;
    int penetration;
} CRET2D;

#define FIXEDH(a) (((a) + 2048) >> 12)

void bFindCollisionPoint(CDATA2D *body, CRET2D *collisionResult)
{
    int lower, upper;
    int i, k, besti, bestk;
    int sign, sign0, sign1, smallest;
#define cd (&body[i ^ 1])

    besti = 0;
    bestk = 0;
    sign = 0;

    smallest = body[0].limit[0] + 1;

    i = 1;
    do {
        k = 1;
        do {
            upper = body[i].limit[k] - body[i].dist[k];
            lower = body[i].dist[k] + body[i].limit[k];

            if (smallest > upper) {
                smallest = upper;
                besti = i;
                bestk = k;
                sign = (i == 1) ? 1 : -1;
            }

            if (smallest > lower) {
                smallest = lower;
                besti = i;
                bestk = k;
                sign = (i == 1) ? -1 : 1;
            }

            k--;
        } while (k != -1);

        i--;
    } while (i != k);

    i = besti;

    if (cd->axis[0].vx * body[i].axis[bestk].vx + cd->axis[0].vz * body[i].axis[bestk].vz + 2048 > -1)
        sign0 = -sign;
    else
        sign0 = sign;

    if (cd->axis[1].vx * body[i].axis[bestk].vx + cd->axis[1].vz * body[i].axis[bestk].vz + 2048 > -1)
        sign1 = -sign;
    else
        sign1 = sign;

    collisionResult->hit.vx = cd->x.vx + FIXEDH(cd->axis[0].vx * cd->length[0] * sign0 + cd->axis[1].vx * cd->length[1] * sign1);
    collisionResult->hit.vz = cd->x.vz + FIXEDH(cd->axis[0].vz * cd->length[0] * sign0 + cd->axis[1].vz * cd->length[1] * sign1);

    collisionResult->penetration = smallest;

    if (i != 0)
        sign = -sign;

    collisionResult->surfNormal.vx = body[i].axis[bestk].vx * sign;
    collisionResult->surfNormal.vy = 0;
    collisionResult->surfNormal.vz = body[i].axis[bestk].vz * sign;
}
