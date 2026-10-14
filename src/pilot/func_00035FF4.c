/* wjmDraw3-like flat-gouraud triangle (POLY_G3) */
typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long D_00097530;
#define current (*(DB **)&D_00097530)
extern int func_0007B1E8();

typedef struct {
    unsigned char id, v0, v1, v2;
    unsigned int color0, color1, color2;
} POLYFG3;

typedef struct {
    unsigned int tag;
    unsigned int rgb0;
    short x0, y0;
    unsigned int rgb1;
    short x1, y1;
    unsigned int rgb2;
    short x2, y2;
} PG3;

void func_00035FF4(void)
{
    POLYFG3 *src;
    short *zbuff;
    int *outlongs;
    PG3 *prims;
    int z;

    src = *(POLYFG3 **)0x1F80003C;
    outlongs = (int *)0x1F800044;
    zbuff = *(short **)0x1F80001C;
    prims = (PG3 *)current->primptr;

    if (func_0007B1E8(outlongs[src->v0], outlongs[src->v1], outlongs[src->v2]) > 0) {
        prims->rgb0 = src->color0;
        prims->rgb1 = src->color1;
        prims->rgb2 = src->color2;
        ((unsigned char *)prims)[3] = 6;
        ((unsigned char *)prims)[7] = 0x30;
        prims->x0 = ((short *)&outlongs[src->v0])[0];
        prims->y0 = ((short *)&outlongs[src->v0])[1];
        prims->x1 = ((short *)&outlongs[src->v1])[0];
        prims->y1 = ((short *)&outlongs[src->v1])[1];
        prims->x2 = ((short *)&outlongs[src->v2])[0];
        prims->y2 = ((short *)&outlongs[src->v2])[1];
        z = (zbuff[src->v0] + zbuff[src->v1] + zbuff[src->v2]) / 3;
        prims->tag = (prims->tag & 0xFF000000) | (current->ot[z] & 0xFFFFFF);
        current->ot[z] = (current->ot[z] & 0xFF000000) | ((unsigned int)prims & 0xFFFFFF);
        current->primptr += 0x1C;
    }
}
