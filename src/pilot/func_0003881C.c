/* Textured quad (POLY_FT4) plus a semi-transparent overlay POLY_FT4 of grey level col */
typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long current;
#define current (*(DB **)&current)
extern long long combointensity;
extern unsigned short texture_cluts[][32];
extern unsigned short texture_pages[];
extern unsigned char *D_000972F0;
extern long long D_000974B4;
extern long long D_000974B8;
extern int NormalClip();
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
#define setaddr(_p, _addr) (((P_TAG *)(_p))->addr = (unsigned long)(_addr))
#define getaddr(_p) (unsigned long)(((P_TAG *)(_p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)

typedef struct {
    unsigned char id, texture_set;
    unsigned short texture_id;
    unsigned char v0, v1, v2, v3;
    unsigned char e0, e1, e2, e3;
    unsigned char u0, v0_, u1, v1_, u2, v2_, u3, v3_;
} POLYFT4SRC;

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
    short x3, y3;
    unsigned char u3, v3;
    unsigned short pad2;
} PFT4;

void func_0003881C(unsigned char col)
{
    POLYFT4SRC *src;
    short *zbuff;
    int *outlongs;
    PFT4 *prims;
    unsigned char *uvtab;
    unsigned char off;
    int z;
    int ts;

    src = *(POLYFT4SRC **)0x1F80003C;
    outlongs = (int *)0x1F800044;
    zbuff = *(short **)0x1F80001C;
    prims = (PFT4 *)current->primptr;
    uvtab = *(unsigned char **)0x1F800038;

    if (NormalClip(outlongs[src->v0], outlongs[src->v1], outlongs[src->v2]) > 0) {
        *(int *)&prims->r0 = *(int *)&combointensity;
        ((unsigned char *)prims)[3] = 9;
        ((unsigned char *)&prims[1])[3] = 9;
        prims[1].r0 = col;
        prims[1].g0 = col;
        prims[1].b0 = col;
        prims[1].code = 0x2C;
        prims->code = 0x2C;
        prims->x0 = ((short *)&outlongs[src->v0])[0];
        prims->y0 = ((short *)&outlongs[src->v0])[1];
        prims->x1 = ((short *)&outlongs[src->v1])[0];
        prims->y1 = ((short *)&outlongs[src->v1])[1];
        prims->x2 = ((short *)&outlongs[src->v3])[0];
        prims->y2 = ((short *)&outlongs[src->v3])[1];
        prims->x3 = ((short *)&outlongs[src->v2])[0];
        prims->y3 = ((short *)&outlongs[src->v2])[1];
        prims[1].x0 = ((short *)&outlongs[src->v0])[0];
        prims[1].y0 = ((short *)&outlongs[src->v0])[1];
        prims[1].x1 = ((short *)&outlongs[src->v1])[0];
        prims[1].y1 = ((short *)&outlongs[src->v1])[1];
        prims[1].x2 = ((short *)&outlongs[src->v3])[0];
        prims[1].y2 = ((short *)&outlongs[src->v3])[1];
        prims[1].x3 = ((short *)&outlongs[src->v2])[0];
        prims[1].y3 = ((short *)&outlongs[src->v2])[1];
        off = *D_000972F0;
        prims->u0 = src->u0 + off;
        prims->v0 = src->v0_;
        prims->u1 = src->u1 + off;
        prims->v1 = src->v1_;
        prims->u2 = src->u3 + off;
        prims->v2 = src->v3_;
        prims->u3 = src->u2 + off;
        prims->v3 = src->v2_;
        prims[1].u0 = uvtab[src->e0 * 2];
        prims[1].v0 = uvtab[src->e0 * 2 + 1];
        prims[1].u1 = uvtab[src->e1 * 2];
        prims[1].v1 = uvtab[src->e1 * 2 + 1];
        prims[1].u2 = uvtab[src->e3 * 2];
        prims[1].v2 = uvtab[src->e3 * 2 + 1];
        prims[1].u3 = uvtab[src->e2 * 2];
        prims[1].v3 = uvtab[src->e2 * 2 + 1];
        prims->clut = texture_cluts[src->texture_set][src->texture_id];
        prims->tpage = texture_pages[src->texture_set];
        ts = *(int *)&D_000974B4;
        prims[1].clut = texture_cluts[ts][*(int *)&D_000974B8];
        prims[1].tpage = texture_pages[ts] | 0x20;
        prims[1].code |= 2;
        z = (zbuff[src->v0] + zbuff[src->v1] + zbuff[src->v2] + zbuff[src->v3]) / 4;
        addPrim(current->ot + z, &prims[1]);
        addPrim(current->ot + z, prims);
        current->primptr += 0x50;
    }
}
