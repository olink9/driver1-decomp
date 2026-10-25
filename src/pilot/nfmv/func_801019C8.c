#include "common.h"

int func_801019C8(CdlLOC *loc)
{
    short retry = 0;
    short n = 0;

    for (;;) {
        while (CdControl(CdlSeekL, (u_char *)loc, 0) == 0) {
            if (++retry == 5)
                return 0;
        }
        retry = 0;
        if (++n == 6)
            return 0;
        if (CdRead2(CdlModeStream | CdlModeSpeed | CdlModeRT))
            return 1;
    }
}
