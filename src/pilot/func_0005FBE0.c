/* no Driver 2 counterpart: picks an open direction (bitmask) and the target cell position */
typedef struct { int vx, vy, vz, pad; } VECTOR;

extern short D_000C0390[];
extern int D_0009E714[];
extern int func_0004FFE8(int);

static inline int cellpos(int c)
{
    return c * 1500 + 750;
}

int func_0005FBE0(int mask, int x, int z, VECTOR *out)
{
    int n;
    int dirs;
    int bit;
    int cx, cz;

    n = 0;
    dirs = 0;
    if (D_000C0390[4] == -32768) {
        dirs = 1;
        n = 1;
    }
    if (D_000C0390[3] == -32768) {
        dirs |= 2;
        n++;
    }
    if (D_000C0390[1] == -32768) {
        dirs |= 4;
        n++;
    }
    if (D_000C0390[2] == -32768) {
        dirs |= 8;
        n++;
    }

    if (dirs & mask) {
        dirs &= mask;
    } else if (n >= 2) {
        bit = 16;
        n = func_0004FFE8(0) % n + 1;
        while (n) {
            bit >>= 1;
            if (dirs & bit)
                n--;
        }
        dirs &= bit;
    }

    cx = (x + D_0009E714[2]) / 1500;
    cz = (D_0009E714[3] + (750 - z)) / 1500;

    switch (dirs) {
    case 1:
        out->vx = cellpos(cx - 1) - D_0009E714[2];
        out->vz = D_0009E714[3] - cellpos(cz);
        break;
    case 2:
        out->vx = cellpos(cx + 1) - D_0009E714[2];
        out->vz = D_0009E714[3] - cz * 1500;
        break;
    case 4:
        out->vx = cellpos(cx) - D_0009E714[2];
        out->vz = D_0009E714[3] - (cz + 1) * 1500;
        break;
    case 8:
        out->vx = cellpos(cx) - D_0009E714[2];
        out->vz = D_0009E714[3] - (cz - 1) * 1500;
        break;
    }
    out->vy = 0;

    return dirs;
}
