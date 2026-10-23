/* POLY_FT3 face plus a semi-transparent POLY_FT3 overlay (second uv set, fixed clut/tpage) */
typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long current;
#define current (*(DB **)&current)
extern long long combointensity;
extern long long D_000974B4;
extern unsigned short texture_cluts[][32];
extern unsigned short texture_pages[];
extern unsigned char *D_000972F0;
extern int NormalClip();

typedef struct {
    unsigned char id, texture_set;
    unsigned short texture_id;
    unsigned char v0, v1, v2, spare;
    unsigned char e0, e1, e2, spare2;
    unsigned char u0, v0_, u1, v1_, u2, v2_;
} SRC;

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
    unsigned short pad;
} PFT3;

typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
void func_0003837C(unsigned char col)
{
    SRC *src;
    short *zbuff;
    short *outlongs;
    unsigned char *uvs;
    PFT3 *prims;
    unsigned char off;
    int z;
    int t;

    src = *(SRC **)0x1F80003C;
    outlongs = (short *)0x1F800044;
    zbuff = *(short **)0x1F80001C;
    prims = (PFT3 *)current->primptr;
    uvs = *(unsigned char **)0x1F800038;

    if (NormalClip(((int *)outlongs)[src->v0], ((int *)outlongs)[src->v1], ((int *)outlongs)[src->v2]) > 0) {
        *(int *)&prims->r0 = *(int *)&combointensity;
        ((unsigned char *)prims)[3] = 7;
        ((unsigned char *)prims)[0x23] = 7;
        prims[1].r0 = col;
        prims[1].g0 = col;
        prims[1].b0 = col;
        prims[1].code = 0x24;
        prims->code = 0x24;
        prims->x0 = outlongs[src->v0 * 2];
        prims->y0 = outlongs[src->v0 * 2 + 1];
        prims->x1 = outlongs[src->v1 * 2];
        prims->y1 = outlongs[src->v1 * 2 + 1];
        prims->x2 = outlongs[src->v2 * 2];
        prims->y2 = outlongs[src->v2 * 2 + 1];
        prims[1].x0 = outlongs[src->v0 * 2];
        prims[1].y0 = outlongs[src->v0 * 2 + 1];
        prims[1].x1 = outlongs[src->v1 * 2];
        prims[1].y1 = outlongs[src->v1 * 2 + 1];
        prims[1].x2 = outlongs[src->v2 * 2];
        prims[1].y2 = outlongs[src->v2 * 2 + 1];
        off = *D_000972F0;
        prims->u0 = src->u0 + off;
        prims->v0 = src->v0_;
        prims->u1 = src->u1 + off;
        prims->v1 = src->v1_;
        prims->u2 = src->u2 + off;
        prims->v2 = src->v2_;
        prims[1].u0 = uvs[src->e0 * 2];
        prims[1].v0 = uvs[src->e0 * 2 + 1];
        prims[1].u1 = uvs[src->e1 * 2];
        prims[1].v1 = uvs[src->e1 * 2 + 1];
        prims[1].u2 = uvs[src->e2 * 2];
        prims[1].v2 = uvs[src->e2 * 2 + 1];
        prims->clut = texture_cluts[src->texture_set][src->texture_id];
        prims->tpage = texture_pages[src->texture_set];
        prims[1].clut = texture_cluts[*(int *)&D_000974B4][0];
        prims[1].tpage = texture_pages[*(int *)&D_000974B4] | 0x20;
        prims[1].code |= 2;
        z = (zbuff[src->v0] + zbuff[src->v1] + zbuff[src->v2]) / 3;
        ((P_TAG *)&prims[1])->addr = ((P_TAG *)&current->ot[z])->addr;
        ((P_TAG *)&current->ot[z])->addr = (unsigned int)&prims[1];
        ((P_TAG *)prims)->addr = ((P_TAG *)&current->ot[z])->addr;
        ((P_TAG *)&current->ot[z])->addr = (unsigned int)prims;
        current->primptr += 0x40;
    }
}
