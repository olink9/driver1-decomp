#include "common.h"
extern long long D_801268A8;

unsigned short func_80101D0C(unsigned short x)
{
    if (*(unsigned short *)&D_801268A8)
        return (x * 3) >> 1;
    return x;
}
