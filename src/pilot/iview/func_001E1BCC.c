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

extern DB *current;
extern TEXTURE_DETAILS D_000B91D0;
extern int D_001E35B8;

void func_001E1BCC(void)
{
    int i;
    POLY_FT4 *poly;

    for (i = 0; i < D_001E35B8; i++) {
        poly = (POLY_FT4 *)current->primptr;
        poly->x0 = 0x18C + i * 0x1C;
        poly->y0 = 16;
        poly->x1 = 0x19A + i * 0x1C;
        poly->y1 = 16;
        poly->x2 = 0x18C + i * 0x1C;
        poly->y2 = 32;
        poly->x3 = 0x19A + i * 0x1C;
        poly->y3 = 32;
        poly->u0 = D_000B91D0.u0;
        poly->v0 = D_000B91D0.v0;
        poly->u1 = D_000B91D0.u1;
        poly->v1 = D_000B91D0.v1;
        poly->u2 = D_000B91D0.u2;
        poly->v2 = D_000B91D0.v2;
        poly->u3 = D_000B91D0.u3;
        poly->v3 = D_000B91D0.v3;
        ((P_TAG *)poly)->len = 9;
        poly->code = 0x2C;
        poly->r0 = 128;
        poly->g0 = 128;
        poly->b0 = 128;
        poly->tpage = D_000B91D0.tpageid | 0x20;
        poly->clut = D_000B91D0.clutid;
        ((P_TAG *)poly)->addr = ((P_TAG *)&current->ot[2])->addr;
        ((P_TAG *)&current->ot[2])->addr = (unsigned int)poly;
        current->primptr += sizeof(POLY_FT4);
    }
}
