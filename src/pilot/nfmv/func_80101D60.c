#include "common.h"
extern int D_8012675C;

/* bump allocator: returns the current pointer, advances it by n rounded up to 4 */
int func_80101D60(int n)
{
    int p = D_8012675C;
    if (n & 3)
        n = (n + 4) & ~3;
    D_8012675C = p + n;
    return p;
}
