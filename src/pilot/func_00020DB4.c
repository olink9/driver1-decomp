/* func_00020DB4 - no Driver 2 counterpart: find the road cell next to a position along x or z */
typedef struct {
    int width;
    int height;
    int ofs_x;
    int ofs_z;
} MAPINFO;

typedef struct {
    int vx, vy, vz, pad;
} VEC;

typedef struct {
    short dx;
    short dz;
    short x;
    short z;
    unsigned char hit;
} ROADOUT;

extern int D_0009E714[];
extern short *D_0009E724[];
extern long long D_00097374;
extern void func_000774C4();
extern int func_0004A9F4();

static inline VEC CopyPos(VEC *in)
{
    VEC r;

    func_000774C4(&r, 0, 16);
    r.vx = in->vx;
    r.vz = in->vz;
    return r;
}

static inline short ProbeZ(short *cell, int dstep, int dd, VEC *pos, ROADOUT *out)
{
    short *p = cell;
    int dist = 0;
    int step = dstep;
    int d = dd;
    short id;
    VEC v;

    do { p += step; dist += d; } while (*p == *cell);
    v = CopyPos(pos);
    p += step;
    id = *p;
    if ((unsigned short)id < 0x3C00) {
        v.vz += dist;
        out->dx = 0;
        out->dz = dist + d;
        if (func_0004A9F4(&v)) {
            out->hit = 1;
            out->x = v.vx;
            out->z = v.vz;
        } else
            out->hit = 0;
    } else
        id = -1;
    return id;
}

static inline short ProbeX(short *cell, int dstep, int dd, VEC *pos, ROADOUT *out)
{
    short *p = cell;
    int dist = 0;
    int step = dstep;
    int d = dd;
    short id;
    VEC v;

    do { p += step; dist += d; } while (*p == *cell);
    v = CopyPos(pos);
    p += step;
    id = *p;
    if ((unsigned short)id < 0x3C00) {
        v.vx += dist;
        out->dx = dist + d;
        out->dz = 0;
        if (func_0004A9F4(&v)) {
            out->hit = 1;
            out->x = v.vx;
            out->z = v.vz;
        } else
            out->hit = 0;
    } else
        id = -1;
    return id;
}

short func_00020DB4(int x, int z, int dir, ROADOUT *out)
{
    int cx, cz, xc, zc, t;
    short *cell;
    short r;
    VEC pos;

    cx = (x + D_0009E714[2]) / 1500;
    t = D_0009E714[3] + 750;
    cz = (t - z) / 1500;
    if (cx >= 0 && cx < D_0009E714[0] && cz >= 0 && cz < D_0009E714[1])
    {
    zc = cz / 60; xc = cx / 60; cx -= xc * 60; cz -= zc * 60; zc = *(int *)&D_00097374 - zc;
    cell = &D_0009E724[(xc & 1) + ((zc & 1) << 1)][cz * 60 + cx];
    if (*cell == -1)
        return -1;

    func_000774C4(&pos, 0, 16);
    pos.vx = x;
    pos.vz = z;

    if (dir & 1)
    {
        r = ProbeZ(cell, 60, 1500, &pos, out);
        if (r != -1)
            return r;
        return ProbeZ(cell, -60, -1500, &pos, out);
    }
    else
    {
        r = ProbeX(cell, -1, -1500, &pos, out);
        if (r != -1)
            return r;
        return ProbeX(cell, 1, 1500, &pos, out);
    }
    }
    return -1;
}
