/* InitSound */
extern int D_0008AA24[];
extern int D_000BD484[];
extern int D_000BCA0C[];
extern char D_000BC9DC[];
extern int D_00096C94;
extern int SpuMalloc();
extern void SpuInit();
extern void SpuInitMalloc();
extern void SpuSetMute();
extern void AllocateReverb();
extern void func_0007C744();
extern void ResetSound();
extern void func_0006FDB8();
extern void SetMasterVolume();
extern void func_0004E8A8();
extern void VSyncCallback();

void InitSound(void)
{
    int i;

    SpuInit();
    SpuInitMalloc(5, D_000BC9DC);
    SpuSetMute(1);
    AllocateReverb(5, 0x4000);
    for (i = 0; i < 5; i++) {
        D_000BD484[i] = SpuMalloc(D_0008AA24[i]);
        if (D_000BD484[i] == -1)
            func_0007C744(-1);
    }
    for (i = 0; i < 16; i++)
        D_000BCA0C[i] = 1 << i;
    ResetSound();
    func_0006FDB8(0);
    SetMasterVolume(D_00096C94);
    VSyncCallback(func_0004E8A8);
    SpuSetMute(0);
}
