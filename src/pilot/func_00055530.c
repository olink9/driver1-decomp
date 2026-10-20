/* Driver 2-like: draw a TILE_1 dot at a transformed world position */
typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long D_00097530;
#define current (*(DB **)&D_00097530)
typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
#define setaddr(_p, _addr) (((P_TAG *)(_p))->addr = (unsigned long)(_addr))
#define getaddr(_p) (unsigned long)(((P_TAG *)(_p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
typedef struct { unsigned int tag; unsigned char r0, g0, b0, code; short x0, y0; } TILE_1;
extern void func_000539C8();
void func_00055530(VECTOR *pos, int col)
{
    VECTOR v;
    TILE_1 *t;
    func_000539C8(pos, &v, 1, 0, 0);
    t = (TILE_1 *)current->primptr;
    ((P_TAG *)t)->len = 2;
    *(int *)&t->r0 = col;
    t->code = 0x68;
    t->x0 = v.vx;
    t->y0 = v.vz;
    addPrim(current->ot, t);
    current->primptr += sizeof(TILE_1);
}
