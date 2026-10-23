/* PlotSkyPoly (Driver 2 name) */
typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long current;
#define current (*(DB **)&current)
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
#define setaddr(_p, _addr) (((P_TAG *)(_p))->addr = (unsigned long)(_addr))
#define getaddr(_p) (unsigned long)(((P_TAG *)(_p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)

typedef struct { short vx, vy; } DVECTOR;
typedef struct {
    unsigned char id, pad0[3];
    unsigned char v0, v1, v2, v3;
} POLYFT4;
typedef struct { unsigned char u0, v0, u1, v1, u2, v2, u3, v3; } UVT;

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
    short x3, y3;
    unsigned char u3, v3;
    unsigned short pad2;
} PFT4;

extern unsigned char D_000974F0, D_000974F4, D_000974DC;
extern unsigned short D_000974F8;
extern UVT D_000BDE40[];
extern unsigned short D_000BDD20[];

void PlotSkyPoly(int skytexnum)
{
    POLYFT4 *src;
    DVECTOR *outpoints;
    PFT4 *poly;

    src = *(POLYFT4 **)0x1F80003C;
    outpoints = (DVECTOR *)0x1F800044;
    poly = (PFT4 *)current->primptr;

    if ((outpoints[src->v0].vy > -1 || outpoints[src->v1].vy > -1 || outpoints[src->v2].vy > -1 || outpoints[src->v3].vy > -1) &&
        (outpoints[src->v0].vx > -1 || outpoints[src->v1].vx > -1 || outpoints[src->v2].vx > -1 || outpoints[src->v3].vx > -1) &&
        (outpoints[src->v0].vx < 513 || outpoints[src->v1].vx < 513 || outpoints[src->v2].vx < 513 || outpoints[src->v3].vx < 513))
    {
        poly->x0 = outpoints[src->v0].vx;
        poly->y0 = outpoints[src->v0].vy;
        poly->x1 = outpoints[src->v1].vx;
        poly->y1 = outpoints[src->v1].vy;
        poly->x2 = outpoints[src->v3].vx;
        poly->y2 = outpoints[src->v3].vy;
        poly->x3 = outpoints[src->v2].vx;
        poly->y3 = outpoints[src->v2].vy;
        poly->r0 = D_000974F0;
        poly->g0 = D_000974F4;
        poly->b0 = D_000974DC;
        ((unsigned char *)poly)[3] = 9;
        poly->code = 0x2C;
        poly->x0 = outpoints[src->v0].vx;
        poly->y0 = outpoints[src->v0].vy;
        poly->x1 = outpoints[src->v1].vx;
        poly->y1 = outpoints[src->v1].vy;
        poly->x2 = outpoints[src->v3].vx;
        poly->y2 = outpoints[src->v3].vy;
        poly->x3 = outpoints[src->v2].vx;
        poly->y3 = outpoints[src->v2].vy;
        poly->u0 = D_000BDE40[skytexnum].u2;
        poly->v0 = D_000BDE40[skytexnum].v2;
        poly->u1 = D_000BDE40[skytexnum].u3;
        poly->v1 = D_000BDE40[skytexnum].v3;
        poly->u2 = D_000BDE40[skytexnum].u0;
        poly->v2 = D_000BDE40[skytexnum].v0;
        poly->u3 = D_000BDE40[skytexnum].u1;
        poly->v3 = D_000BDE40[skytexnum].v1;
        poly->clut = D_000BDD20[skytexnum];
        poly->tpage = D_000974F8;
        addPrim(current->ot + 0x107F, poly);
        current->primptr += 0x28;
    }
}
