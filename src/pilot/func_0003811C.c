/* Driver 2: wjmDraw4 variant (gouraud-textured quad, POLY_GT4) */
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long current;
#define current (*(DB **)&current)
extern long long combointensity;
extern unsigned short texture_cluts[][32];
extern unsigned short texture_pages[];
extern int NormalClip();

typedef struct {
    unsigned char id, texture_set;
    unsigned short texture_id;
    unsigned char v0, v1, v2, v3;
    int pad;
    unsigned short uv0, uv1, uv2, uv3;
} POLYGT4;

typedef struct {
    unsigned int tag;
    unsigned int rgb0, xy0, uv0clut;
    unsigned int rgb1, xy1, uv1tpage;
    unsigned int rgb2, xy2, uv2;
    unsigned int rgb3, xy3, uv3;
} PGT4;

void func_0003811C(void)
{
    POLYGT4 *src;
    short *zbuff;
    int *outlongs;
    PGT4 *prims;
    unsigned int clut;
    unsigned int tpage;
    int z;

    src = *(POLYGT4 **)0x1F80003C;
    outlongs = (int *)0x1F800044;
    zbuff = *(short **)0x1F80001C;
    prims = (PGT4 *)current->primptr;

    if (NormalClip(outlongs[src->v0], outlongs[src->v1], outlongs[src->v2]) > 0) {
        clut = texture_cluts[src->texture_set][src->texture_id];
        tpage = texture_pages[src->texture_set];
        prims->rgb0 = *(int *)&combointensity;
        prims->rgb1 = *(int *)&combointensity;
        prims->rgb2 = *(int *)&combointensity;
        prims->rgb3 = *(int *)&combointensity;
        ((unsigned char *)prims)[3] = 12;
        ((unsigned char *)prims)[7] = 0x3C;
        prims->xy0 = outlongs[src->v0];
        prims->uv0clut = src->uv0 | (clut << 16);
        prims->xy1 = outlongs[src->v1];
        prims->uv1tpage = src->uv1 | (tpage << 16);
        prims->xy2 = outlongs[src->v3];
        prims->uv2 = src->uv3;
        prims->xy3 = outlongs[src->v2];
        prims->uv3 = src->uv2;
        z = (zbuff[src->v0] + zbuff[src->v1] + zbuff[src->v2] + zbuff[src->v3]) / 4;
        ((P_TAG *)prims)->addr = ((P_TAG *)&current->ot[z])->addr;
        ((P_TAG *)&current->ot[z])->addr = (unsigned int)prims;
        current->primptr += 0x34;
    }
}
