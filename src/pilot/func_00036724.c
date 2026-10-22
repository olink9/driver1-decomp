/* Textured quad (POLY_FT4) from scratchpad vertex list, colour from combointensity */
typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long current;
#define current (*(DB **)&current)
extern long long combointensity;
extern unsigned short texture_cluts[][32];
extern unsigned short texture_pages[];
extern int NormalClip();
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
#define setaddr(_p, _addr) (((P_TAG *)(_p))->addr = (unsigned long)(_addr))
#define getaddr(_p) (unsigned long)(((P_TAG *)(_p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)

typedef struct {
    unsigned char id, texture_set;
    unsigned short texture_id;
    unsigned char v0, v1, v2, v3;
    unsigned char uv0[2], uv1[2], uv2[2], uv3[2];
} POLYFT4SRC;

typedef struct {
    unsigned int tag;
    unsigned int rgb0;
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

void func_00036724(void)
{
    POLYFT4SRC *src;
    short *zbuff;
    int *outlongs;
    PFT4 *prims;
    int z;

    src = *(POLYFT4SRC **)0x1F80003C;
    outlongs = (int *)0x1F800044;
    zbuff = *(short **)0x1F80001C;
    prims = (PFT4 *)current->primptr;

    if (NormalClip(outlongs[src->v0], outlongs[src->v1], outlongs[src->v2]) > 0) {
        prims->rgb0 = *(int *)&combointensity;
        ((unsigned char *)prims)[3] = 9;
        ((unsigned char *)prims)[7] = 0x2C;
        prims->x0 = ((short *)&outlongs[src->v0])[0];
        prims->y0 = ((short *)&outlongs[src->v0])[1];
        prims->x1 = ((short *)&outlongs[src->v1])[0];
        prims->y1 = ((short *)&outlongs[src->v1])[1];
        prims->x2 = ((short *)&outlongs[src->v3])[0];
        prims->y2 = ((short *)&outlongs[src->v3])[1];
        prims->x3 = ((short *)&outlongs[src->v2])[0];
        prims->y3 = ((short *)&outlongs[src->v2])[1];
        prims->u0 = src->uv0[0];
        prims->v0 = src->uv0[1];
        prims->u1 = src->uv1[0];
        prims->v1 = src->uv1[1];
        prims->u2 = src->uv3[0];
        prims->v2 = src->uv3[1];
        prims->u3 = src->uv2[0];
        prims->v3 = src->uv2[1];
        prims->clut = texture_cluts[src->texture_set][src->texture_id];
        prims->tpage = texture_pages[src->texture_set];
        z = (zbuff[src->v0] + zbuff[src->v1] + zbuff[src->v2] + zbuff[src->v3]) / 4;
        addPrim(current->ot + z, prims);
        current->primptr += 0x28;
    }
}
