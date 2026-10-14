typedef struct { int vx, vy, vz, pad; } VEC;
typedef struct {
    VEC pos;        /* 0x00 */
    VEC opos;       /* 0x10 */
    short vel[3];   /* 0x20 */
    short pad26;
    VEC spos;       /* 0x28 */
    short s38;      /* 0x38 */
    short pad3a;
    short s3C;      /* 0x3C */
    short s3E;      /* 0x3E */
    int i40;        /* 0x40 */
    unsigned char c44; /* 0x44 */
    char pad45[3];
    int i48;        /* 0x48 */
    short pad4c;
    short s4E;      /* 0x4E */
} DEBRIS;

extern long long D_0009687C;
extern int D_0009742C;
extern short D_000BB530[];
extern DEBRIS D_000B92A0[];
extern int func_000776E4();

static inline int alloc(void)
{
    int n;
    if (D_0009742C < 80) {
        n = D_000BB530[D_0009742C];
        D_0009742C++;
        return n;
    }
    return -1;
}

void func_00047FA4(VEC *pos, VEC *vel, int count, unsigned char type)
{
    int i;
    int n;
    DEBRIS *p;
    int r;
    int x, y, z;

    if (*(int *)&D_0009687C != 0)
        return;
    for (i = 0; i < count; i++) {
        n = alloc();
        if (n < 0)
            return;
        r = func_000776E4();
        p = &D_000B92A0[n];
        p->pos.vx = pos->vx;
        p->pos.vy = pos->vy;
        p->pos.vz = pos->vz;
        p->opos = p->pos;
        p->spos = p->pos;
        p->s4E = (unsigned char)(i << 2);
        p->i40 = r & 0x3F;
        p->i48 = 10;
        x = vel->vx - 0x4000;
        x += r & 0x7F00;
        x <<= 2;
        y = vel->vy + (r & 0x7F);
        y <<= 10;
        r = func_000776E4();
        z = vel->vz - 0x40;
        p->vel[0] = x >> 12;
        p->vel[1] = -(y >> 12);
        p->vel[2] = (unsigned int)(z + (r & 0x7F)) >> 2;
        if (type == 1) {
            p->s3C = 200;
            p->c44 = 10;
            p->s3E = 2;
            p->s38 = 0;
        } else if (type == 2) {
            p->c44 = 10;
            p->s3C = 255;
            p->s3E = 2;
            p->s38 = 2;
        } else {
            p->c44 = 40;
            p->s3C = 255;
            p->s3E = 10;
            p->s38 = 1;
        }
    }
}
