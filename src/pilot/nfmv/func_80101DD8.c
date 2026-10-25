#include "common.h"
extern short D_80126760;
extern short D_80126762;
extern void StopPAD(void);

void func_80101DD8(void)
{
    StopPAD();
    D_80126760 = 0;
    D_80126762 = 0;
}
