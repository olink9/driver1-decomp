/* draw_box */
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
typedef struct { unsigned int tag; unsigned char r0,g0,b0,code; short x0,y0,x1,y1,x2,y2,x3,y3; int pad; } LF4;
typedef struct { unsigned int tag; unsigned char r0,g0,b0,code; short x0,y0,x1,y1; } LF2;
typedef struct { char *primptr; unsigned int *ot; } DBX;
extern long long current;
#define current (*(DBX **)&current)
extern void SetLineF4(), SetLineF2();

#define addPrim(ot, p) ((P_TAG *)(p))->addr = ((P_TAG *)(ot))->addr, ((P_TAG *)(ot))->addr = (unsigned int)(p)

void draw_box(void)
{
    LF4 *line4;
    LF2 *line2;

    line4 = (LF4 *)current->primptr;
    SetLineF4(line4);
    line4->x0 = 399;
    line4->y0 = 159;
    line4->x1 = 496;
    line4->y1 = 159;
    line4->x2 = 496;
    line4->y2 = 220;
    line4->x3 = 399;
    line4->y3 = 220;
    line4->r0 = 0;
    line4->g0 = 0;
    line4->b0 = 128;
    addPrim(current->ot + 1, line4);
    line2 = (LF2 *)(line4 + 1);
    SetLineF2(line2);
    line2->x0 = 399;
    line2->y0 = 159;
    line2->x1 = 399;
    line2->y1 = 220;
    line2->r0 = 0;
    line2->g0 = 0;
    line2->b0 = 128;
    addPrim(current->ot + 1, line2);
    current->primptr += 0x2C;
}
