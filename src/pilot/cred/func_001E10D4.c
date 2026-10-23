/* Credits: scroll the picture split D_001F3848 by D_001F3860 (clamped 0xF0..0x200), set the scale matrix
   and screen centre from it, and draw two black tiles */
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
typedef struct { char *primptr; unsigned long *ot; } DB;
typedef struct {
    unsigned long tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    short w, h;
} TILE;

extern DB *current;
extern int D_001F3848, D_001F3860;
extern short D_0008AA38[];
extern void func_000739B4();

void func_001E10D4(void)
{
    TILE *t;

    D_001F3848 += D_001F3860;
    if (D_001F3848 < 0xF0)
        D_001F3848 = 0xF0;
    else if (D_001F3848 > 0x200)
        D_001F3848 = 0x200;
    D_0008AA38[0] = (D_001F3848 << 12) / 320;
    func_000739B4(((0x200 - D_001F3848) >> 3) + D_001F3848 / 2, 0x78);

    t = (TILE *)current->primptr;
    ((P_TAG *)t)->len = 3;
    t->code = 0x60;
    t->r0 = 0;
    t->g0 = 0;
    t->b0 = 0;
    t->x0 = D_001F3848;
    t->y0 = 0;
    t->w = 0x200 - D_001F3848;
    t->h = 0xF0;
    ((P_TAG *)t)->addr = ((P_TAG *)&current->ot[6])->addr;
    ((P_TAG *)&current->ot[6])->addr = (unsigned long)t;
    t++;
    ((P_TAG *)t)->len = 3;
    t->code = 0x60;
    t->r0 = 0;
    t->g0 = 0;
    t->b0 = 0;
    t->x0 = 0;
    t->y0 = 0;
    t->w = (0x200 - D_001F3848) >> 3;
    t->h = 0xF0;
    ((P_TAG *)t)->addr = ((P_TAG *)&current->ot[6])->addr;
    ((P_TAG *)&current->ot[6])->addr = (unsigned long)t;
    current->primptr += 2 * sizeof(TILE);
}
