/* Driver 2: DrawCompass (OVERMAP.C) */
typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct { int x, y; } XYPAIR;
typedef struct { int pad[5]; int scale; } OVERMAP;
typedef struct { char *primptr; unsigned int *ot; } DB;

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char r1, g1, b1, p1;
    short x1, y1;
} LINE_G2;

typedef struct { unsigned int tag; unsigned int code[1]; } DR_TPAGE;

typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
#define setaddr(_p, _addr) (((P_TAG *)(_p))->addr = (unsigned long)(_addr))
#define getaddr(_p) (unsigned long)(((P_TAG *)(_p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)

extern long long D_00097530;
#define current (*(DB **)&D_00097530)
extern long long D_00096834;
#define GameLevel (*(int *)&D_00096834)
extern XYPAIR D_0008B230[];
extern OVERMAP D_0008B080[];

extern void func_000539C8(VECTOR *in, VECTOR *out, int count, int a, int b);
extern void func_00053C88(VECTOR *pos, int flag);

void func_00053E54(void)
{
    VECTOR position[5];
    XYPAIR *pNorth;
    int scale;
    unsigned char loop;
    VECTOR *pPosition;
    unsigned int *pot;
    LINE_G2 *pLine;
    DR_TPAGE *tp;

    pNorth = &D_0008B230[GameLevel];
    scale = D_0008B080[GameLevel].scale;

    position[1].vx = pNorth->x * scale;
    position[1].vz = pNorth->y * scale;
    position[0].vx = position[1].vx * 20 + 8 >> 4;
    position[2].vx = position[1].vx + 2 >> 2;
    position[0].vz = position[1].vz * 20 + 8 >> 4;
    position[2].vz = position[1].vz + 2 >> 2;
    position[3].vx = (position[1].vz - position[1].vx) * 171 >> 10;
    position[3].vz = (-position[1].vz - position[1].vx) * 171 >> 10;
    position[4].vx = position[3].vz;
    position[4].vz = -position[3].vx;

    func_000539C8(position, position, 3, 1, 0);
    func_000539C8(position + 3, position + 3, 2, 1, 1);

    position[3].vx = position[3].vx + position[1].vx;
    position[3].vz = position[3].vz + position[1].vz;
    position[4].vx = position[4].vx + position[1].vx;
    position[4].vz = position[4].vz + position[1].vz;

    pot = current->ot;
    pPosition = position + 2;

    for (loop = 0; loop < 3; loop++) {
        pLine = (LINE_G2 *)current->primptr;

        ((P_TAG *)pLine)->len = 4;
        pLine->code = 0x50;

        pLine->x0 = position[1].vx;
        pLine->y0 = position[1].vz;
        pLine->x1 = pPosition->vx;
        pLine->y1 = pPosition->vz;

        pLine->r0 = 96;
        pLine->g0 = 96;
        pLine->b0 = 96;
        pLine->r1 = 0;
        pLine->g1 = 0;
        pLine->b1 = 0;
        pLine->code |= 2;

        addPrim(pot, pLine);
        current->primptr += sizeof(LINE_G2);
        pPosition++;
    }

    func_00053C88(position, 0);

    tp = (DR_TPAGE *)current->primptr;
    ((P_TAG *)tp)->len = 1;
    tp->code[0] = 0xE1000620;
    addPrim(pot, tp);
    current->primptr += sizeof(DR_TPAGE);
}
