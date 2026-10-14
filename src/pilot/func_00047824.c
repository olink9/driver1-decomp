/* Driver 2: Setup_Debris */
typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { unsigned char r, g, b, pad; } RGB;

typedef struct {
    int position[4];
    SVECTOR direction;      /* 0x10 */
    unsigned short life;    /* 0x18 */
    unsigned short flags;   /* 0x1A */
    unsigned short num;     /* 0x1C */
    unsigned short pos;     /* 0x1E */
    RGB rgb;                /* 0x20 */
    char step;              /* 0x24 */
    char type;              /* 0x25 */
    char pad26[2];
} DEBRIS;

typedef struct {
    char pad[0x11];
    unsigned char f11;
    char pad12[0x50 - 0x12];
} CIV;

extern int D_00097414;
extern short D_000B9048[];
extern DEBRIS D_000BABC0[];
extern CIV D_00098950[];
extern RGB D_0008A770[];
extern long long D_00096850;
extern int func_000776E4(void);

static inline int AllocateDebris(void)
{
    if (D_00097414 < 60)
        return D_000B9048[D_00097414++];
    return -1;
}

void func_00047824(VECTOR *ipos, VECTOR *ispeed, int num_debris, int type)
{
    int seed;
    int i;
    int col;
    int num;
    int vx, vy, vz;
    DEBRIS *mydebris;

    col = type >> 16;
    type &= 0xffff;

    for (i = 0; i < num_debris; i++) {
        num = AllocateDebris();
        if (num == -1)
            return;

        mydebris = &D_000BABC0[num];
        mydebris->position[0] = ipos->vx;
        mydebris->position[1] = ipos->vy;
        mydebris->position[2] = ipos->vz;

        if (type < 8) {
            seed = func_000776E4();
            vy = ((seed & 0x7f00) * num_debris) * 2;
            vx = (ispeed->vx + ((seed & 0x7f) - 64) * num_debris) << 10;
            vz = ispeed->vz + ((func_000776E4() & 0x7fff) - 0x4000) * num_debris * 2;
            mydebris->step = seed;
            if (col > 1)
                col = D_00098950[col - 2].f11 + 1;
            mydebris->rgb.r = D_0008A770[col].r;
            mydebris->rgb.g = D_0008A770[col].g;
            mydebris->rgb.b = D_0008A770[col].b;
            if (*(int *)&D_00096850 != 0) {
                mydebris->rgb.r >>= 1;
                mydebris->rgb.g >>= 1;
                mydebris->rgb.b >>= 1;
            }
        } else {
            vy = ispeed->vy;
            vx = ispeed->vx;
            vz = ispeed->vz;
            mydebris->step = 2;
            mydebris->rgb.b = 60;
            mydebris->rgb.g = 60;
            mydebris->rgb.r = 60;
        }

        vx >>= 12;
        vy >>= 12;
        vz >>= 12;
        mydebris->type = type & 7;
        mydebris->direction.vx = vx;
        mydebris->direction.vy = -vy;
        mydebris->direction.vz = vz;
        mydebris->life = 0x80;
        mydebris->flags = 2;
        mydebris->pos = i & 0x1f;
    }
}
