/* textured quad (POLY_FT4) split into two halves with midpoint vertices */
typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long current;
#define current (*(DB **)&current)
extern unsigned short texture_cluts[][32];
extern unsigned short texture_pages[];
extern int D_000890DC[];

typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { unsigned char u, v; } UVS;

typedef struct {
    unsigned char id, page;
    unsigned short clut;
    unsigned char v[4];
    UVS uv[4];
} POLYFT4SRC;

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    int xy0;
    unsigned int uv0;
    int xy1;
    unsigned int uv1;
    int xy2;
    unsigned int uv2;
    int xy3;
    unsigned int uv3;
} PFT4;

typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;

#define gte_ldv3c(r0) __asm__ volatile ( \
    "lwc2  $0, 0( %0 );" "lwc2  $1, 4( %0 );" "lwc2  $2, 8( %0 );" \
    "lwc2  $3, 12( %0 );" "lwc2  $4, 16( %0 );" "lwc2  $5, 20( %0 )" : : "r"(r0))
#define gte_rtpt() __asm__ volatile ("nop;" "nop;" "rtpt")
#define gte_stsxy3c(r0) __asm__ volatile ( \
    "swc2  $12, 0( %0 );" "swc2  $13, 4( %0 );" "swc2  $14, 8( %0 )" : : "r"(r0) : "memory")
#define gte_stsz3c(r0) __asm__ volatile ( \
    "swc2  $17, 0( %0 );" "swc2  $18, 4( %0 );" "swc2  $19, 8( %0 )" : : "r"(r0) : "memory")

void func_0002BDB0(POLYFT4SRC *src, short *verts)
{
    SVECTOR v[6];
    int xy[6];
    int z[6];
    char pad[0x30];
    UVS uv[6];
    int i, j;
    PFT4 *prim;
    unsigned char page;
    int clut, tpage;
    int zz;

    page = src->page;
    clut = texture_cluts[page][src->clut] << 16;
    tpage = texture_pages[page] << 16;

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

    gte_ldv3c(&v[0]);
    gte_rtpt();
    v[4].vx = (v[0].vx + v[1].vx) >> 1;
    v[5].vx = (v[2].vx + v[3].vx) >> 1;
    v[4].vz = (v[0].vz + v[1].vz) >> 1;
    v[5].vz = (v[2].vz + v[3].vz) >> 1;
    v[5].vy = 0;
    v[4].vy = 0;
    v[5].vz = 0;
    v[4].vz = 0;
    gte_stsxy3c(&xy[0]);
    gte_stsz3c(&z[0]);
    gte_ldv3c(&v[3]);
    gte_rtpt();
    uv[0] = src->uv[0];
    uv[1] = src->uv[1];
    uv[2] = src->uv[2];
    uv[3] = src->uv[3];
    uv[4].u = (uv[0].u + uv[1].u) >> 1;
    uv[4].v = (uv[0].v + uv[1].v) >> 1;
    uv[5].u = (uv[2].u + uv[3].u) >> 1;
    uv[5].v = (uv[2].v + uv[3].v) >> 1;
    gte_stsxy3c(&xy[3]);
    gte_stsz3c(&z[3]);

    prim = (PFT4 *)current->primptr;
    for (i = j = 0; i < 2; i++, j += 4) {
        prim->code = 0x2C;
        ((unsigned char *)prim)[3] = 9;
        prim->r0 = 0;
        prim->g0 = 0;
        prim->b0 = 0;
        prim->code |= 2;
        prim->xy0 = xy[D_000890DC[j + 0]];
        prim->xy1 = xy[D_000890DC[j + 1]];
        prim->xy2 = xy[D_000890DC[j + 2]];
        prim->xy3 = xy[D_000890DC[j + 3]];
        zz = (z[D_000890DC[j + 0]] + z[D_000890DC[j + 1]] + z[D_000890DC[j + 2]] + z[D_000890DC[j + 3]]) >> 5;
        *(unsigned int *)((char *)prim + 12) = *(unsigned short *)&uv[D_000890DC[j + 0]] | clut;
        *(unsigned int *)((char *)prim + 20) = *(unsigned short *)&uv[D_000890DC[j + 1]] | tpage;
        *(unsigned int *)((char *)prim + 28) = *(unsigned short *)&uv[D_000890DC[j + 2]];
        *(unsigned int *)((char *)prim + 36) = *(unsigned short *)&uv[D_000890DC[j + 3]];
        ((P_TAG *)prim)->addr = ((P_TAG *)&current->ot[zz + 256])->addr;
        ((P_TAG *)&current->ot[zz + 256])->addr = (unsigned int)prim;
        prim++;
    }
    current->primptr = (char *)prim;
}
