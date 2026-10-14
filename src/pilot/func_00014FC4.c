typedef struct {
    unsigned char lanes[4];
    short pos[4][2];
} JUNC;

typedef struct {
    char pad[10];
    unsigned char flags;
    char pad2[5];
} ROADINFO;

extern JUNC D_0009E744[];
extern long long D_00097044;
extern int func_0004FFE8();

void func_00014FC4(int j, int dir, int flip, int mode, unsigned char lane, int *out)
{
    int n;
    int max;
    int li;
    int off;
    int x;
    int z;
    int r;

    n = D_0009E744[j].lanes[dir];
    max = n - 1;
    if (mode == 1 || lane == 0xfe) {
        li = max;
    } else if (lane == 0xff && max != -1) {
        li = func_0004FFE8(j) % n;
    } else {
        if (max < lane)
            lane = max;
        if (flip)
            lane = max - lane;
        li = lane;
    }
    off = li * 1500;

    switch (dir) {
    case 0:
    default:
        x = D_0009E744[j].pos[0][0] * 1500 + off;
        z = D_0009E744[j].pos[0][1] * 1500;
        break;
    case 2:
        x = D_0009E744[j].pos[2][0] * 1500 - off;
        z = D_0009E744[j].pos[2][1] * 1500;
        break;
    case 1:
        x = D_0009E744[j].pos[1][0] * 1500;
        z = D_0009E744[j].pos[1][1] * 1500 - off;
        break;
    case 3:
        x = D_0009E744[j].pos[3][0] * 1500;
        z = D_0009E744[j].pos[3][1] * 1500 + off;
        break;
    }

    if (mode != 1) {
        if (lane == 0xfe)
            r = 300;
        else if ((*(ROADINFO **)&D_00097044)[j].flags & 1)
            r = (unsigned char)func_0004FFE8(j);
        else
            r = (func_0004FFE8(j) & 0x1ff) - 256;

        switch (dir) {
        case 0:
            x -= r;
            break;
        case 2:
            x += r;
            break;
        case 1:
            z += r;
            break;
        case 3:
            z -= r;
            break;
        }
    }
    out[0] = x;
    out[1] = z;
}
