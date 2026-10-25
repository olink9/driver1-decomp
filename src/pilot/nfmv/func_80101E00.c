#include "common.h"
extern volatile short D_80126760;
extern volatile short D_80126762;
extern volatile unsigned char D_80126920[34];
extern volatile unsigned char D_80126942[34];

void func_80101E00(void)
{
    D_80126760 = 0;
    if (D_80126920[0] != 0xFF && (D_80126920[1] >> 4) == 4)
        D_80126760 |= 1;
    if (D_80126920[0] != 0xFF && (D_80126920[1] >> 4) == 7)
        D_80126760 |= 1;
    if (D_80126942[0] != 0xFF && (D_80126942[1] >> 4) == 4)
        D_80126760 |= 2;
    if (D_80126942[0] != 0xFF && (D_80126942[1] >> 4) == 7)
        D_80126760 |= 1;
    if (D_80126760 == 3 || D_80126760 == 1)
        D_80126762 = 0;
    else if (D_80126760 == 2)
        D_80126762 = 1;
}
