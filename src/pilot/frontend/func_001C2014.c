/* func_001C2014: draw a flat POLY_F4 box (setPolyF4, setXYWH order, setRGB0, addPrim).
   FAKEMATCH: char pad[32] only sizes the 0x20 frame (no saves; probably an unused local aggregate). */
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
typedef struct { unsigned int tag; unsigned char r0, g0, b0, code; short x0, y0, x1, y1, x2, y2, x3, y3; } PF4;
typedef struct { unsigned int *p; unsigned int **ot; } DB;
extern int D_001CDE88, D_001CDE8C;
extern DB *current;

void func_001C2014(int x, int y, unsigned char *c, int w, int h, int z)
{
    char pad[32];
    PF4 *p;
    x += D_001CDE88;
    y += D_001CDE8C;
    p = (PF4 *)current->p;
    ((unsigned char *)p)[3] = 5;
    ((unsigned char *)p)[7] = 0x29;
    p->x0 = x;
    p->y0 = y;
    p->x1 = x + w;
    p->y1 = y;
    p->x2 = x;
    p->y2 = y + h;
    p->x3 = x + w;
    p->y3 = y + h;
    p->r0 = c[0];
    p->g0 = c[1];
    p->b0 = c[2];
    ((P_TAG *)p)->addr = ((P_TAG *)current->ot[z])->addr;
    ((P_TAG *)current->ot[z])->addr = (unsigned)p;
    current->p += 6;
}
