/* G4 variant of func_00035FF4 (flat-gouraud quad, POLY_G4) */
typedef struct { unsigned addr:24; unsigned len:8; } TAG;
typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long current;
#define current (*(DB **)&current)
extern int NormalClip();

typedef struct {
    unsigned char id, pad[3];
    unsigned char v0, v1, v2, v3;
    unsigned int color0, color1, color2, color3;
} POLYFG4;

typedef struct {
    unsigned int tag;
    unsigned int rgb0;
    short x0, y0;
    unsigned int rgb1;
    short x1, y1;
    unsigned int rgb2;
    short x2, y2;
    unsigned int rgb3;
    short x3, y3;
} PG4;

void func_00036218(void)
{
    POLYFG4 *src;
    short *zbuff;
    int *outlongs;
    PG4 *prims;
    int z;

    src = *(POLYFG4 **)0x1F80003C;
    outlongs = (int *)0x1F800044;
    zbuff = *(short **)0x1F80001C;
    prims = (PG4 *)current->primptr;

    if (NormalClip(outlongs[src->v0], outlongs[src->v1], outlongs[src->v2]) > 0) {
        prims->rgb0 = src->color0;
        prims->rgb1 = src->color1;
        prims->rgb2 = src->color2;
        prims->rgb3 = src->color3;
        ((unsigned char *)prims)[3] = 8;
        ((unsigned char *)prims)[7] = 0x38;
        prims->x0 = ((short *)&outlongs[src->v0])[0];
        prims->y0 = ((short *)&outlongs[src->v0])[1];
        prims->x1 = ((short *)&outlongs[src->v1])[0];
        prims->y1 = ((short *)&outlongs[src->v1])[1];
        prims->x2 = ((short *)&outlongs[src->v3])[0];
        prims->y2 = ((short *)&outlongs[src->v3])[1];
        prims->x3 = ((short *)&outlongs[src->v2])[0];
        prims->y3 = ((short *)&outlongs[src->v2])[1];
        z = (zbuff[src->v0] + zbuff[src->v1] + zbuff[src->v2] + zbuff[src->v3]) / 4;
        ((TAG *)prims)->addr = ((TAG *)&current->ot[z])->addr;
        ((TAG *)&current->ot[z])->addr = (unsigned int)prims;
        current->primptr += 0x24;
    }
}
