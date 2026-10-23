#include "common.h"

typedef struct { char *primptr; unsigned long *ot; } DB;
extern DB *current;

void func_001E1830(int a)
{
    POLY_G4 *p;
    DR_MODE *dr;
    int b;

    b = a - 190;
    if (a >= 256)
        a = 255;
    if (b < 0)
        b = 0;
    else if (b >= 256)
        b = 255;

    p = (POLY_G4 *)current->primptr;
    setPolyG4(p);
    setSemiTrans(p, 1);
    setRGB0(p, a, a, a);
    setRGB1(p, a, a, a);
    setRGB2(p, b, b, b);
    setRGB3(p, b, b, b);
    setXY4(p, 0, 0, 0x200, 0, 0, 0x100, 0x200, 0x100);
    addPrim(current->ot + 1, p);
    dr = (DR_MODE *)(p + 1);
    SetDrawMode(dr, 0, 0, 0x40, 0);
    addPrim(current->ot + 1, dr);
    current->primptr = (char *)(dr + 1);
}
