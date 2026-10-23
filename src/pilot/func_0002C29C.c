/* semi-transparent textured quad (POLY_FT4) from 4 model vertices */
typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long current;
#define current (*(DB **)&current)
extern unsigned short texture_cluts[][32];
extern unsigned short texture_pages[];

typedef struct { short vx, vy, vz, pad; } SVECTOR;

typedef struct {
    unsigned char id, page;
    unsigned short clut;
    unsigned char v[4];
    unsigned short uv[4];
} POLYFT4SRC;

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned int uv0;
    short x1, y1;
    unsigned int uv1;
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
#define gte_stsz(r0) __asm__ volatile ("swc2  $19, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy0(r0) __asm__ volatile ("swc2  $12, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy1(r0) __asm__ volatile ("swc2  $13, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy2(r0) __asm__ volatile ("swc2  $14, 0( %0 )" : : "r"(r0) : "memory")

#define gte_stsxy3(r0, r1, r2) __asm__ volatile ( \
    "swc2  $12, 0( %0 );" "swc2  $13, 0( %1 );" "swc2  $14, 0( %2 )" : : "r"(r0), "r"(r1), "r"(r2) : "memory")

typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;

void func_0002C29C(POLYFT4SRC *src, short *verts)
{
    SVECTOR v[4];
    char pad[0x30];
    long z0;
    long z;
    PFT4 *prims;
    int zz;
    unsigned char page;
    int clut, tpage;

    v[0].vz = *(short *)((char *)verts + src->v[0] * 8);
    v[1].vz = *(short *)((char *)verts + src->v[1] * 8);
    v[2].vz = *(short *)((char *)verts + src->v[2] * 8);
    v[3].vz = *(short *)((char *)verts + src->v[3] * 8);
    v[0].vx = ((short *)((char *)verts + src->v[0] * 8))[2] - ((short *)((char *)verts + src->v[0] * 8))[1];
    v[1].vx = ((short *)((char *)verts + src->v[1] * 8))[2] - ((short *)((char *)verts + src->v[1] * 8))[1];
    v[2].vx = ((short *)((char *)verts + src->v[2] * 8))[2] - ((short *)((char *)verts + src->v[2] * 8))[1];
    v[3].vx = ((short *)((char *)verts + src->v[3] * 8))[2] - ((short *)((char *)verts + src->v[3] * 8))[1];
    v[3].vy = 0;
    v[2].vy = 0;
    v[1].vy = 0;
    v[0].vy = 0;

    gte_ldv0(&v[0]);
    gte_rtps();
    prims = (PFT4 *)current->primptr;
    prims->code = 0x2C;
    ((unsigned char *)prims)[3] = 9;
    prims->r0 = 0;
    prims->g0 = 0;
    prims->b0 = 0;
    prims->code |= 2;
    gte_stsz(&z0);
    gte_stsxy2(&prims->x3);
    gte_ldv3(&v[2], &v[3], &v[1]);
    gte_rtpt();
    page = src->page;
    clut = texture_cluts[page][src->clut] << 16;
    tpage = texture_pages[page] << 16;
    prims->uv0 = src->uv[2] | clut;
    prims->uv1 = src->uv[3] | tpage;
    prims->uv2 = src->uv[1];
    prims->uv3 = src->uv[0];
    gte_stsz(&z);
    zz = z >> 3;
    ((P_TAG *)prims)->addr = ((P_TAG *)&current->ot[zz + 256])->addr;
    ((P_TAG *)&current->ot[zz + 256])->addr = (unsigned int)prims;
    current->primptr += 0x28;
    gte_stsxy3(&prims->x0, &prims->x1, &prims->x2);
}
