#include "common.h"
extern long long D_801268A8;

int func_80101D40(void)
{
    int r = 2;
    if (*(unsigned short *)&D_801268A8)
        r = 3;
    return r;
}
