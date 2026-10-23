/* sQuad (Driver 2 SHADOW.C) */
typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long current;
#define current (*(DB **)&current)

typedef struct { short vx, vy, vz, pad; } SVECTOR;

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char r1, g1, b1, p1;
    short x1, y1;
    unsigned char r2, g2, b2, p2;
    short x2, y2;
    unsigned char r3, g3, b3, p3;
    short x3, y3;
} PG4;

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
} PFT3;

typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;

#define gte_ldv3(r0, r1, r2) __asm__ volatile ( \
    "lwc2  $0, 0( %0 );" "lwc2  $1, 4( %0 );" "lwc2  $2, 0( %1 );" \
    "lwc2  $3, 4( %1 );" "lwc2  $4, 0( %2 );" "lwc2  $5, 4( %2 )" : : "r"(r0), "r"(r1), "r"(r2))
#define gte_ldv0(r0) __asm__ volatile ("lwc2  $0, 0( %0 );" "lwc2  $1, 4( %0 )" : : "r"(r0))
#define gte_rtpt() __asm__ volatile ("nop;" "nop;" "rtpt")
#define gte_rtps() __asm__ volatile ("nop;" "nop;" "rtps")
#define gte_stsz(r0) __asm__ volatile ("swc2  $19, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy(r0) __asm__ volatile ("swc2  $14, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy3_g4(r0) __asm__ volatile ( \
    "swc2  $12, 8( %0 );" "swc2  $13, 16( %0 );" "swc2  $14, 24( %0 )" : : "r"(r0) : "memory")
#define gte_stsz3c(r0) __asm__ volatile ( \
    "swc2  $17, 0( %0 );" "swc2  $18, 4( %0 );" "swc2  $19, 8( %0 )" : : "r"(r0) : "memory")

extern PG4 *D_0009717C;
extern long long LightSortCorrect;
extern long long D_00097484;
extern SVECTOR D_000A1C58[];
extern int D_00097160;
extern int D_00097170;
extern void clippedPoly();

void sQuad(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3)
{
    int z1;
    int zz;
    int z[4];
    PG4 *poly;
    PFT3 *null;

    if (v0->vz > 1000 && v1->vz > 1000 && v2->vz > 1000 && v3->vz > 1000) {
        poly = D_0009717C;
        gte_ldv3(v0, v1, v3);
        gte_rtpt();
        ((P_TAG *)poly)->len = 8;
        poly->code = 0x3A;
        poly->r1 = *(unsigned char *)&D_00097484;
        poly->g1 = *(unsigned char *)&D_00097484;
        poly->b1 = *(unsigned char *)&D_00097484;
        poly->r0 = 0;
        poly->g0 = 0;
        poly->b0 = 0;
        poly->r2 = 0;
        poly->g2 = 0;
        poly->b2 = 0;
        poly->r3 = 0;
        poly->g3 = 0;
        poly->b3 = 0;
        gte_stsxy3_g4(poly);
        gte_stsz3c(&z[0]);
        gte_ldv0(v2);
        gte_rtps();
        gte_stsxy(&poly->x3);
        gte_stsz(&z[3]);

        z1 = (z[0] + z[1] + z[2] + z[3] >> 2) + *(int *)&LightSortCorrect;
        if (z1 < 0)
            z1 = 0;
        zz = z1 >> 3;

        ((P_TAG *)D_0009717C)->addr = ((P_TAG *)&current->ot[zz])->addr;
        ((P_TAG *)&current->ot[zz])->addr = (unsigned int)D_0009717C;

        poly++;
        null = (PFT3 *)poly;
        ((P_TAG *)null)->len = 7;
        null->code = 0x24;
        null->x0 = -1;
        null->y0 = -1;
        null->x1 = -1;
        null->y1 = -1;
        null->x2 = -1;
        null->y2 = -1;
        null->tpage = 0x20;
        ((P_TAG *)null)->addr = ((P_TAG *)&current->ot[zz])->addr;
        ((P_TAG *)&current->ot[zz])->addr = (unsigned int)null;
        poly++;
        D_0009717C = poly;
        current->primptr += 0x20;
    } else if (v0->vz > 0 || v1->vz > 0 || v2->vz > 0 || v3->vz > 0) {
        D_000A1C58[0] = *v0;
        D_000A1C58[1] = *v1;
        D_000A1C58[2] = *v3;
        D_000A1C58[1].pad = *(unsigned short *)&D_00097484;
        D_000A1C58[0].pad = 0;
        D_000A1C58[2].pad = 0;
        D_00097160 = 3;
        D_00097170 = 2;
        clippedPoly();

        D_000A1C58[0] = *v2;
        D_000A1C58[1] = *v1;
        D_000A1C58[2] = *v3;
        D_000A1C58[1].pad = *(unsigned short *)&D_00097484;
        D_000A1C58[0].pad = 0;
        D_000A1C58[2].pad = 0;
        D_00097160 = 3;
        D_00097170 = 2;
        clippedPoly();
    }
}
