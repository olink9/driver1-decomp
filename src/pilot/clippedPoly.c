/* clippedPoly (Driver 2, shadow.c) */
typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long current;
#define current (*(DB **)&current)
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;

typedef struct { short vx, vy, vz, pad; } SVECTOR;

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char r1, g1, b1, pad1;
    short x1, y1;
    unsigned char r2, g2, b2, pad2;
    short x2, y2;
} POLY_G3;

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short x1, y1;
    unsigned char u1, v1;
    unsigned short tpage;
    short x2, y2;
    unsigned char u2, v2;
    unsigned short pad1;
} POLY_FT3;

extern POLY_G3 *D_0009717C;
extern int D_00097160;
extern int D_00097170;
extern SVECTOR D_000A1C58[];
extern long long LightSortCorrect;
#define LightSortCorrect (*(int *)&LightSortCorrect)
extern void clipAgainstZ();

#define gte_ldv3(r0, r1, r2) __asm__ volatile ( \
    "lwc2  $0, 0( %0 );" "lwc2  $1, 4( %0 );" "lwc2  $2, 0( %1 );" \
    "lwc2  $3, 4( %1 );" "lwc2  $4, 0( %2 );" "lwc2  $5, 4( %2 )" : : "r"(r0), "r"(r1), "r"(r2))
#define gte_rtpt() __asm__ volatile ("nop;" "nop;" "rtpt")
#define gte_stsxy3_g3(r0) __asm__ volatile ( \
    "swc2  $12, 8( %0 );" "swc2  $13, 16( %0 );" "swc2  $14, 24( %0 )" : : "r"(r0) : "memory")
#define gte_stsz3c(r0) __asm__ volatile ( \
    "swc2  $17, 0( %0 );" "swc2  $18, 4( %0 );" "swc2  $19, 8( %0 )" : : "r"(r0) : "memory")

void clippedPoly(void)
{
    int i;
    int j;
    int z1;
    POLY_G3 *pg3;
    POLY_FT3 *null;
    int z[3];

    pg3 = D_0009717C;

    i = D_00097160;
    j = D_00097170;
    while (--i != -1) {
        D_000A1C58[j].vz = D_000A1C58[j].vz - D_000A1C58[j].vx;
        j--;
    }
    clipAgainstZ();
    if (D_00097160 < 3)
        return;

    for (i = D_00097160 - 1, j = D_00097170; i != -1; i--, j--)
        D_000A1C58[j].vz += D_000A1C58[j].vx * 2;
    clipAgainstZ();
    if (D_00097160 < 3)
        return;

    for (i = D_00097160 - 1, j = D_00097170; i != -1; i--, j--)
        D_000A1C58[j].vz -= D_000A1C58[j].vx + D_000A1C58[j].vy * 2;
    clipAgainstZ();
    if (D_00097160 < 3)
        return;

    for (i = D_00097160 - 1, j = D_00097170; i != -1; i--, j--)
        D_000A1C58[j].vz += D_000A1C58[j].vy * 4;
    clipAgainstZ();
    if (D_00097160 < 3)
        return;

    for (i = D_00097160 - 1, j = D_00097170; i != -1; i--, j--)
        D_000A1C58[j].vz -= D_000A1C58[j].vy * 2;

    j = D_00097170;
    for (i = D_00097160 - 3; i >= 0; i--) {
        gte_ldv3(&D_000A1C58[D_00097170], &D_000A1C58[j - 1], &D_000A1C58[j - 2]);
        gte_rtpt();

        ((P_TAG *)pg3)->len = 6;
        pg3->code = 0x32;
        pg3->r0 = D_000A1C58[D_00097170].pad;
        pg3->g0 = D_000A1C58[D_00097170].pad;
        pg3->b0 = D_000A1C58[D_00097170].pad;
        pg3->r1 = D_000A1C58[j - 1].pad;
        pg3->g1 = D_000A1C58[j - 1].pad;
        pg3->b1 = D_000A1C58[j - 1].pad;
        pg3->r2 = D_000A1C58[j - 2].pad;
        pg3->g2 = D_000A1C58[j - 2].pad;
        pg3->b2 = D_000A1C58[j - 2].pad;

        gte_stsxy3_g3(pg3);
        gte_stsz3c(z);

        z1 = (z[0] + z[1] + z[2]) / 3 + LightSortCorrect;
        if (z1 < 64)
            z1 = 64;
        z1 >>= 3;

        ((P_TAG *)pg3)->addr = ((P_TAG *)&current->ot[z1])->addr;
        ((P_TAG *)&current->ot[z1])->addr = (unsigned int)pg3;
        pg3++;

        null = (POLY_FT3 *)pg3;
        ((P_TAG *)null)->len = 7;
        null->code = 0x24;
        null->x0 = -1;
        null->y0 = -1;
        null->x1 = -1;
        null->y1 = -1;
        null->x2 = -1;
        null->y2 = -1;
        null->tpage = 0x20;
        ((P_TAG *)null)->addr = ((P_TAG *)&current->ot[z1])->addr;
        ((P_TAG *)&current->ot[z1])->addr = (unsigned int)null;
        pg3++;
        current->primptr += 0x20;
        j--;
    }
    D_0009717C = pg3;
}
