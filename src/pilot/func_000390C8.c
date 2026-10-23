/* POLY_GT4 draw with per-vertex xy/uv split (wjmDraw4-like) */
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long current;
#define current (*(DB **)&current)
extern long long combointensity;
extern unsigned short texture_cluts[][32];
extern unsigned short texture_pages[];
extern unsigned char *D_000972F0;
extern long long D_00096990;
extern long long D_000975CC;
extern int D_000BE9FC[][30][5];
extern int NormalClip();

typedef struct {
    unsigned char id, texture_set;
    unsigned short texture_id;
    unsigned char v0, v1, v2, v3;
    int pad;
    unsigned char u0, v0_, u1, v1_, u2, v2_, u3, v3_;
} POLYFT4;

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
} PGT4;

void func_000390C8(void)
{
    POLYFT4 *src;
    short *zbuff;
    short *outlongs;
    PGT4 *prims;
    unsigned char off;
    int z;
    int col;

    src = *(POLYFT4 **)0x1F80003C;
    outlongs = (short *)0x1F800044;
    zbuff = *(short **)0x1F80001C;
    prims = (PGT4 *)current->primptr;

    if (NormalClip(((int *)outlongs)[src->v0], ((int *)outlongs)[src->v1], ((int *)outlongs)[src->v2]) > 0) {
        col = *(int *)&combointensity;
        ((unsigned char *)prims)[3] = 12;
        *(int *)&prims->r0 = col;
        *(int *)&prims->r1 = col;
        *(int *)&prims->r2 = col;
        *(int *)&prims->r3 = col;
        ((unsigned char *)prims)[7] = 0x3C;
        prims->x0 = outlongs[src->v0 * 2];
        prims->y0 = outlongs[src->v0 * 2 + 1];
        prims->x1 = outlongs[src->v1 * 2];
        prims->y1 = outlongs[src->v1 * 2 + 1];
        prims->x2 = outlongs[src->v3 * 2];
        prims->y2 = outlongs[src->v3 * 2 + 1];
        prims->x3 = outlongs[src->v2 * 2];
        prims->y3 = outlongs[src->v2 * 2 + 1];
        off = *D_000972F0;
        prims->u0 = src->u0 + off;
        prims->v0 = src->v0_;
        prims->u1 = src->u1 + off;
        prims->v1 = src->v1_;
        prims->u2 = src->u3 + off;
        prims->v2 = src->v3_;
        prims->u3 = src->u2 + off;
        prims->v3 = src->v2_;
        if (*(int *)&D_00096990 == 1)
            prims->clut = texture_cluts[src->texture_set][src->texture_id];
        else
            prims->clut = D_000BE9FC[src->texture_set - *(int *)&D_000975CC][src->texture_id][*(int *)&D_00096990 - 2];
        prims->tpage = texture_pages[src->texture_set];
        z = (zbuff[src->v0] + zbuff[src->v1] + zbuff[src->v2] + zbuff[src->v3]) / 4;
        ((P_TAG *)prims)->addr = ((P_TAG *)&current->ot[z])->addr;
        ((P_TAG *)&current->ot[z])->addr = (unsigned int)prims;
        current->primptr += 0x34;
    }
}
