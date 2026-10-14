/* Gouraud-style quad (POLY_G4, one colour) from scratchpad vertex list */
typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long D_00097530;
#define current (*(DB **)&D_00097530)
extern int func_0007B1E8();
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
#define setaddr(_p, _addr) (((P_TAG *)(_p))->addr = (unsigned long)(_addr))
#define getaddr(_p) (unsigned long)(((P_TAG *)(_p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)

typedef struct {
    unsigned char id, pad0[3];
    unsigned char v0, v1, v2, v3;
    unsigned char pad[4];
    unsigned char r, g, b;
} POLYG4SRC;

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char r1, g1, b1, pad1;
    short x1, y1;
    unsigned char r2, g2, b2, pad2;
    short x2, y2;
    unsigned char r3, g3, b3, pad3;
    short x3, y3;
} PG4;

void func_0003772C(void)
{
    POLYG4SRC *src;
    short *zbuff;
    int *outlongs;
    PG4 *prims;
    int z;

    src = *(POLYG4SRC **)0x1F80003C;
    outlongs = (int *)0x1F800044;
    zbuff = *(short **)0x1F80001C;
    prims = (PG4 *)current->primptr;

    if (func_0007B1E8(outlongs[src->v0], outlongs[src->v1], outlongs[src->v2]) > 0) {
        prims->r0 = src->r;
        prims->g0 = src->g;
        prims->b0 = src->b;
        prims->r1 = src->r;
        prims->g1 = src->g;
        prims->b1 = src->b;
        prims->r2 = src->r;
        prims->g2 = src->g;
        prims->b2 = src->b;
        prims->r3 = src->r;
        prims->g3 = src->g;
        prims->b3 = src->b;
        ((unsigned char *)prims)[3] = 8;
        prims->code = 0x38;
        prims->x0 = ((short *)&outlongs[src->v0])[0];
        prims->y0 = ((short *)&outlongs[src->v0])[1];
        prims->x1 = ((short *)&outlongs[src->v1])[0];
        prims->y1 = ((short *)&outlongs[src->v1])[1];
        prims->x2 = ((short *)&outlongs[src->v3])[0];
        prims->y2 = ((short *)&outlongs[src->v3])[1];
        prims->x3 = ((short *)&outlongs[src->v2])[0];
        prims->y3 = ((short *)&outlongs[src->v2])[1];
        z = (zbuff[src->v0] + zbuff[src->v1] + zbuff[src->v2] + zbuff[src->v3]) / 4;
        addPrim(current->ot + z, prims);
        current->primptr += 0x24;
    }
}
