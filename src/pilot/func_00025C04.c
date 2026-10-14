#include "common.h"

extern long long D_00097534;    /* an 8-byte object: absolute, not $gp */

void func_000513D4(void);
void func_000514E0(void);
void func_00025748(void);
void func_0005073C(int a, int b);

void func_00025C04(void)
{
    func_000513D4();
    func_000514E0();
    func_00025748();
    func_0005073C(0, 8);
    VSync(0);
    DrawSync(0);
    *(int *)&D_00097534 += 1;
}
