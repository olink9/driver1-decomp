typedef struct { char *primptr; unsigned int *ot; } DB;
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
typedef struct {
    unsigned char u0, v0, u1, v1, u2, v2, u3, v3;
    unsigned short tpageid;
    unsigned short clutid;
    char pad[4];
} TEXTURE_DETAILS;
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
} POLY_FT4;
typedef struct { int RecordingEnd; } REPLAY_PARAMETER_BLOCK;
typedef struct { int mask; short x, y; } BTN;

extern DB *current;
extern TEXTURE_DETAILS D_000B9270;
extern TEXTURE_DETAILS D_000B9130;
extern int D_000970F4;
extern signed char D_000970A0;
extern int CameraCnt;
extern REPLAY_PARAMETER_BLOCK *ReplayParameterPtr;
extern BTN D_001E33A4[];

void func_001E16D8(int x)
{
    POLY_FT4 *poly;
    int i, px, py;

    poly = (POLY_FT4 *)current->primptr;
    poly->x0 = x;
    poly->y0 = 10;
    poly->x1 = x + 128;
    poly->y1 = 10;
    poly->x2 = x;
    poly->y2 = 70;
    poly->x3 = x + 128;
    poly->y3 = 70;
    poly->u0 = D_000B9270.u0;
    poly->v0 = D_000B9270.v0;
    poly->u1 = D_000B9270.u1;
    poly->v1 = D_000B9270.v1;
    poly->u2 = D_000B9270.u2;
    poly->v2 = D_000B9270.v2;
    poly->u3 = D_000B9270.u3;
    poly->v3 = D_000B9270.v3;
    ((P_TAG *)poly)->len = 9;
    poly->code = 0x2C;
    poly->r0 = 100;
    poly->g0 = 100;
    poly->b0 = 100;
    poly->tpage = D_000B9270.tpageid | 0x20;
    poly->clut = D_000B9270.clutid;
    ((P_TAG *)poly)->addr = ((P_TAG *)&current->ot[3])->addr;
    ((P_TAG *)&current->ot[3])->addr = (unsigned int)poly;
    current->primptr += sizeof(POLY_FT4);

    D_000970F4 &= ~0xA000;
    if (D_000970A0 < -20)
        D_000970F4 |= 0x8000;
    if (D_000970A0 > 20)
        D_000970F4 |= 0x2000;
    if (CameraCnt >= ReplayParameterPtr->RecordingEnd)
        D_000970F4 = 0x10;

    for (i = 0; i < 8; i++) {
        if (D_000970F4 & D_001E33A4[i].mask) {
            POLY_FT4 *p;
            px = x + D_001E33A4[i].x;
            p = (POLY_FT4 *)current->primptr;
            py = D_001E33A4[i].y;
            p->x0 = px - 15;
            p->y0 = py + 1;
            p->x1 = px + 15;
            p->y1 = py + 1;
            p->x2 = px - 15;
            p->y2 = py + 19;
            p->x3 = px + 15;
            p->y3 = py + 19;
            p->u0 = D_000B9130.u0;
            p->v0 = D_000B9130.v0;
            p->u1 = D_000B9130.u1;
            p->v1 = D_000B9130.v1;
            p->u2 = D_000B9130.u2;
            p->v2 = D_000B9130.v2;
            p->u3 = D_000B9130.u3;
            p->v3 = D_000B9130.v3;
            ((P_TAG *)p)->len = 9;
            p->code = 0x2C;
            p->r0 = 80;
            p->g0 = 80;
            p->b0 = 80;
            p->code |= 2;
            p->tpage = D_000B9130.tpageid | 0x20;
            p->clut = D_000B9130.clutid;
            ((P_TAG *)p)->addr = ((P_TAG *)&current->ot[2])->addr;
            ((P_TAG *)&current->ot[2])->addr = (unsigned int)p;
            current->primptr += sizeof(POLY_FT4);
        }
    }
}
