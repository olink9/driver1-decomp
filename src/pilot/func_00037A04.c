/* func_00037A04 - flat textured triangle (POLY_FT3), like func_00036490 but skips z <= 0 */
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long current;
#define current (*(DB **)&current)
extern long long combointensity;
extern unsigned short texture_cluts[][32]; /* texture_cluts */
extern unsigned short texture_pages[];     /* texture_pages */
extern int NormalClip();

typedef struct {
    unsigned char id, texture_set;
    unsigned short texture_id;
    unsigned char v0, v1, v2, pad;
    unsigned char uv0[2], uv1[2], uv2[2];
} POLYFT3;

typedef struct {
    unsigned int tag;
    unsigned int rgb;
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

void func_00037A04(void)
{
    POLYFT3 *src;
    short *zbuff;
    int *outlongs;
    PFT3 *prims;
    int z;

    src = *(POLYFT3 **)0x1F80003C;
    outlongs = (int *)0x1F800044;
    zbuff = *(short **)0x1F80001C;
    prims = (PFT3 *)current->primptr;

    if (NormalClip(outlongs[src->v0], outlongs[src->v1], outlongs[src->v2]) > 0) {
        prims->rgb = *(int *)&combointensity;
        ((unsigned char *)prims)[3] = 7;
        ((unsigned char *)prims)[7] = 0x24;
        prims->x0 = ((short *)&outlongs[src->v0])[0];
        prims->y0 = ((short *)&outlongs[src->v0])[1];
        prims->x1 = ((short *)&outlongs[src->v1])[0];
        prims->y1 = ((short *)&outlongs[src->v1])[1];
        prims->x2 = ((short *)&outlongs[src->v2])[0];
        prims->y2 = ((short *)&outlongs[src->v2])[1];
        prims->u0 = src->uv0[0];
        prims->v0 = src->uv0[1];
        prims->u1 = src->uv1[0];
        prims->v1 = src->uv1[1];
        prims->u2 = src->uv2[0];
        prims->v2 = src->uv2[1];
        prims->clut = texture_cluts[src->texture_set][src->texture_id];
        prims->tpage = texture_pages[src->texture_set];
        z = (zbuff[src->v0] + zbuff[src->v1] + zbuff[src->v2]) / 3;
        if (z > 0) {
        ((P_TAG *)prims)->addr = ((P_TAG *)&current->ot[z])->addr;
        ((P_TAG *)&current->ot[z])->addr = (unsigned int)prims;
        current->primptr += 0x20;
        }
    }
}
