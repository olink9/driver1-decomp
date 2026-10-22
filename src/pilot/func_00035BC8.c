/* Flat triangle (POLY_F3) from scratchpad vertex list, black */
typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long current;
#define current (*(DB **)&current)
extern int NormalClip();
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
#define setaddr(_p, _addr) (((P_TAG *)(_p))->addr = (unsigned long)(_addr))
#define getaddr(_p) (unsigned long)(((P_TAG *)(_p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)

typedef struct {
    unsigned char id, a, b, c;
} POLYF3SRC;

typedef struct {
    unsigned int tag;
    unsigned int rgb0;
    short x0, y0;
    short x1, y1;
    short x2, y2;
} PF3;

void func_00035BC8(void)
{
    POLYF3SRC *src;
    short *zbuff;
    int *outlongs;
    PF3 *prims;
    int z;

    src = *(POLYF3SRC **)0x1F80003C;
    outlongs = (int *)0x1F800044;
    zbuff = *(short **)0x1F80001C;
    prims = (PF3 *)current->primptr;

    if (NormalClip(outlongs[src->a], outlongs[src->b], outlongs[src->c]) > 0) {
        prims->rgb0 = 0;
        z = (zbuff[src->a] + zbuff[src->b] + zbuff[src->c]) / 3;
        ((unsigned char *)prims)[3] = 4;
        ((unsigned char *)prims)[7] = 0x20;
        prims->x0 = ((short *)&outlongs[src->a])[0];
        prims->y0 = ((short *)&outlongs[src->a])[1];
        prims->x1 = ((short *)&outlongs[src->b])[0];
        prims->y1 = ((short *)&outlongs[src->b])[1];
        prims->x2 = ((short *)&outlongs[src->c])[0];
        prims->y2 = ((short *)&outlongs[src->c])[1];
        addPrim(current->ot + z, prims);
        current->primptr += 0x14;
    }
}
