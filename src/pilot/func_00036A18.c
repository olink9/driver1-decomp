/* gouraud textured triangle (POLY_GT3), same family as func_00035DA8 */
typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long D_00097530;
#define current (*(DB **)&D_00097530)
extern long long D_000971A0;
extern unsigned short D_00087030[][32];
extern unsigned short D_00086F30[];
extern int func_0007B1E8();

typedef struct {
    unsigned char id, page;
    unsigned short clut;
    unsigned char v0, v1, v2, v3;
    unsigned char uv0[2], uv1[2], uv2[2];
} POLYGT3SRC;

typedef struct {
    unsigned int tag;
    unsigned int rgb0;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    unsigned int rgb1;
    short x1, y1;
    unsigned char u1, v1;
    unsigned short tpage;
    unsigned int rgb2;
    short x2, y2;
    unsigned char u2, v2;
    unsigned short pad;
} PGT3;

typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
void func_00036A18(void)
{
    POLYGT3SRC *src;
    short *zbuff;
    int *outlongs;
    PGT3 *prims;
    int z;

    src = *(POLYGT3SRC **)0x1F80003C;
    outlongs = (int *)0x1F800044;
    zbuff = *(short **)0x1F80001C;
    prims = (PGT3 *)current->primptr;

    if (func_0007B1E8(outlongs[src->v0], outlongs[src->v1], outlongs[src->v2]) > 0) {
        prims->rgb0 = *(int *)&D_000971A0;
        prims->rgb1 = *(int *)&D_000971A0;
        prims->rgb2 = *(int *)&D_000971A0;
        ((unsigned char *)prims)[3] = 9;
        ((unsigned char *)prims)[7] = 0x34;
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
        prims->clut = D_00087030[src->page][src->clut];
        prims->tpage = D_00086F30[src->page];
        z = (zbuff[src->v0] + zbuff[src->v1] + zbuff[src->v2]) / 3;
        ((P_TAG *)prims)->addr = ((P_TAG *)&current->ot[z])->addr;
        ((P_TAG *)&current->ot[z])->addr = (unsigned int)prims;
        current->primptr += 0x28;
    }
}
