#include "common.h"

void func_801011C4(void)
{
    RECT16 r[4];
    setRECT(&r[0], 0, 0, 512, 256);
    setRECT(&r[1], 512, 0, 512, 256);
    setRECT(&r[2], 0, 256, 512, 256);
    setRECT(&r[3], 512, 256, 512, 256);
    ClearImage(&r[0], 0, 0, 0);
    ClearImage(&r[1], 0, 0, 0);
    ClearImage(&r[2], 0, 0, 0);
    ClearImage(&r[3], 0, 0, 0);
    DrawSync(0);
}
