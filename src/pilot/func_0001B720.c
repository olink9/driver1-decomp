/* Probably Driver 2's TempBuildHandlingMatrix (with RebuildCarMatrix inlined) */
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
    int direction;          /* 0xF8 */
    char padFC[0x104 - 0xFC];
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
extern COS D_000853D8[];
extern CAR D_0009D438;
extern long long D_00096898;
extern short D_0008EAAA[];
extern int func_000525B8();
extern void func_0001D3EC();
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

void func_0001B720(CAR *cp, int init)
{
    int dz;
    int i;

    i = cp->model;
    dz = D_0009AF04[i].d0 + D_0009AF04[i].d1;
    if (cp == &D_0009D438) {
        if (*(unsigned char *)&D_00096898 == 3 || *(unsigned char *)&D_00096898 == 6) {
            i = func_000525B8();
            dz = D_000853D8[i].d0 + D_000853D8[i].d1;
        }
    }
    dz = dz / 5;

    if (init == 1) {
        cp->st.fposition[0] = cp->drawt[0] << 4;
        cp->st.fposition[1] = cp->drawt[1] << 4;
        cp->st.fposition[2] = cp->drawt[2] << 4;
    }

    cp->st.orientation[0] = (-D_0008EAAA[(cp->direction >> 1 & 0x7FF) * 2] * dz + 0x800) >> 12;
    cp->st.orientation[1] = D_0008EAAA[(cp->direction >> 1 & 0x7FF) * 2 - 1];
    cp->st.orientation[2] = (D_0008EAAA[(cp->direction >> 1 & 0x7FF) * 2 - 1] * dz + 0x800) >> 12;
    cp->st.orientation[3] = D_0008EAAA[(cp->direction >> 1 & 0x7FF) * 2];

    if (*(int *)&D_00096858) {
        cp->st.orientation[0] = 0;
        cp->st.orientation[2] = 0;
    }

    RebuildCarMatrix(&cp->st, cp);
    func_0001D3EC(cp);
}
