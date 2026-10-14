/* Flat quad (POLY_F4) from scratchpad vertex list, black */
typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long D_00097530;
#define current (*(DB **)&D_00097530)
extern int func_0007B1E8();
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
#define setaddr(_p, _addr) (((P_TAG *)(_p))->addr = (unsigned long)(_addr))
#define getaddr(_p) (unsigned long)(((P_TAG *)(_p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)

typedef struct {
    unsigned char id, a, b, c;
    unsigned char v0, v1, v2, v3;
} POLYF4SRC;

typedef struct {
    unsigned int tag;
    unsigned int rgb0;
    short x0, y0;
    short x1, y1;
    short x2, y2;
    short x3, y3;
} PF4;

void func_00037274(void)
{
    POLYF4SRC *src;
    short *zbuff;
    int *outlongs;
    PF4 *prims;
    int z;

    src = *(POLYF4SRC **)0x1F80003C;
    outlongs = (int *)0x1F800044;
    zbuff = *(short **)0x1F80001C;
    prims = (PF4 *)current->primptr;

    if (func_0007B1E8(outlongs[src->v0], outlongs[src->v1], outlongs[src->v2]) > 0) {
        ((unsigned char *)prims)[3] = 5;
        prims->rgb0 = 0;
        ((unsigned char *)prims)[7] = 0x28;
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
        current->primptr += 0x18;
    }
}
