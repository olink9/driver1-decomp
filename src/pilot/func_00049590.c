/* Driver 2: DrawRainDrops (DEBRIS.C) */
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct { char *primptr; unsigned int *ot; } DB;
typedef struct { short m[3][3]; int t[3]; } MATRIX;

typedef struct {
    unsigned short coords[4];
    unsigned short tpageid;
    unsigned short clutid;
} TEXTURE_DETAILS;

typedef struct {
    int vx, vy, vz;
    SVECTOR oldposition;
} RAIN_TYPE;

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    unsigned char r1, g1, b1, p1;
    short x1, y1;
    unsigned char u1, v1;
    unsigned short tpage;
    unsigned char r2, g2, b2, p2;
    short x2, y2;
    unsigned char u2, v2;
    unsigned short pad2;
} POLY_GT3;

extern int D_00097428;
typedef struct { unsigned char b[8]; } B8;
extern B8 D_00096897;
extern short D_0009B1AA[];
extern long long D_00096850;
extern int D_00097474;
extern long long D_0009687C;
extern long long D_00097530;
#define current (*(DB **)&D_00097530)
extern MATRIX D_000BBE84;
extern VECTOR D_0008909C;
extern RAIN_TYPE D_000B81C8[];
extern TEXTURE_DETAILS D_000B9170;
extern VECTOR D_0008A8F0;
extern short D_000B8028[];
extern void func_000466D4(VECTOR *v);
extern void func_0004A064(void);

#define gte_SetRotMatrix(r0) __asm__ volatile ( \
    "lw    $12, 0( %0 );" "lw    $13, 4( %0 );" "ctc2  $12, $0;" "ctc2  $13, $1;" \
    "lw    $12, 8( %0 );" "lw    $13, 12( %0 );" "lw    $14, 16( %0 );" \
    "ctc2  $12, $2;" "ctc2  $13, $3;" "ctc2  $14, $4" : : "r"(r0) : "$12", "$13", "$14")
#define gte_SetTransVector(r0) __asm__ volatile ( \
    "lw    $12, 0( %0 );" "lw    $13, 4( %0 );" "lw    $14, 8( %0 );" \
    "ctc2  $12, $5;" "ctc2  $13, $6;" "ctc2  $14, $7" : : "r"(r0) : "$12", "$13", "$14")
#define gte_ldv0(r0) __asm__ volatile ( \
    "lwc2  $0, 0( %0 );" "lwc2  $1, 4( %0 )" : : "r"(r0))
#define gte_rtps() __asm__ volatile ("nop;" "nop;" "rtps")
#define gte_stsxy(r0) __asm__ volatile ("swc2  $14, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsz(r0) __asm__ volatile ("swc2  $19, 0( %0 )" : : "r"(r0) : "memory")

typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
#define setaddr(_p, _addr) (((P_TAG *)(_p))->addr = (unsigned long)(_addr))
#define getaddr(_p) (unsigned long)(((P_TAG *)(_p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)

static inline void ReleaseRainDrop(int idx)
{
    D_000B8028[--D_00097428] = idx;
}

void func_00049590(void)
{
    SVECTOR v[3];
    VECTOR drift;
    int z;
    int Count;
    POLY_GT3 *poly;
    RAIN_TYPE *RainPtr;
    int col;
    int tpage;
    int clut;
    int notfirst;
    int w;

    if (D_00097428 == 0)
        return;

    func_000466D4(&drift);

    if (D_00096897.b[0] != 1)
        col = 70 - D_0009B1AA[0];
    else
        col = 70;

    if (col < 50)
        col = 50;

    if (*(int *)&D_00096850 != 0)
        col -= 15;

    col = col >> 1;
    col |= col << 8;
    col |= col << 16;

    if (D_00097474 == 0)
        func_0004A064();

    gte_SetRotMatrix(&D_000BBE84);
    gte_SetTransVector(&D_0008909C);

    RainPtr = D_000B81C8;
    tpage = D_000B9170.tpageid | 0x200000;
    clut = D_000B9170.clutid << 16;
    Count = D_00097428;

    while (Count--) {
        if (*(int *)&D_0009687C == 0) {
            RainPtr->vy += 30;
            RainPtr->vx -= drift.vx * 2;
            RainPtr->vz -= drift.vz * 2;
        }

        v[0].vx = RainPtr->vx - D_0008A8F0.vx;
        v[0].vy = RainPtr->vy - D_0008A8F0.vy;
        v[0].vz = RainPtr->vz - D_0008A8F0.vz;

        poly = (POLY_GT3 *)current->primptr;
        notfirst = *(int *)&RainPtr->oldposition;

        gte_ldv0(v);
        gte_rtps();

        *(int *)&poly->r0 = 0;
        poly->code = 0x36;
        *(int *)&poly->r1 = col;
        *(int *)&poly->r2 = col;
        ((P_TAG *)poly)->len = 9;
        *(int *)&poly->x0 = *(int *)&RainPtr->oldposition;

        gte_stsz(&z);
        gte_stsxy(&poly->x2);

        if (z - 151U >= 850 || poly->x2 < -100 || poly->x2 >= 613 || poly->y2 < -50 || poly->y2 >= 241) {
            ReleaseRainDrop(RainPtr->oldposition.pad);
            RainPtr++;
        } else {
            *(int *)&RainPtr->oldposition = *(int *)&poly->x2;
            if (notfirst) {
                w = (z >> 10) - 2;
                *(unsigned int *)&poly->u0 = D_000B9170.coords[0] | clut;
                *(unsigned int *)&poly->u1 = D_000B9170.coords[1] | tpage;
                *(unsigned int *)&poly->u2 = D_000B9170.coords[2];
                poly->x1 = poly->x2 - w;
                poly->y1 = poly->y2;
                addPrim(current->ot + (z >> 3), poly);
                current->primptr += sizeof(POLY_GT3);
            }
            RainPtr++;
        }
    }
}
