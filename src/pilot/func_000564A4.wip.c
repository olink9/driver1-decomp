/* draw_box */
#include "common.h"
typedef struct { unsigned int tag; unsigned char r0,g0,b0,code; short x0,y0,x1,y1,x2,y2,x3,y3; int pad; } LF4;
typedef struct { unsigned int tag; unsigned char r0,g0,b0,code; short x0,y0,x1,y1; } LF2;
typedef struct { char *primptr; unsigned int *ot; } DBX;
extern long long D_00097530;
#define CUR (*(DBX **)&D_00097530)
extern void func_0007EAA4(), func_0007EA84();

void func_000564A4(void)
{
    LF4 *line4 = (LF4 *)CUR->primptr;
    LF2 *line2;
    DBX *c;
    unsigned int m = 0xffffff;
    func_0007EAA4(line4);
    line4->r0 = 0;
    line4->g0 = 0;
    line4->b0 = 128;
    line4->x0 = 399;
    line4->y0 = 159;
    line4->x1 = 496;
    line4->y1 = 159;
    line4->x2 = 496;
    line4->y2 = 220;
    line4->x3 = 399;
    line4->y3 = 220;
    c = CUR;
    *(unsigned int *)line4 = (*(unsigned int *)line4 & 0xff000000) | (c->ot[1] & m);
    c->ot[1] = (c->ot[1] & 0xff000000) | ((unsigned int)line4 & m);
    line2 = (LF2 *)(line4 + 1);
    func_0007EA84(line2);
    line2->r0 = 0;
    line2->g0 = 0;
    line2->b0 = 128;
    line2->x0 = 399;
    line2->y0 = 159;
    line2->x1 = 399;
    line2->y1 = 220;
    c = CUR;
    *(unsigned int *)line2 = (*(unsigned int *)line2 & 0xff000000) | (c->ot[1] & m);
    c->ot[1] = (c->ot[1] & 0xff000000) | ((unsigned int)line2 & m);
    CUR->primptr += 0x2C;
}
