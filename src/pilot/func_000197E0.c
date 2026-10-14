/* func_000197E0 - no Driver 2 counterpart */
typedef struct {
    char pad0[0x28];
    int f28;
    char pad2C[0x50 - 0x2C];
} ENT;

extern long long D_00096870;
extern ENT D_00098950[];
extern int func_00016760();
extern int func_0004FFE8();

typedef struct {
    int x;
    int z;
} POS;

static inline void PlaceOne(int x, int z, int dir, int seed, int val)
{
    int r;

    r = (func_0004FFE8(seed) & 0x3FF) - 0x600;
    D_00098950[func_00016760(x, -250, z, ((dir + 2) & 3) * 1024 - r, 1)].f28 = val;
}

void func_000197E0(POS pos, int val, int dir, int count, unsigned char flag)
{
    int *p[2];
    POS tmp;
    int off;
    int i;

    off = 0;
    if (dir & 1) {
        p[0] = &pos.z;
        p[1] = &pos.x;
    } else {
        p[0] = &pos.x;
        p[1] = &pos.z;
    }
    switch (*(int *)&D_00096870) {
    case 0:
        break;
    case 1:
        off = 1500;
        break;
    case 2:
        off = 1750;
        break;
    }
    if (dir == 2 || dir == 3)
        off = -off;
    for (i = 0; i < count; i++) {
        PlaceOne(pos.x, pos.z, dir, i, val);
        if (flag || i < count - 1) {
            switch (*(int *)&D_00096870) {
            case 0:
                break;
            case 1:
                tmp.x = pos.x;
                tmp.z = pos.z;
                *p[0] += 750;
                *p[1] += off;
                PlaceOne(pos.x, pos.z, (dir - 1) & 3, i, val);
                pos.x = tmp.x;
                pos.z = tmp.z;
                break;
            case 2:
                tmp.x = pos.x;
                tmp.z = pos.z;
                *p[0] += 750;
                *p[1] += off;
                PlaceOne(pos.x, pos.z, dir, i, val);
                pos.x = tmp.x;
                pos.z = tmp.z;
                break;
            }
        }
        *p[0] += 1500;
        *p[1] += 0x80 - (func_0004FFE8(i) & 0xFF);
    }
}
