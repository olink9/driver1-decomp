/* Draws the clipped polygon built by func_00028AF8 (3..6 points) as POLY_FT3/POLY_FT4 */
typedef struct { char *primptr; unsigned int *ot; int pad; int id; } DB;
typedef struct { int xy; unsigned short uv; short pad; } CLIPPT;
typedef struct { unsigned int tag, rgbc; int xy0, uv0, xy1, uv1, xy2, uv2; } FT3;
typedef struct { unsigned int tag, rgbc; int xy0, uv0, xy1, uv1, xy2, uv2, xy3, uv3; } FT4;

typedef struct { unsigned int *v; } P4;
extern P4 D_00097154;
#define OT D_00097154.v
typedef struct { int v; } I4;
extern I4 D_00097130;
extern I4 D_00097134;
#define clut D_00097130.v
#define tpage D_00097134.v
extern int D_00097158;
extern unsigned int *D_00096950;
extern CLIPPT D_000A1BC8[];
extern long long D_00097530;
#define current (*(DB **)&D_00097530)
#define pts D_000A1BC8

extern void func_00028AF8(int a, int b);

void func_00028FA4(unsigned int *ot)
{
    OT = ot;
    D_00097158 = 0;
    func_00028AF8(0, 1);
    func_00028AF8(1, 2);
    func_00028AF8(2, 3);
    func_00028AF8(3, 0);

    if (D_00097158 == 3) {
        FT3 *p = (FT3 *)current->primptr;
        unsigned int *o = OT;
        p->tag = *o | 0x7000000;
        *o = (unsigned int)p;
        p->rgbc = (*D_00096950 & 0xFFFFFF) | 0x24000000;
        p->xy0 = pts[0].xy;
        p->uv0 = pts[0].uv | clut;
        p->xy1 = pts[1].xy;
        p->uv1 = pts[1].uv | tpage;
        p->xy2 = pts[2].xy;
        p->uv2 = pts[2].uv;
        current->primptr = (char *)(p + 1);
    } else if (D_00097158 == 4) {
        FT4 *p = (FT4 *)current->primptr;
        unsigned int *o = OT;
        p->tag = *o | 0x9000000;
        *o = (unsigned int)p;
        p->rgbc = *D_00096950;
        p->xy0 = pts[0].xy;
        p->uv0 = pts[0].uv | clut;
        p->xy1 = pts[1].xy;
        p->uv1 = pts[1].uv | tpage;
        p->xy2 = pts[3].xy;
        p->uv2 = pts[3].uv;
        p->xy3 = pts[2].xy;
        p->uv3 = pts[2].uv;
        current->primptr = (char *)(p + 1);
    } else if (D_00097158 == 5) {
        FT3 *p = (FT3 *)current->primptr;
        FT4 *q;
        unsigned int *o2;
        unsigned int *o = OT;
        p->tag = *o | 0x7000000;
        *o = (unsigned int)p;
        p->rgbc = (*D_00096950 & 0xFFFFFF) | 0x24000000;
        p->xy0 = pts[0].xy;
        p->uv0 = pts[0].uv | clut;
        p->xy1 = pts[1].xy;
        p->uv1 = pts[1].uv | tpage;
        p->xy2 = pts[4].xy;
        p->uv2 = pts[4].uv;
        {
        DB *c = current;
        o2 = OT;
        c->primptr = (char *)(p + 1);
        }
        q = (FT4 *)(p + 1);
        q->tag = *o2 | 0x9000000;
        *o2 = (unsigned int)q;
        q->rgbc = *D_00096950;
        q->xy0 = pts[1].xy;
        q->uv0 = pts[1].uv | clut;
        q->xy1 = pts[2].xy;
        q->uv1 = pts[2].uv | tpage;
        q->xy2 = pts[4].xy;
        q->uv2 = pts[4].uv;
        q->xy3 = pts[3].xy;
        q->uv3 = pts[3].uv;
        current->primptr = (char *)(q + 1);
    } else if (D_00097158 == 6) {
        FT4 *p = (FT4 *)current->primptr;
        FT4 *q;
        unsigned int *o2;
        unsigned int *o = OT;
        p->tag = *o | 0x9000000;
        *o = (unsigned int)p;
        p->rgbc = *D_00096950;
        p->xy0 = pts[0].xy;
        p->uv0 = pts[0].uv | clut;
        p->xy1 = pts[1].xy;
        p->uv1 = pts[1].uv | tpage;
        p->xy2 = pts[5].xy;
        p->uv2 = pts[5].uv;
        p->xy3 = pts[2].xy;
        p->uv3 = pts[2].uv;
        {
        DB *c = current;
        o2 = OT;
        c->primptr = (char *)(p + 1);
        }
        q = (FT4 *)(p + 1);
        q->tag = *o2 | 0x9000000;
        *o2 = (unsigned int)q;
        q->rgbc = *D_00096950;
        q->xy0 = pts[5].xy;
        q->uv0 = pts[5].uv | clut;
        q->xy1 = pts[2].xy;
        q->uv1 = pts[2].uv | tpage;
        q->xy2 = pts[4].xy;
        q->uv2 = pts[4].uv;
        q->xy3 = pts[3].xy;
        q->uv3 = pts[3].uv;
        current->primptr = (char *)(q + 1);
    }
}
