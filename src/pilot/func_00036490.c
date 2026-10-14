/* func_00036490 - flat textured triangle (POLY_FT3), sibling of func_00035FF4 */
typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long D_00097530;
#define current (*(DB **)&D_00097530)
extern long long D_000971A0;
extern unsigned short D_00087030[][32]; /* texture_cluts */
extern unsigned short D_00086F30[];     /* texture_pages */
extern int func_0007B1E8();

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

void func_00036490(void)
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

    if (func_0007B1E8(outlongs[src->v0], outlongs[src->v1], outlongs[src->v2]) > 0) {
        prims->rgb = *(int *)&D_000971A0;
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
        prims->clut = D_00087030[src->texture_set][src->texture_id];
        prims->tpage = D_00086F30[src->texture_set];
        z = (zbuff[src->v0] + zbuff[src->v1] + zbuff[src->v2]) / 3;
        prims->tag = (prims->tag & 0xFF000000) | (current->ot[z] & 0xFFFFFF);
        current->ot[z] = (current->ot[z] & 0xFF000000) | ((unsigned int)prims & 0xFFFFFF);
        current->primptr += 0x20;
    }
}
