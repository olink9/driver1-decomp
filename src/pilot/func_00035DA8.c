/* flat black quad (POLY_F4), same family as func_00035FF4 */
typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long current;
#define current (*(DB **)&current)
extern int NormalClip();

typedef struct {
    unsigned char id, pad[3], v0, v1, v2, v3;
} POLYF4SRC;

typedef struct {
    unsigned int tag;
    unsigned int rgb0;
    short x0, y0;
    short x1, y1;
    short x2, y2;
    short x3, y3;
} PF4;

typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
void func_00035DA8(void)
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

    if (NormalClip(outlongs[src->v0], outlongs[src->v1], outlongs[src->v2]) > 0) {
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
        ((P_TAG *)prims)->addr = ((P_TAG *)&current->ot[z])->addr;
        ((P_TAG *)&current->ot[z])->addr = (unsigned int)prims;
        current->primptr += 0x18;
    }
}
