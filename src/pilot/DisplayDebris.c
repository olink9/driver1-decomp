/* DisplayDebris: Driver 2 DisplayDebris (DEBRIS.C) */
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct { char *primptr; unsigned int *ot; } DB;
typedef struct { signed char vx, vy, vz, pad; } BVECTOR;
typedef struct { BVECTOR v0, v1, v2; } TRI_POINT;

typedef struct {
    struct { unsigned char u0, v0, u1, v1, u2, v2, u3, v3; } coords;
    unsigned short tpageid;
    unsigned short clutid;
} TEXTURE_DETAILS;

typedef struct {
    VECTOR position;        /* 0x00 */
    char pad10[8];
    short life;             /* 0x18 */
    char pad1A[4];
    unsigned short pos;     /* 0x1E */
    struct { unsigned char r, g, b, pad; } rgb;   /* 0x20 */
    char pad24;
    unsigned char type;     /* 0x25 */
} DEBRIS;

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
    unsigned char r3, g3, b3, p3;
    short x3, y3;
    unsigned char u3, v3;
    unsigned short pad3;
} POLY_GT4;

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

extern long long current;
#define current (*(DB **)&current)
extern VECTOR camera_position;
extern TRI_POINT *D_0008A75C[];
extern TEXTURE_DETAILS D_000B90D0;
extern TEXTURE_DETAILS D_000B9260;

#define gte_ldv0(r0) __asm__ volatile ( \
    "lwc2 $0, 0(%0);" "lwc2 $1, 4(%0)" : : "r"(r0))
#define gte_ldv3(r0, r1, r2) __asm__ volatile ( \
    "lwc2 $0, 0(%0);" "lwc2 $1, 4(%0);" "lwc2 $2, 0(%1);" "lwc2 $3, 4(%1);" \
    "lwc2 $4, 0(%2);" "lwc2 $5, 4(%2)" : : "r"(r0), "r"(r1), "r"(r2))
#define gte_ldv3c(r0) __asm__ volatile ( \
    "lwc2 $0, 0(%0);" "lwc2 $1, 4(%0);" "lwc2 $2, 8(%0);" \
    "lwc2 $3, 12(%0);" "lwc2 $4, 16(%0);" "lwc2 $5, 20(%0)" : : "r"(r0))
#define gte_rtpt() __asm__ volatile ("nop;" "nop;" "rtpt")
#define gte_rtps() __asm__ volatile ("nop;" "nop;" "rtps")
#define gte_stsxy3(r0, r1, r2) __asm__ volatile ( \
    "swc2 $12, 0(%0);" "swc2 $13, 0(%1);" "swc2 $14, 0(%2)" : : "r"(r0), "r"(r1), "r"(r2) : "memory")
#define gte_stsxy(r0) __asm__ volatile ("swc2 $14, 0(%0)" : : "r"(r0) : "memory")
#define gte_stsz(r0) __asm__ volatile ("swc2 $19, 0(%0)" : : "r"(r0) : "memory")

typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
#define setaddr(_p, _addr) (((P_TAG *)(_p))->addr = (unsigned long)(_addr))
#define getaddr(_p) (unsigned long)(((P_TAG *)(_p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
#define setlen(p, _len) (((P_TAG *)(p))->len = (unsigned char)(_len))

void DisplayDebris(DEBRIS *debris, unsigned char type)
{
    SVECTOR v[4];
    VECTOR debrisvec;
    int z;
    TRI_POINT *tv;
    POLY_GT4 *poly1;
    POLY_FT3 *poly;
    unsigned int cbgr;
    unsigned int b16;
    unsigned int gc;
    int pos;

    pos = debris->pos;

    debrisvec.vx = debris->position.vx - camera_position.vx;
    debrisvec.vy = debris->position.vy - camera_position.vy;
    debrisvec.vz = debris->position.vz - camera_position.vz;

    if (debrisvec.vx < -10000 ||
        debrisvec.vz < -10000 ||
        debrisvec.vx > 10000 ||
        debrisvec.vz > 10000)
    {
        debris->life = 1;
        return;
    }
    {
        tv = D_0008A75C[debris->type] + (pos >> 3 & 0x1fU);

        v[0].vx = tv->v0.vx + debrisvec.vx;
        v[0].vy = tv->v0.vy + debrisvec.vy;
        v[0].vz = tv->v0.vz + debrisvec.vz;
        v[1].vx = tv->v1.vx + debrisvec.vx;
        v[1].vy = tv->v1.vy + debrisvec.vy;
        v[1].vz = tv->v1.vz + debrisvec.vz;
        v[2].vx = tv->v2.vx + debrisvec.vx;
        v[2].vy = tv->v2.vy + debrisvec.vy;
        v[2].vz = tv->v2.vz + debrisvec.vz;

        gte_ldv3c(v);
        gte_rtpt();

        if ((unsigned char)(type - 1) < 2) {
            poly1 = (POLY_GT4 *)current->primptr;

            v[3].vx = debrisvec.vx - tv->v0.vx;
            v[3].vy = debrisvec.vy - tv->v0.vy;
            v[3].vz = debrisvec.vz - tv->v0.vz;

            *(unsigned int *)&poly1->u0 = *(unsigned short *)&D_000B90D0.coords.u0 | D_000B90D0.clutid << 16;
            *(unsigned int *)&poly1->u1 = *(unsigned short *)&D_000B90D0.coords.u1 | D_000B90D0.tpageid << 16;
            *(unsigned int *)&poly1->u2 = *(unsigned short *)&D_000B90D0.coords.u2;
            *(unsigned int *)&poly1->u3 = *(unsigned short *)&D_000B90D0.coords.u3;

            gte_stsxy3(&poly1->x0, &poly1->x1, &poly1->x2);
            gte_stsz(&z);

            if (z > 223) {
                gte_ldv0(&v[3]);
                gte_rtps();

                b16 = debris->rgb.b << 16;
                gc = debris->rgb.g << 8 | 0x3c000000;
                cbgr = b16 | gc | debris->rgb.r;
                setlen(poly1, 12);
                *(unsigned int *)&poly1->r0 = cbgr;
                *(unsigned int *)&poly1->r2 = cbgr;
                *(unsigned int *)&poly1->r1 = cbgr + 0x202020;
                *(unsigned int *)&poly1->r3 = cbgr + 0x606060;

                addPrim(current->ot + (z >> 3), poly1);
                gte_stsxy(&poly1->x3);
                current->primptr = (char *)(poly1 + 1);
            }
        } else {
            poly = (POLY_FT3 *)current->primptr;

            setlen(poly, 7);
            poly->code = 0x24;
            poly->u0 = D_000B9260.coords.u0;
            poly->v0 = D_000B9260.coords.v0;
            poly->u1 = D_000B9260.coords.u1;
            poly->v1 = D_000B9260.coords.v1;
            poly->u2 = D_000B9260.coords.u2;
            poly->v2 = D_000B9260.coords.v2;

            gte_stsxy3(&poly->x0, &poly->x1, &poly->x2);
            gte_stsz(&z);

            poly->r0 = debris->rgb.r;
            poly->g0 = debris->rgb.g;
            poly->b0 = debris->rgb.b;
            poly->tpage = D_000B9260.tpageid;
            poly->clut = D_000B9260.clutid;

            if (z > 223) {
                addPrim(current->ot + (z >> 3), poly);
                current->primptr += sizeof(POLY_FT3);
            }
        }
    }
}
