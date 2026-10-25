#include "common.h"
extern volatile int D_80126754;
extern void func_80101E00();

void func_80101160(void)
{
    D_80126754++;
    if (D_80126754 % 50 == 0)
        func_80101E00();
}
