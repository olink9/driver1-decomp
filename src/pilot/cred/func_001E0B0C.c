typedef struct { unsigned int tag; unsigned char r0, g0, b0, code; short x0, y0, w, h; } TILE;
typedef struct { char *primptr; unsigned int *ot; } DB;
typedef struct { short x, y, w, h; } BOX;
extern DB *current;
extern BOX D_001E1F48[];

typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (unsigned)(_addr))
#define getaddr(p) (unsigned)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)

void func_001E0B0C(int x, int y)
{
    TILE *p;
    int i;

    p = (TILE *)current->primptr;
    for (i = 0; i < 4; i++) {
        ((unsigned char *)&p->tag)[3] = 3;
        p->code = 0x60;
        p->r0 = 0xFA;
        p->g0 = 0xFA;
        p->b0 = 0xFA;
        p->x0 = D_001E1F48[i].x + x;
        p->y0 = D_001E1F48[i].y + y;
        p->w = D_001E1F48[i].w;
        p->h = D_001E1F48[i].h;
        addPrim(&current->ot[4], p);
        p++;
    }
    current->primptr = (char *)p;
}
