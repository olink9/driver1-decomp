/* Driver 2: DrawWheelObject (CARS.C) */
typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long D_00097530;
#define current (*(DB **)&D_00097530)
extern long long D_000971A0;
#define combointensity (*(unsigned int *)&D_000971A0)
extern unsigned short D_00087030[][32];
extern unsigned short D_00086F30[];

typedef struct { short vx, vy, vz, pad; } SVECTOR;

typedef struct {
    unsigned char id, texture_set;
    unsigned short texture_id;
    unsigned char v0, v1, v2, v3;
    unsigned short uv0, uv1, uv2, uv3;
    int color;
} POLYFT4;

typedef struct {
    char pad[0xE];
    unsigned short num_polys;
    int pad2;
    POLYFT4 *poly_block;
} MODEL;

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned int uv0clut;
    short x1, y1;
    unsigned int uv1tpage;
    short x2, y2;
    unsigned int uv2;
    short x3, y3;
    unsigned int uv3;
} PFT4;

#define gte_ldv3(r0, r1, r2) __asm__ volatile ( \
    "lwc2  $0, 0( %0 );" "lwc2  $1, 4( %0 );" "lwc2  $2, 0( %1 );" \
    "lwc2  $3, 4( %1 );" "lwc2  $4, 0( %2 );" "lwc2  $5, 4( %2 )" : : "r"(r0), "r"(r1), "r"(r2))
#define gte_ldv0(r0) __asm__ volatile ("lwc2  $0, 0( %0 );" "lwc2  $1, 4( %0 )" : : "r"(r0))
#define gte_rtpt() __asm__ volatile ("nop;" "nop;" "rtpt")
#define gte_rtps() __asm__ volatile ("nop;" "nop;" "rtps")
#define gte_nclip() __asm__ volatile ("nop;" "nop;" "nclip")
#define gte_avsz4() __asm__ volatile ("nop;" "nop;" "avsz4")
#define gte_stopz(r0) __asm__ volatile ("swc2  $24, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stotz(r0) __asm__ volatile ("swc2  $7, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy0(r0) __asm__ volatile ("swc2  $12, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy1(r0) __asm__ volatile ("swc2  $13, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy2(r0) __asm__ volatile ("swc2  $14, 0( %0 )" : : "r"(r0) : "memory")

typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
#define setaddr(_p, _addr) (((P_TAG *)(_p))->addr = (unsigned long)(_addr))
#define getaddr(_p) (unsigned long)(((P_TAG *)(_p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
#define setlen(p, _len) (((P_TAG *)(p))->len = (unsigned char)(_len))

void func_0003B748(MODEL *model, SVECTOR *verts, int transparent)
{
    PFT4 *prims;
    POLYFT4 *src;
    int i;
    int Z;
    unsigned int clut;
    unsigned int tpage;

    src = model->poly_block;
    prims = (PFT4 *)current->primptr;
    i = model->num_polys;
    clut = D_00087030[src->texture_set][src->texture_id] << 16;
    tpage = (D_00086F30[src->texture_set] | 0x20) << 16;

    while (--i != -1) {
        int flag1;
        long vl;

        vl = *(long *)&src->v0;
        gte_ldv3(&verts[vl & 0xFF], &verts[(vl >> 8) & 0xFF], &verts[(vl >> 16) & 0xFF]);
        gte_rtpt();
        gte_nclip();
        gte_stopz(&flag1);
        gte_stsxy0(&prims->x0);
        gte_ldv0(&verts[(unsigned long)vl >> 24]);
        gte_rtps();
        gte_avsz4();
        gte_stotz(&Z);

        if (Z > 2) {
            unsigned int tmp;
            Z >>= 1;
            Z = (int)(current->ot + Z + 9);
            tmp = *(unsigned int *)Z;
            *(unsigned int *)Z = (unsigned int)prims & 0xFFFFFF;
            prims->tag = (tmp & 0xFFFFFF) | 0x09000000;

            if (i < 2 || flag1 < 0)
                *(unsigned int *)&prims->r0 = 0x2C000000;
            else
                *(unsigned int *)&prims->r0 = (combointensity & 0xFFFFFF) | 0x2C000000;

            if (transparent)
                prims->code |= 2;
            else
                prims->code &= ~2;

            prims->uv0clut = src->uv0 | clut;
            gte_stsxy0(&prims->x1);
            prims->uv1tpage = src->uv1 | tpage;
            gte_stsxy2(&prims->x2);
            prims->uv2 = src->uv3;
            gte_stsxy1(&prims->x3);
            prims->uv3 = src->uv2;
            prims++;
        }
        src++;
    }
    current->primptr = (char *)prims;
}
