#include "common.h"
extern unsigned char D_80126920[];
extern void ChangeClearPad(), func_80101E00();

void func_80101D88(void)
{
    InitPAD(D_80126920, 0x22, D_80126920 + 0x22, 0x22);
    StartPAD();
    ChangeClearPad(0);
    VSync(0);
    func_80101E00();
}
