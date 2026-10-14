/* Driver 2: bFindCollisionTime */
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
    char pad[0x24];
    int neverfree;
} CRET2D;

extern int func_0003F2E0(CDATA2D *cd);

int func_0003F88C(CDATA2D *cd, CRET2D *collisionResult)
{
    int hit;
    int i, q;
    int time, step;
    int neverfree;
    CDATA2D original[2];

    hit = 1;
    neverfree = 1;
    time = 4096;
    step = 2048;

    for (q = 1; q != -1; q--) {
        original[q].x.vx = cd[q].x.vx;
        original[q].x.vz = cd[q].x.vz;
        original[q].theta = cd[q].theta;
        for (i = 1; i != -1; i--) {
            original[q].limit[i] = cd[q].limit[i];
            original[q].dist[i] = cd[q].dist[i];
        }
    }

    i = 7;
    do {
        for (q = 1; q != -1; q--) {
            cd[q].vel.vx >>= 1;
            cd[q].vel.vz >>= 1;
            cd[q].avel >>= 1;

            if (hit) {
                cd[q].x.vx -= cd[q].vel.vx;
                cd[q].x.vz -= cd[q].vel.vz;
                cd[q].theta -= cd[q].avel;
            } else {
                cd[q].x.vx += cd[q].vel.vx;
                cd[q].x.vz += cd[q].vel.vz;
                cd[q].theta += cd[q].avel;
            }
        }

        if (hit) {
            time -= step;
        } else {
            time += step;
            neverfree = 0;
        }

        hit = func_0003F2E0(cd);

        if (i != 0)
            step >>= 1;

        i--;
    } while (i != -1);

    if (hit == 0) {
        for (q = 1; q != -1; q--) {
            cd[q].x.vx += cd[q].vel.vx;
            cd[q].x.vz += cd[q].vel.vz;
            cd[q].theta += cd[q].avel;
        }
        time += step;
        func_0003F2E0(cd);
    } else if (neverfree) {
        for (q = 1; q != -1; q--) {
            cd[q].x.vx = original[q].x.vx;
            cd[q].x.vz = original[q].x.vz;
            cd[q].theta = original[q].theta;
            func_0003F2E0(cd);
        }
        time = 4096;
    }

    collisionResult->neverfree = neverfree;

    return time;
}
