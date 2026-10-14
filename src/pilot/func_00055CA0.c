/* FAKEMATCH: second extern name (__asm__ alias) for the same symbol to defeat CSE */
/* Overmap target arrow (D2 overmap.c DrawTargetArrow-like): G4/G3 arrow + DR_TPAGE into ot[0] */
typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long D_00097530;
#define current (*(DB **)&D_00097530)
extern long long D_00096834;
#define GameLevel (*(int *)&D_00096834)
extern long long D_00096850;
#define gTimeOfDay (*(int *)&D_00096850)

typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct { unsigned addr:24; unsigned len:8; } TAG;
typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char r1, g1, b1, p1;
    short x1, y1;
    unsigned char r2, g2, b2, p2;
    short x2, y2;
} POLY_G3;
typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char r1, g1, b1, p1;
    short x1, y1;
    unsigned char r2, g2, b2, p2;
    short x2, y2;
    unsigned char r3, g3, b3, p3;
    short x3, y3;
} POLY_G4;
typedef struct { unsigned int tag; unsigned int code[1]; } DR_TPAGE;

typedef struct { int pad[5]; int scale; } OVERMAP;
typedef struct { char pad[0xC8]; int x; int y; int z; char pad2[0x18C - 0xD4]; } CAR;

extern OVERMAP D_0008B080[];
extern CAR D_0009B0A4[];
extern VECTOR D_0008909C;
/* second name for the same object: the target rebuilds &D_0008909C for the second call instead of CSE-ing it */
extern VECTOR camb __asm__("D_0008909C");
extern int D_00096D58;

extern int func_00067EE0();
extern void func_000539C8();

#define setlen(p, _len) (((TAG *)(p))->len = (unsigned char)(_len))
#define addPrim(ot, p) (((TAG *)(p))->addr = ((TAG *)(ot))->addr, ((TAG *)(ot))->addr = (unsigned int)(p))

void func_00055CA0(VECTOR *pos)
{
    VECTOR vertex[4];
    int dist;
    unsigned int len;
    int scale;
    int shade;
    void *pot;
    POLY_G4 *poly4;
    POLY_G3 *poly3;
    DR_TPAGE *tp;

    vertex[2].vx = pos->vx - D_0009B0A4[0].x;
    vertex[2].vz = pos->vz - D_0009B0A4[0].z;
    vertex[0].vx = (vertex[2].vz + 2) >> 2;
    vertex[0].vz = (2 - vertex[2].vx) >> 2;

    dist = func_00067EE0(&D_0008909C, &vertex[0]) / D_0008B080[GameLevel].scale;

    if (dist > D_00096D58) {
        vertex[0].vx = vertex[0].vx * D_00096D58 / dist;
        vertex[0].vz = vertex[0].vz * D_00096D58 / dist;
    }

    vertex[1].vx = -vertex[0].vx;
    vertex[1].vz = -vertex[0].vz;

    scale = D_0008B080[GameLevel].scale;
    pot = current->ot;

    len = func_00067EE0(&camb, &vertex[2]);

    shade = 180;
    if (gTimeOfDay != 0)
        shade = 75;

    if (len > scale * 46) {
        int l = len / scale;

        vertex[3].vx = vertex[1].vx + ((vertex[2].vx - vertex[1].vx) * 46) / l;
        vertex[3].vz = vertex[1].vz + ((vertex[2].vz - vertex[1].vz) * 46) / l;
        vertex[2].vx = vertex[0].vx + ((vertex[2].vx - vertex[0].vx) * 46) / l;
        vertex[2].vz = vertex[0].vz + ((vertex[2].vz - vertex[0].vz) * 46) / l;

        func_000539C8(vertex, vertex, 4, 1, 0);

        poly4 = (POLY_G4 *)current->primptr;
        setlen(poly4, 8);
        poly4->code = 0x38;
        poly4->x0 = vertex[0].vx;
        poly4->y0 = vertex[0].vz;
        poly4->x1 = vertex[2].vx;
        poly4->y1 = vertex[2].vz;
        poly4->x2 = vertex[1].vx;
        poly4->y2 = vertex[1].vz;
        poly4->x3 = vertex[3].vx;
        poly4->y3 = vertex[3].vz;
        poly4->r0 = 0; poly4->g0 = 0; poly4->b0 = 0;
        poly4->r1 = shade; poly4->g1 = shade; poly4->b1 = shade;
        poly4->r2 = 0; poly4->g2 = 0; poly4->b2 = 0;
        poly4->r3 = shade; poly4->g3 = shade; poly4->b3 = shade;
        poly4->code |= 2;
        addPrim(pot, poly4);
        current->primptr += sizeof(POLY_G4);
    } else {
        func_000539C8(vertex, vertex, 3, 1, 0);

        poly3 = (POLY_G3 *)current->primptr;
        setlen(poly3, 6);
        poly3->code = 0x30;
        poly3->x0 = vertex[0].vx;
        poly3->y0 = vertex[0].vz;
        poly3->x1 = vertex[2].vx;
        poly3->y1 = vertex[2].vz;
        poly3->x2 = vertex[1].vx;
        poly3->y2 = vertex[1].vz;
        poly3->r0 = 0; poly3->g0 = 0; poly3->b0 = 0;
        poly3->r1 = shade; poly3->g1 = shade; poly3->b1 = shade;
        poly3->r2 = 0; poly3->g2 = 0; poly3->b2 = 0;
        poly3->code |= 2;
        addPrim(pot, poly3);
        current->primptr += sizeof(POLY_G3);
    }

    tp = (DR_TPAGE *)current->primptr;
    setlen(tp, 1);
    tp->code[0] = 0xE1000640;
    addPrim(pot, tp);
    current->primptr += sizeof(DR_TPAGE);
}
