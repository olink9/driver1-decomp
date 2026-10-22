/* FAKEMATCH (if it ever matches): `c = *p0; ... idx = *p0;` reads cos twice into two variables only to shift
 * local-alloc priorities. No do{}while(0) wrapper any more. */
/* Driver 2: DisplaySun (SKY.C) - null FT3 + sun FT4 + rotating flare FT4. D2 debug locals: polys, null,
 * VECTOR output (stack), int width, height. cos = rcossin_tbl[(i*2)+1] is referenced FIRST (that is why the
 * target addresses D_0008EAAA and derives the sin address as base-2 by CSE).
 * r4-14b: 99.37 WITHOUT the wrapper (was 99.22 with it). Permuter found the reuse of `idx` as the cos value;
 * that alone puts the masks/9/base3/poly3 (t1/t3/t4/t2/t0) right - everything outside the rotation block now
 * matches. A clean no-steering form scores 99.22 (also wrapper-free):
 *     idx = *p0; sn = *p1; r.vx = ((idx - sn) * size.vx) >> 12; r.vy = ((*p1 + *p0) * size.vy) >> 12;
 * LEFT (rotation block only): target has -35 in a1, &cos in a3, sin value + r.vx in a2. With local-alloc
 * priority floor_log2(refs)*refs/life, the -35 qty (3 refs over >= 7 insns, 0.43) can never be allocated before
 * r.vx's sra (4 refs / 9 insns, 0.89) or poly3 (0.92), yet a1 must be busy for both -> the target's -35 has
 * >= 4 refs, or r.vx/poly3 are global (set twice). Tried without success: D2 literal form (camera_angle struct
 * .vy, (vy & 0x1ffe)+1 index -> andi-before-sll, wrong), S8/CA struct forms of D_00096C4A, mask addPrim,
 * 8-byte struct `current`, size.vx operand order, size chain assignment, static inline rot(&r,&size,ang),
 * one variable spanning sin value + r.vx (D2 `width` reuse), null reused as the third prim pointer. */

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

extern long long current;
#define current (*(DB **)&current)
extern SPRTEX D_000B9240;
extern SPRTEX D_000B9130;
extern short D_0008EAAA[];
extern long long D_00096C4A;
#define angle (*(unsigned short *)&D_00096C4A)

typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
#define setaddr(_p, _addr) (((P_TAG *)(_p))->addr = (unsigned long)(_addr))
#define getaddr(_p) (unsigned long)(((P_TAG *)(_p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)


void DisplaySun(DVECTOR *pos, int a1, int a2, int intensity)
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
    { short *p0 = &D_0008EAAA[idx * 2]; short *p1 = &D_0008EAAA[idx * 2 - 1]; int c, sn;
    c = *p0; sn = *p1; idx = *p0;
    r.vx = ((c - sn) * size.vx) >> 12;
    r.vy = ((idx + *p1) * size.vy) >> 12; }

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
