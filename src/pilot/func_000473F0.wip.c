/* (no Driver 2 counterpart) - HUD sprite pair + rotating overlay */

typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { short vx, vy; } DVECTOR;
typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct { char *primptr; unsigned int *ot; } DB;

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0; unsigned short clut;
    short x1, y1;
    unsigned char u1, v1; unsigned short tpage;
    short x2, y2;
    unsigned char u2, v2; unsigned short pad1;
} POLY_FT3;

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0; unsigned short clut;
    short x1, y1;
    unsigned char u1, v1; unsigned short tpage;
    short x2, y2;
    unsigned char u2, v2; unsigned short pad1;
    short x3, y3;
    unsigned char u3, v3; unsigned short pad2;
} POLY_FT4;

typedef struct {
    unsigned char u0, v0, u1, v1, u2, v2, u3, v3;
    unsigned short tpage;
    unsigned short clut;
} SPRTEX;

typedef struct { short vx, vy, vz, pad; } ANGLE8;

extern long long D_00097530;
#define current (*(DB **)&D_00097530)
extern SPRTEX D_000B9240;
extern SPRTEX D_000B9130;
extern short D_0008EAAA[];
extern long long D_00096C4A;
#define angle (*(unsigned short *)&D_00096C4A)

typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
#define setaddr(_p, _addr) (((P_TAG *)(_p))->addr = (unsigned long)(_addr))
#define getaddr(_p) (unsigned long)(((P_TAG *)(_p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)

void func_000473F0(DVECTOR *pos, int a1, int a2, int intensity)
{
    SVECTOR size;
    VECTOR r;
    POLY_FT3 *null;
    POLY_FT4 *poly;
    int idx;

    null = (POLY_FT3 *)current->primptr;
    ((P_TAG *)null)->len = 7;
    null->code = 0x24;
    null->x0 = -1;
    null->y0 = -1;
    null->x1 = -1;
    null->y1 = -1;
    null->x2 = -1;
    null->y2 = -1;
    null->tpage = 0;
    addPrim(current->ot + 0x1079, null);
    current->primptr += sizeof(POLY_FT3);

    poly = (POLY_FT4 *)current->primptr;
    ((P_TAG *)poly)->len = 9;
    poly->code = 0x2C;
    poly->r0 = 0x80;
    poly->g0 = 0x80;
    poly->b0 = 0x80;
    poly->x0 = pos->vx - 12;
    poly->y0 = pos->vy - 10;
    poly->x1 = pos->vx + 12;
    poly->y1 = pos->vy - 10;
    poly->x2 = pos->vx - 12;
    poly->y2 = pos->vy + 10;
    poly->x3 = pos->vx + 12;
    poly->y3 = pos->vy + 10;
    poly->u0 = D_000B9240.u0;
    poly->v0 = D_000B9240.v0;
    poly->u1 = D_000B9240.u1;
    poly->v1 = D_000B9240.v1;
    poly->u2 = D_000B9240.u2;
    poly->v2 = D_000B9240.v2 - 1;
    poly->u3 = D_000B9240.u3;
    poly->v3 = D_000B9240.v3 - 1;
    poly->clut = D_000B9240.clut;
    poly->tpage = D_000B9240.tpage | 0x20;
    addPrim(current->ot + 0x1079, poly);
    current->primptr += sizeof(POLY_FT4);

    poly = (POLY_FT4 *)current->primptr;
    ((P_TAG *)poly)->len = 9;
    poly->code = 0x2E;
    poly->r0 = intensity >> 1;
    poly->g0 = intensity >> 1;
    poly->b0 = intensity >> 2;

    size.vx = -35;
    size.vy = -35;
    idx = (angle >> 1) & 0xFFF;
    r.vx = ((D_0008EAAA[idx * 2] - D_0008EAAA[idx * 2 - 1]) * size.vx) >> 12;
    r.vy = ((D_0008EAAA[idx * 2] + D_0008EAAA[idx * 2 - 1]) * size.vy) >> 12;

    poly->x0 = pos->vx + ((r.vx * 3) >> 1);
    poly->y0 = pos->vy + r.vy;
    poly->x1 = pos->vx - ((r.vy * 3) >> 1);
    poly->y1 = pos->vy + r.vx;
    poly->x2 = pos->vx + ((r.vy * 3) >> 1);
    poly->y2 = pos->vy - r.vx;
    poly->x3 = pos->vx - ((r.vx * 3) >> 1);
    poly->y3 = pos->vy - r.vy;
    poly->u0 = D_000B9130.u0;
    poly->v0 = D_000B9130.v0;
    poly->u1 = D_000B9130.u1;
    poly->v1 = D_000B9130.v1;
    poly->u2 = D_000B9130.u2;
    poly->v2 = D_000B9130.v2 - 1;
    poly->u3 = D_000B9130.u3;
    poly->v3 = D_000B9130.v3 - 1;
    poly->clut = D_000B9130.clut;
    poly->tpage = D_000B9130.tpage | 0x20;
    addPrim(current->ot + 0x1078, poly);
    current->primptr += sizeof(POLY_FT4);
}
