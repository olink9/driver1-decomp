extern long long D_00096C90;
extern void SsSetSerialVol();
extern void VSync();

void func_0006F4D8(void)
{
    int i = *(int *)&D_00096C90;
    short vol;
    while (i >= -9999) {
        i -= 1000;
        if (i < -10000) i = -10000;
        vol = 127 - (i / -78);
        SsSetSerialVol(0, vol, vol);
        VSync(2);
    }
}
