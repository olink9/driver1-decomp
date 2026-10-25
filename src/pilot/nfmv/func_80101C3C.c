#include "common.h"
extern void SsSetSerialVol(short, short, short);

int func_80101C3C(short *vol)
{
    *vol -= 8;
    if (*vol < 0)
        *vol = 0;
    SsSetSerialVol(0, *vol, *vol);
    return (*vol == 0) * 2;
}
