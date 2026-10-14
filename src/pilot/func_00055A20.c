/* Driver 2: DrawSightCone (overmap.c) - D1 version: no tile, ot[0], fixed TransparencyOn */
typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long D_00097530;
#define current (*(DB **)&D_00097530)

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
typedef struct { unsigned int tag; unsigned int code[1]; } DR_TPAGE;

typedef struct {
    short surroundViewDistance;
    short frontViewDistance;
    short frontViewAngle;
} COP_SIGHT_DATA;

extern short D_0008EAA8[];
extern void func_000539C8();

#define setlen(p, _len) (((TAG *)(p))->len = (unsigned char)(_len))
#define addPrim(ot, p) (((TAG *)(p))->addr = ((TAG *)(ot))->addr, ((TAG *)(ot))->addr = (unsigned int)(p))

void func_00055A20(COP_SIGHT_DATA *pCopSightData, VECTOR *pPosition, int direction)
{
    VECTOR vertex[9];
    VECTOR *pVertex;
    int angle;
    int frontViewAngle;
    int frontViewDistance;
    int surroundViewDistance;
    int temp;
    int dir;
    void *pot;
    VECTOR *pNextVertex;
    POLY_G3 *poly;
    DR_TPAGE *tp;

    frontViewAngle = pCopSightData->frontViewAngle;
    frontViewDistance = pCopSightData->frontViewDistance;
    surroundViewDistance = pCopSightData->surroundViewDistance;

    vertex[0].vx = pPosition->vx;
    vertex[0].vz = pPosition->vz;

    angle = 0;
    pVertex = vertex + 1;
    do {
        if (angle <= frontViewAngle || angle >= 4096 - frontViewAngle)
            temp = frontViewDistance;
        else
            temp = surroundViewDistance;

        dir = angle + direction & 0xfff;
        angle += 512;
        pVertex->vx = vertex[0].vx + ((D_0008EAA8[dir * 2] * temp + 0x800) >> 12);
        pVertex->vz = vertex[0].vz + ((D_0008EAA8[dir * 2 + 1] * temp + 0x800) >> 12);
        pVertex++;
    } while (angle < 4096);

    func_000539C8(vertex, vertex, 9, 0, 0);

    {
    VECTOR *pVertex;
    pot = current->ot;
    pVertex = vertex + 1;
    while (pVertex < &vertex[9]) {
        poly = (POLY_G3 *)current->primptr;
        pNextVertex = pVertex + 1;
        if (pNextVertex > vertex + 8)
            pNextVertex = vertex + 1;

        setlen(poly, 6);
        poly->code = 0x30;
        poly->x0 = vertex[0].vx;
        poly->y0 = vertex[0].vz;
        poly->x1 = pVertex->vx;
        poly->y1 = pVertex->vz;
        poly->x2 = pNextVertex->vx;
        poly->y2 = pNextVertex->vz;
        poly->r0 = 96; poly->g0 = 96; poly->b0 = 96;
        poly->r1 = 0; poly->g1 = 0; poly->b1 = 0;
        poly->r2 = 0; poly->g2 = 0; poly->b2 = 0;
        poly->code |= 2;
        addPrim(pot, poly);
        current->primptr += sizeof(POLY_G3);
        pVertex++;
    }
    }

    tp = (DR_TPAGE *)current->primptr;
    setlen(tp, 1);
    tp->code[0] = 0xE1000620;
    addPrim(pot, tp);
    current->primptr += sizeof(DR_TPAGE);
}
