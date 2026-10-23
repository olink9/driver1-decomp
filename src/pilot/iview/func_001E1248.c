/* FAKEMATCH: unused DVECTOR locals pad/pad2 only place p0/p1 at sp+8/0x10 and size the 0x20 frame; `len = -20` variable keeps the mult (real-ish C). */
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
typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    short x1, y1;
} LINE_F2;
typedef struct { short vx, vy; } DVECTOR;

extern DB *current;
extern TEXTURE_DETAILS D_000B9160;
extern int D_001E4278;
extern int CameraCnt;
extern unsigned char D_001E33E4[];
extern short rcossin_tbl[];

void func_001E1248(void)
{
    POLY_FT4 *poly;
    LINE_F2 *line;
    DVECTOR pad, p0, p1, pad2;  /* pad, pad2: frame layout only */
    int s, c, a, len, col;

    poly = (POLY_FT4 *)current->primptr;
    poly->x0 = 12;
    poly->y0 = 14;
    poly->x1 = 108;
    poly->y1 = 14;
    poly->x2 = 12;
    poly->y2 = 96;
    poly->x3 = 108;
    poly->y3 = 96;
    poly->u0 = D_000B9160.u0;
    poly->v0 = D_000B9160.v0;
    poly->u1 = D_000B9160.u1;
    poly->v1 = D_000B9160.v1;
    poly->u2 = D_000B9160.u2;
    poly->v2 = D_000B9160.v2;
    poly->u3 = D_000B9160.u3;
    poly->v3 = D_000B9160.v3;
    ((P_TAG *)poly)->len = 9;
    poly->code = 0x2C;
    if (D_001E4278 <= 0xC00) {
        poly->r0 = 128;
        poly->g0 = 128;
        poly->b0 = 128;
    } else {
        col = D_001E33E4[CameraCnt & 0xF];
        poly->r0 = col + 128;
        poly->g0 = 128 - col;
        poly->b0 = 128 - col;
    }
    poly->tpage = D_000B9160.tpageid | 0x20;
    poly->clut = D_000B9160.clutid;
    ((P_TAG *)poly)->addr = ((P_TAG *)&current->ot[1])->addr;
    ((P_TAG *)&current->ot[1])->addr = (unsigned int)poly;
    current->primptr += sizeof(POLY_FT4);

    line = (LINE_F2 *)current->primptr;
    a = D_001E4278 & 0xFFF;
    len = -20;
    s = rcossin_tbl[a * 2] * len;
    c = rcossin_tbl[a * 2 + 1] * len;
    p0.vx = 60;
    p0.vy = 70;
    p1.vx = (-s * 3 >> 13) + 60;
    p1.vy = (c >> 12) + 70;
    line->x0 = p0.vx;
    line->y0 = p0.vy;
    line->x1 = p1.vx;
    line->y1 = p1.vy;
    line->r0 = 40;
    line->g0 = 40;
    line->b0 = 40;
    ((P_TAG *)line)->len = 3;
    line->code = 0x40;
    ((P_TAG *)line)->addr = ((P_TAG *)&current->ot[0])->addr;
    ((P_TAG *)&current->ot[0])->addr = (unsigned int)line;
    current->primptr += sizeof(LINE_F2);
}
