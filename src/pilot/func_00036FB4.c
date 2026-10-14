/* Flat black triangle (POLY_F3) from scratchpad vertex list, optional semi-trans with dummy POLY_FT3 tpage reset */
typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long D_00097530;
#define current (*(DB **)&D_00097530)
extern long long D_00096978;
#define semitrans (*(unsigned char *)&D_00096978)
extern int func_0007B1E8();
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
#define setaddr(_p, _addr) (((P_TAG *)(_p))->addr = (unsigned long)(_addr))
#define getaddr(_p) (unsigned long)(((P_TAG *)(_p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)

typedef struct {
    unsigned char id, v0, v1, v2;
} POLYF3SRC;

typedef struct {
    unsigned int tag;
    unsigned int rgb0;
    short x0, y0;
    short x1, y1;
    short x2, y2;
} PF3;

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short x1, y1;
    unsigned char u1, v1;
    unsigned short tpage;
    short x2, y2;
    unsigned char u2, v2;
    unsigned short pad1;
} PFT3;

void func_00036FB4(void)
{
    POLYF3SRC *src;
    short *zbuff;
    int *outlongs;
    PF3 *prims;
    PFT3 *tp;
    int z;

    src = *(POLYF3SRC **)0x1F80003C;
    outlongs = (int *)0x1F800044;
    zbuff = *(short **)0x1F80001C;
    prims = (PF3 *)current->primptr;

    if (func_0007B1E8(outlongs[src->v0], outlongs[src->v1], outlongs[src->v2]) > 0) {
        ((unsigned char *)prims)[3] = 4;
        prims->rgb0 = 0;
        ((unsigned char *)prims)[7] = 0x20;
        prims->x0 = ((short *)&outlongs[src->v0])[0];
        prims->y0 = ((short *)&outlongs[src->v0])[1];
        prims->x1 = ((short *)&outlongs[src->v1])[0];
        prims->y1 = ((short *)&outlongs[src->v1])[1];
        prims->x2 = ((short *)&outlongs[src->v2])[0];
        prims->y2 = ((short *)&outlongs[src->v2])[1];
        z = (zbuff[src->v0] + zbuff[src->v1] + zbuff[src->v2]) / 3;
        if (semitrans)
            ((unsigned char *)prims)[7] |= 2;
        else
            ((unsigned char *)prims)[7] &= ~2;
        addPrim(current->ot + z, prims);
        current->primptr += 0x14;

        if (semitrans) {
            tp = (PFT3 *)current->primptr;
            ((unsigned char *)tp)[3] = 7;
            tp->code = 0x24;
            tp->x0 = -1;
            tp->y0 = -1;
            tp->x1 = -1;
            tp->y1 = -1;
            tp->x2 = -1;
            tp->y2 = -1;
            tp->tpage = 0x20;
            addPrim(current->ot + z, tp);
            current->primptr += 0x20;
        }
    }
}
