#include "common.h"
extern void func_80101D88(), func_80100FB8(), func_80101160();

void func_80100E4C(void)
{
    ResetCallback();
    ResetGraph(0);
    SetGraphDebug(0);
    CdInit();
    InitGeom();
    func_80101D88();
    func_80100FB8();
    VSyncCallback(func_80101160);
}
