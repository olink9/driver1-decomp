/* Driver 2: ReInitSystem */
extern long long D_000970E4;
extern long long D_00096C94;
extern long long D_00096C90;
extern void func_000777E8(), func_000776F4(), func_00079800(), InitGeom(), func_000739B4();
extern void func_00077D6C(), func_00077E1C(), func_000357B4(), CdInit(), func_0004D3DC();
extern void func_0004D648(), func_0004E454(), func_0006EB48();

void func_0006D56C(void)
{
    func_000777E8();
    func_000776F4();
    func_00079800(0);
    InitGeom();
    func_000739B4(256, 120);
    *(int *)&D_000970E4 = 256;
    func_00077D6C(256);
    func_00077E1C(1);
    func_000357B4();
    CdInit();
    func_0004D3DC();
    func_0004D648(*(int *)&D_00096C94);
    func_0004E454(*(int *)&D_00096C90);
    func_0006EB48();
}
