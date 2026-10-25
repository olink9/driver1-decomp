#include "common.h"
extern short func_80101F54(short mask);

int func_80100DF8(void)
{
    int r = 0;
    if (func_80101F54(0x4000) || func_80101F54(1))
        r = 1;
    return r;
}
