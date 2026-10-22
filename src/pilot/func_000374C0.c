/* Gouraud-style triangle (POLY_G3, one colour) from scratchpad vertex list */
typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long current;
#define current (*(DB **)&current)
extern int NormalClip();
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
#define setaddr(_p, _addr) (((P_TAG *)(_p))->addr = (unsigned long)(_addr))
#define getaddr(_p) (unsigned long)(((P_TAG *)(_p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)

typedef struct {
    unsigned char id, v0, v1, v2;
    unsigned char pad[4];
    unsigned char r, g, b;
} POLYG3SRC;

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char r1, g1, b1, pad1;
    short x1, y1;
    unsigned char r2, g2, b2, pad2;
    short x2, y2;
} PG3;

void func_000374C0(void)
{
    POLYG3SRC *src;
    short *zbuff;
    int *outlongs;
    PG3 *prims;
    int z;

    src = *(POLYG3SRC **)0x1F80003C;
    outlongs = (int *)0x1F800044;
    zbuff = *(short **)0x1F80001C;
    prims = (PG3 *)current->primptr;

    if (NormalClip(outlongs[src->v0], outlongs[src->v1], outlongs[src->v2]) > 0) {
        prims->r0 = src->r;
        prims->g0 = src->g;
        prims->b0 = src->b;
        prims->r1 = src->r;
        prims->g1 = src->g;
        prims->b1 = src->b;
        prims->r2 = src->r;
        prims->g2 = src->g;
        prims->b2 = src->b;
        ((unsigned char *)prims)[3] = 6;
        prims->code = 0x30;
        prims->x0 = ((short *)&outlongs[src->v0])[0];
        prims->y0 = ((short *)&outlongs[src->v0])[1];
        prims->x1 = ((short *)&outlongs[src->v1])[0];
        prims->y1 = ((short *)&outlongs[src->v1])[1];
        prims->x2 = ((short *)&outlongs[src->v2])[0];
        prims->y2 = ((short *)&outlongs[src->v2])[1];
        z = (zbuff[src->v0] + zbuff[src->v1] + zbuff[src->v2]) / 3;
        addPrim(current->ot + z, prims);
        current->primptr += 0x1C;
    }
}
