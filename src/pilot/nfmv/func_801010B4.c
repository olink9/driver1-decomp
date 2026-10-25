#include "common.h"
extern int D_80126758;
extern void func_801109D8(), func_80101DD8();
extern void SsUtSetReverbType(short), SsUtSetReverbDepth(short, short), SsEnd(void);

void func_801010B4(void)
{
    if (D_80126758 < 50 || D_80126758 > 62)
        SetDispMask(0);
    VSyncCallback(0);
    func_801109D8();
    SsUtSetReverbType(0);
    SsUtSetReverbDepth(0, 0);
    SpuSetTransferStartAddr(0x66FFF);
    SpuWrite0(0x19000);
    VSync(100);
    SsEnd();
    StopCallback();
    func_80101DD8();
    if (D_80126758 < 50 || D_80126758 > 62)
        ResetGraph(3);
}
