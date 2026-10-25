#include "common.h"
extern void func_8010C8F8(), func_801109D8(), func_801109F8();
extern void SsInit(void), SsUtSetReverbType(short), SsUtSetReverbDepth(short, short), SsSetTickMode(long);
extern void SsSetMVol(short, short), SsSetSerialAttr(char, char, char), SsSetSerialVol(short, short, short);

void func_80100FB8(void)
{
    CdlATV atv;
    u_char result[8];

    atv.val0 = 127;
    atv.val1 = 127;
    atv.val2 = 127;
    atv.val3 = 127;
    CdControl(CdlDemute, 0, result);
    CdControlB(CdlSetfilter, 0, result);
    CdMix(&atv);
    func_8010C8F8();
    SpuSetTransferStartAddr(0x66FFF);
    SpuWrite0(0x19000);
    SsInit();
    func_801109D8();
    SsUtSetReverbType(0);
    SsUtSetReverbDepth(0, 0);
    SsSetTickMode(5);
    SsSetMVol(127, 127);
    SsSetSerialAttr(0, 0, 1);
    SsSetSerialVol(0, 127, 127);
    SsUtSetReverbType(3);
    func_801109F8();
    VSync(75);
    SsUtSetReverbDepth(40, 40);
}
