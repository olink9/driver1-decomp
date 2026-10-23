/* Credits: draw the face picture sprite, a black tile right of it, and its draw-mode prim */
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
typedef struct { char *primptr; unsigned long *ot; } DB;
typedef struct {
    unsigned long tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short w, h;
} SPRT;
typedef struct {
    unsigned long tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    short w, h;
} TILE;
typedef struct { unsigned long tag; unsigned long code[2]; } DR_MODE;
typedef struct {
    unsigned char u0, v0;
    unsigned char pad[4];
    unsigned char u1, v1;
    unsigned short tpage;
    unsigned short clut;
} IMGINFO;

extern DB *current;
extern int D_001F3848;
extern IMGINFO D_001F3850;
extern void SetDrawMode();

#define addPrimOT(n, p) \
    (((P_TAG *)(p))->addr = ((P_TAG *)&current->ot[n])->addr, \
     ((P_TAG *)&current->ot[n])->addr = (unsigned long)(p))

void func_001E0D40(void)
{
    SPRT *spr;
    TILE *tile;
    DR_MODE *mode;
    int x;

    x = D_001F3848;
    spr = (SPRT *)current->primptr;
    ((P_TAG *)spr)->len = 4;
    spr->code = 0x64;
    spr->r0 = 0x80;
    spr->g0 = 0x80;
    spr->b0 = 0x80;
    spr->x0 = x;
    spr->y0 = 0;
    spr->u0 = D_001F3850.u0;
    spr->v0 = D_001F3850.v0;
    spr->w = D_001F3850.u1 - D_001F3850.u0;
    spr->h = D_001F3850.v1 - D_001F3850.v0;
    spr->clut = D_001F3850.clut;
    addPrimOT(1, spr);

    tile = (TILE *)(spr + 1);
    ((P_TAG *)tile)->len = 3;
    tile->code = 0x60;
    tile->r0 = 0;
    tile->g0 = 0;
    tile->b0 = 0;
    tile->x0 = x;
    tile->y0 = 0;
    tile->w = 0x200 - D_001F3848;
    tile->h = 0x40;
    addPrimOT(1, tile);

    mode = (DR_MODE *)(tile + 1);
    SetDrawMode(mode, 0, 0, D_001F3850.tpage, 0);
    addPrimOT(1, mode);

    current->primptr = (char *)(mode + 1);
}
