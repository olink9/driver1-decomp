/* Probably Driver 2's InitCarPhysics (with RebuildCarMatrix inlined) */
typedef struct {
    int fposition[3];       /* 0x24 */
    int orientation[4];     /* 0x30 */
    int linearVelocity[3];  /* 0x40 */
    int angularVelocity[3]; /* 0x4C */
} ST;

typedef struct {
    unsigned char susCompression;
    char pad[3];
} WHEEL;

typedef struct {
    char pad0[2];
    unsigned char model;    /* 0x02 */
    char pad3;
    int where[5];           /* 0x04 */
    int t[3];               /* 0x18 */
    ST st;                  /* 0x24 */
    int acc[3];             /* 0x58 */
    char pad64[4];
    int aacc[3];            /* 0x68 */
    char pad74[4];
    WHEEL wheel[4];         /* 0x78 */
    char pad88[0xC8 - 0x88];
    int drawt[3];           /* 0xC8 */
    char padD4[0xF0 - 0xD4];
    int fF0;                /* 0xF0 */
    int fF4;                /* 0xF4 */
    char padF8[0x104 - 0xF8];
    int f104;               /* 0x104 */
    char pad108[0x17E - 0x108];
    unsigned char f17E;
    unsigned char f17F;
} CAR;

typedef struct {
    char pad[0x44];
    short d0;               /* 0x44 */
    char pad46[6];
    short d1;               /* 0x4C */
    char pad4E[0x64 - 0x4E];
} COS;

extern COS D_0009AF04[];
extern long long D_00096858;
extern long long D_000972E4;
extern long long D_000972DC;
extern void func_0001B380();

static inline void RebuildCarMatrix(ST *st, CAR *cp)
{
    int sm;
    int osm;
    int qx;

    cp->drawt[0] = cp->t[0] = st->fposition[0] >> 4;
    cp->drawt[1] = cp->t[1] = st->fposition[1] >> 4;
    cp->drawt[2] = cp->t[2] = st->fposition[2] >> 4;

    qx = st->orientation[0];
    osm = qx * qx + st->orientation[1] * st->orientation[1] +
          st->orientation[2] * st->orientation[2] + st->orientation[3] * st->orientation[3];
    if (osm < 1024) {
        st->orientation[2] = 0;
        st->orientation[1] = 0;
        st->orientation[0] = 0;
        st->orientation[3] = 4096;
    } else {
        sm = 0x1800 - (osm >> 13);
        st->orientation[0] = (sm * qx + 0x800) >> 12;
        st->orientation[1] = (sm * st->orientation[1] + 0x800) >> 12;
        st->orientation[2] = (sm * st->orientation[2] + 0x800) >> 12;
        st->orientation[3] = (sm * st->orientation[3] + 0x800) >> 12;
    }
    func_0001B380(st->orientation, cp->where);
}

void func_0001B4A4(CAR *cp, int *startpos)
{
    int dz;
    int i;

    dz = D_0009AF04[cp->model].d0 + D_0009AF04[cp->model].d1;
    dz = dz / 32;
    cp->f17F = 0;
    cp->f17E = 0;
    cp->st.orientation[0] = 0;
    cp->st.orientation[1] = 0;
    cp->st.orientation[2] = 0;
    cp->st.orientation[3] = 4096;
    cp->st.fposition[0] = startpos[0] << 4;
    cp->st.fposition[1] = startpos[1] << 4;
    cp->st.fposition[2] = startpos[2] << 4;
    cp->st.linearVelocity[0] = 0;
    cp->st.linearVelocity[1] = 0;
    cp->st.linearVelocity[2] = 0;
    cp->st.angularVelocity[0] = 0;
    cp->st.angularVelocity[1] = 0;
    cp->st.angularVelocity[2] = 0;
    cp->aacc[0] = 0;
    cp->aacc[1] = 0;
    cp->aacc[2] = 0;
    cp->acc[0] = 0;
    cp->acc[1] = 0;
    cp->acc[2] = 0;
    RebuildCarMatrix(&cp->st, cp);
    if (*(int *)&D_00096858) {
        for (i = 3; i != -1; i--)
            cp->wheel[i].susCompression = 21;
    } else {
        cp->wheel[0].susCompression = 20 - dz;
        cp->wheel[1].susCompression = dz + 21;
        cp->wheel[2].susCompression = 20 - dz;
        cp->wheel[3].susCompression = dz + 21;
    }
    cp->fF0 = 0;
    cp->fF4 = 0;
    cp->f104 = 0;
    *(char *)&D_000972E4 = 0;
    *(char *)&D_000972DC = 0;
}
