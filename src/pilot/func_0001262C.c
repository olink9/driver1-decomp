/* func_0001262C: does any other car (or the player) overlap the 4-plane box ahead of civ car cp; result cached in D_000988B0 (no Driver 2 counterpart) */
typedef struct {
    unsigned char id;       /* 0x00 */
    unsigned char type;     /* 0x01 */
    char pad2[0x34 - 2];
    int f34;
    int f38;
    char pad3C[0x50 - 0x3C];
} CIV;

typedef struct {
    char pad0[0xC8];
    int x;                  /* 0xC8 */
    int y;                  /* 0xCC */
    int z;                  /* 0xD0 */
    char padD4[0xF8 - 0xD4];
    int dir;                /* 0xF8 */
    char padFC[396 - 0xFC];
} CAR;

typedef struct { int nx, ny, nz, d; } PLANE;

extern short rcossin_tbl[];
extern long long D_00096898;
extern long long CameraCnt;
extern short D_000978B0[];
extern unsigned char D_000988B0[];
extern CIV D_00098900[];
extern CAR car_data;
extern CAR D_0009B230[];

int func_0001262C(CIV *cp)
{
    PLANE pl[4];
    int id;
    int done;
    int range;
    int slot;
    int dist;
    int i;
    int px, pz;
    int x, z;
    int ang;
    short *box;
    int x0, z0, x1, z1, x2, z2, x3, z3;
    int s, c;
    int d0, d1, d2, d3;
    int dd;
    int ox, oz;
    int other;
    unsigned char gm;

    done = 0;
    slot = cp->id;
    id = slot;
    if ((*(int *)&CameraCnt ^ slot) & 1)
        return D_000988B0[slot];

    if (cp->type == 3 && cp->f34 != 0) {
        range = 0xB4;
        dist = cp->f38 + 400;
    } else {
        range = 300;
        dist = (cp->f38 << 4) + 0x6D6;
    }

    cp = D_00098900;
    px = D_0009B230[id].x;
    pz = D_0009B230[id].z;

    for (i = 0; i < 23; i++, cp++) {
        if (i == 0) {
            gm = *(unsigned char *)&D_00096898;
            if (gm == 3 || gm == 6 || (unsigned char)(gm - 1) < 2)
                continue;
        }
        if (cp->type == 0 && i != 0)
            continue;
        other = cp->id;
        if (other == id && i != 0)
            continue;
        if (i != 0) {
            x = D_0009B230[other].x;
            z = D_0009B230[other].z;
        } else {
            x = car_data.x;
            z = car_data.z;
        }
        if (x - px >= 0) {
            if (x - px < dist) ; else continue;
        } else if (px - x >= dist)
            continue;
        if (z - pz >= 0) {
            if (z - pz < dist) ; else continue;
        } else if (pz - z >= dist)
            continue;

        if (!done) {
            ang = -D_0009B230[id].dir & 0xFFF;
            c = rcossin_tbl[ang * 2 + 1];
            s = rcossin_tbl[ang * 2];
            ox = (-range * c) >> 12;
            oz = (-range * s) >> 12;
            ox += px;
            oz += pz;
            pl[0].d = -c * ox + -s * oz;
            ox = (range * c) >> 12;
            oz = (range * s) >> 12;
            pl[1].d = c * (ox + px) + s * (oz + pz);
            ox = (dist * -s) >> 12;
            oz = (dist * c) >> 12;
            pl[2].d = -s * (ox + px) + c * (oz + pz);
            pl[3].d = s * px + -c * pz;
            pl[0].nx = -c;
            pl[0].nz = -s;
            pl[1].nx = c;
            pl[1].nz = s;
            pl[2].nx = -s;
            pl[2].nz = c;
            pl[3].nx = s;
            pl[3].nz = -c;
            done = 1;
        }

        ang = (i != 0 ? -D_0009B230[other].dir : -car_data.dir) & 0xFFF;
        box = &D_000978B0[((ang & 0x3FF) / 4) * 8];

        switch (ang & 0xC00) {
        case 0:
            x0 = box[0]; z0 = box[1];
            x1 = box[2]; z1 = box[3];
            x2 = box[4]; z2 = box[5];
            x3 = box[6]; z3 = box[7];
            break;
        case 0x400:
            z0 = box[0]; x0 = -box[1];
            z1 = box[2]; x1 = -box[3];
            z2 = box[4]; x2 = -box[5];
            z3 = box[6]; x3 = -box[7];
            break;
        case 0x800:
            x0 = -box[0]; z0 = -box[1];
            x1 = -box[2]; z1 = -box[3];
            x2 = -box[4]; z2 = -box[5];
            x3 = -box[6]; z3 = -box[7];
            break;
        default:
            x0 = box[1]; z0 = -box[0];
            x1 = box[3]; z1 = -box[2];
            x2 = box[5]; z2 = -box[4];
            x3 = box[7]; z3 = -box[6];
            break;
        }
        x0 += x; z0 += z;
        x1 += x; z1 += z;
        x2 += x; z2 += z;
        x3 += x; z3 += z;

        dd = pl[0].d;
        d0 = pl[0].nx * x0 + pl[0].nz * z0;
        d1 = pl[0].nx * x1 + pl[0].nz * z1;
        d2 = pl[0].nx * x2 + pl[0].nz * z2;
        d3 = pl[0].nx * x3 + pl[0].nz * z3;
        if (dd < d0 && dd < d1 && dd < d2 && dd < d3)
            continue;
        dd = pl[1].d;
        d0 = pl[1].nx * x0 + pl[1].nz * z0;
        d1 = pl[1].nx * x1 + pl[1].nz * z1;
        d2 = pl[1].nx * x2 + pl[1].nz * z2;
        d3 = pl[1].nx * x3 + pl[1].nz * z3;
        if (dd < d0 && dd < d1 && dd < d2 && dd < d3)
            continue;
        dd = pl[2].d;
        d0 = pl[2].nx * x0 + pl[2].nz * z0;
        d1 = pl[2].nx * x1 + pl[2].nz * z1;
        d2 = pl[2].nx * x2 + pl[2].nz * z2;
        d3 = pl[2].nx * x3 + pl[2].nz * z3;
        if (dd < d0 && dd < d1 && dd < d2 && dd < d3)
            continue;
        dd = pl[3].d;
        d0 = pl[3].nx * x0 + pl[3].nz * z0;
        d1 = pl[3].nx * x1 + pl[3].nz * z1;
        d2 = pl[3].nx * x2 + pl[3].nz * z2;
        d3 = pl[3].nx * x3 + pl[3].nz * z3;
        if (dd < d0 && dd < d1 && dd < d2 && dd < d3)
            continue;
        D_000988B0[slot] = 1;
        return 1;
    }
    D_000988B0[slot] = 0;
    return 0;
}
