/* lamp / light flare draw (no Driver 2 counterpart); sibling of func_00042C8C */
typedef struct { int x, y, z, w; } V;
typedef struct { int x, y, z; unsigned char pad, rot; } O;
struct L { int a, b, c, d, id; char e, g, set; char f; };
extern int D_0008A8F0[];
extern int D_0008A8F4[];
extern long long D_00096C48, D_00096C4A;
extern long long D_00096850;
extern long long D_00096834;
extern int D_00097480;
extern int D_00097464;
extern int D_00097448;
extern struct L D_000B738C[];
extern int D_000BBDD0[];
extern int D_000B9170[];
extern int D_000B9230[];
extern int func_0001F5B0();
extern void func_0004449C();
extern void func_00045718();
extern void func_000451A8();
extern void func_0004A398();

static inline int find_lamp_streak(int id)
{
    int i;
    for (i = 0; i < 20; i++) {
        if (D_000B738C[i].id == id) {
            D_000B738C[i].set = 1;
            return i;
        }
    }
    if (D_00097448 < 20) {
        D_000BBDD0[D_00097448++] = id;
    }
    return -1;
}

void func_0004343C(O *o, int a1, int a2, int a3, int flags)
{
    unsigned char col[4];
    unsigned char col3[4];
    unsigned char col2[4];
    V pos;
    V v2;
    int angx;
    int angy;
    int d;
    int ax;
    int ay;

    if (flags & 0x200) {
        col[0] = 255;
        col[1] = 25;
        col[2] = 25;
    } else if (flags & 0x400) {
        col[0] = 255;
        col[1] = 100;
        col[2] = 35;
    } else if (flags & 0x800) {
        col[0] = 25;
        col[1] = 255;
        col[2] = 25;
    }
    col[3] = 0;

    switch ((o->rot + 8) >> 4) {
    case 0:
        pos.x = o->x - D_0008A8F0[0] + a1;
        pos.z = o->z - D_0008A8F0[2] + a3;
        break;
    case 1:
        pos.x = o->x - D_0008A8F0[0] + a3;
        pos.z = o->z - D_0008A8F0[2] - a1;
        break;
    case 2:
        pos.x = o->x - D_0008A8F0[0] - a1;
        pos.z = o->z - D_0008A8F0[2] - a3;
        break;
    case 3:
        pos.x = o->x - D_0008A8F0[0] - a3;
        pos.z = o->z - D_0008A8F0[2] + a1;
        break;
    }
    angx = ((*(short *)&D_00096C48 + 0x800) & 0xFFF) - 0x800;
    pos.y = o->y - D_0008A8F4[0] + a2;
    ax = angx;
    if (angx < 0)
        ax = -angx;
    angy = (((-*(short *)&D_00096C4A & 0xFFF) - (o->rot << 6)) + 0x800 & 0xFFF) - 0x800;
    ay = angy;
    if (angy < 0)
        ay = -angy;

    if (ay <= 1000) {
        col2[0] = col[0];
        col2[2] = col[2];
        col2[1] = col[1];
        col3[0] = col[0] >> 4;
        col3[2] = col[2] >> 4;
        col3[1] = col[1] >> 4;
        v2.x = pos.x;
        v2.z = pos.z;

        if (ax + ay < 1000) {
            d = 1000 - (ax + ay);
            if (d < 0)
                d = 0;
            col[0] = (col[0] * d) >> 10;
            col[2] = (col[2] * d) >> 10;
            col[1] = (col[1] * d) >> 10;
            col2[0] = col[0];
            col2[2] = col[2];
            col2[1] = col[1];
            D_00097480 = -140;
            D_00097464 = find_lamp_streak((int)o + a1 + a2);
            if (D_00097464 >= 0)
                col[3] = 0x20;
            else
                col[3] = 0;
            func_0004449C(&pos, col, 50, D_000B9170);
            col[0] >>= 1;
            col[2] >>= 1;
            col[1] >>= 1;
            if (*(int *)&D_00096850 || *(int *)&D_00096834 == 3)
                func_00045718(&pos, col, 300, ((angx + angy + (pos.x + pos.z)) >> 3) & 0x1FFE);
        }
        v2.y = -func_0001F5B0(&pos) - D_0008A8F4[0];
        if (*(int *)&D_00096850)
            func_000451A8(&v2, col3, 400);
        func_0004A398(&v2, col, 100, D_000B9230);
        D_00097480 = -10;
    }
}
