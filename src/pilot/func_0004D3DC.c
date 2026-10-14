/* InitSound */
extern int D_0008AA24[];
extern int D_000BD484[];
extern int D_000BCA0C[];
extern char D_000BC9DC[];
extern int D_00096C94;
extern int SpuMalloc();
extern void func_00079AC0();
extern void SpuInitMalloc();
extern void SpuSetMute();
extern void func_0004E840();
extern void func_0007C744();
extern void func_0004D544();
extern void func_0006FDB8();
extern void func_0004D648();
extern void func_0004E8A8();
extern void func_00077784();

void func_0004D3DC(void)
{
    int i;

    func_00079AC0();
    SpuInitMalloc(5, D_000BC9DC);
    SpuSetMute(1);
    func_0004E840(5, 0x4000);
    for (i = 0; i < 5; i++) {
        D_000BD484[i] = SpuMalloc(D_0008AA24[i]);
        if (D_000BD484[i] == -1)
            func_0007C744(-1);
    }
    for (i = 0; i < 16; i++)
        D_000BCA0C[i] = 1 << i;
    func_0004D544();
    func_0006FDB8(0);
    func_0004D648(D_00096C94);
    func_00077784(func_0004E8A8);
    SpuSetMute(0);
}
