/* func_0002F444: transform 12 world points relative to the camera into a ring of
 * SVECTORs and draw flat quads/triangles (POLY_F4) between them. */
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { short m[3][3]; int t[3]; } MATRIX;
typedef struct { int vx, vy, vz, pad; } VECTOR;

typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long D_00097530;
#define current (*(DB **)&D_00097530)

typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    short x1, y1;
    short x2, y2;
    short x3, y3;
} PF4;

extern VECTOR D_00089384[];
extern VECTOR D_0008A8F0;
extern MATRIX D_000BBE84;
extern short D_000BBE90[];
extern MATRIX D_000890BC;
extern VECTOR D_0008909C;
extern long long D_00096850;
extern int D_000969A8;
extern int D_000969AC;
extern int D_000969B0;
extern void func_0002C564();
extern void func_00079D20();

#define gte_SetTransVector(r0) __asm__ volatile ( \
    "lw $12, 0(%0);" "lw $13, 4(%0);" "lw $14, 8(%0);" \
    "ctc2 $12, $5;" "ctc2 $13, $6;" "ctc2 $14, $7" : : "r"(r0) : "$12", "$13", "$14")
#define gte_ldv0(r0) __asm__ volatile ( \
    "lwc2 $0, 0(%0);" "lwc2 $1, 4(%0)" : : "r"(r0))
#define gte_ldv3(r0, r1, r2) __asm__ volatile ( \
    "lwc2 $0, 0(%0);" "lwc2 $1, 4(%0);" "lwc2 $2, 0(%1);" "lwc2 $3, 4(%1);" \
    "lwc2 $4, 0(%2);" "lwc2 $5, 4(%2)" : : "r"(r0), "r"(r1), "r"(r2))
#define gte_rtpt() __asm__ volatile ("nop;" "nop;" "rtpt")
#define gte_rtps() __asm__ volatile ("nop;" "nop;" "rtps")
#define gte_stsxy3_f4(r0) __asm__ volatile ( \
    "swc2 $12, 8(%0);" "swc2 $13, 12(%0);" "swc2 $14, 16(%0)" : : "r"(r0) : "memory")
#define gte_stsxy(r0) __asm__ volatile ("swc2 $14, 0(%0)" : : "r"(r0) : "memory")

static inline void DrawQuad(SVECTOR *a, SVECTOR *b, SVECTOR *c, SVECTOR *d, int col)
{
    PF4 *p;

    if (a->vz > 700 && b->vz > 700 && c->vz > 700 && d->vz > 700) {
        p = (PF4 *)current->primptr;
        gte_ldv3(a, b, d);
        gte_rtpt();
        *(int *)&p->r0 = col;
        ((unsigned char *)p)[3] = 5;
        p->code = 0x28;
        gte_stsxy3_f4(p);
        gte_ldv0(c);
        gte_rtps();
        ((P_TAG *)p)->addr = ((P_TAG *)&current->ot[0x107F])->addr;
        ((P_TAG *)&current->ot[0x107F])->addr = (unsigned int)p;
        gte_stsxy(&p->x3);
        current->primptr = (char *)(p + 1);
    }
}

void func_0002F444(void)
{
    VECTOR v;
    SVECTOR sv[12];
    int i;

    for (i = 0; i < 12; i++) {
        v.vx = D_00089384[i].vx - D_0008A8F0.vx;
        v.vy = D_00089384[i].vy - D_0008A8F0.vy;
        v.vz = D_00089384[i].vz - D_0008A8F0.vz;
        if (D_000BBE84.m[2][0] > 0) {
            if (v.vx > 0x3FF70)
                v.vx = 0x3FF70;
            if (v.vx < 20000)
                v.vx = 20000;
        } else {
            if (v.vx > -20000)
                v.vx = -20000;
            if (v.vx < -0x3FF70)
                v.vx = -0x3FF70;
        }
        v.vx >>= 4;
        v.vy >>= 4;
        v.vz >>= 4;
        func_0002C564(&v);
        sv[i].vx = v.vx;
        sv[i].vy = v.vy;
        sv[i].vz = v.vz;
    }
    func_00079D20(&D_000890BC);
    gte_SetTransVector(&D_0008909C);

    {
        int col = D_000969A8;
        if (*(int *)&D_00096850)
            col >>= 2;
        DrawQuad(&sv[0], &sv[1], &sv[5], &sv[4], col);
    }
    {
        int col = D_000969AC;
        if (*(int *)&D_00096850)
            col >>= 2;
        DrawQuad(&sv[1], &sv[2], &sv[6], &sv[5], col);
    }
    {
        int col = D_000969B0;
        if (*(int *)&D_00096850)
            col >>= 2;
        DrawQuad(&sv[2], &sv[3], &sv[7], &sv[6], col);
    }
    {
        int col = D_000969AC;
        if (*(int *)&D_00096850)
            col >>= 2;
        DrawQuad(&sv[3], &sv[0], &sv[4], &sv[7], col);
    }
    if (D_000BBE90[0] > 0) {
        {
            int col = D_000969B0;
            if (*(int *)&D_00096850)
                col >>= 2;
            DrawQuad(&sv[6], &sv[7], &sv[11], &sv[10], col);
        }
        {
            int col = D_000969B0;
            if (*(int *)&D_00096850)
                col >>= 2;
            DrawQuad(&sv[5], &sv[6], &sv[10], &sv[10], col);
        }
        {
            int col = D_000969B0;
            if (*(int *)&D_00096850)
                col >>= 2;
            DrawQuad(&sv[4], &sv[7], &sv[11], &sv[11], col);
        }
    } else {
        {
            int col = D_000969B0;
            if (*(int *)&D_00096850)
                col >>= 2;
            DrawQuad(&sv[2], &sv[3], &sv[9], &sv[8], col);
        }
        {
            int col = D_000969B0;
            if (*(int *)&D_00096850)
                col >>= 2;
            DrawQuad(&sv[1], &sv[2], &sv[8], &sv[8], col);
        }
        {
            int col = D_000969B0;
            if (*(int *)&D_00096850)
                col >>= 2;
            DrawQuad(&sv[0], &sv[3], &sv[9], &sv[9], col);
        }
    }
}
