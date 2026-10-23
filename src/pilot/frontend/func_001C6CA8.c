extern int D_00096C94;
extern int D_00096C9C;
extern void SetMasterVolume();
extern void func_0004DB84();
extern void func_001C6990();

void func_001C6CA8(void)
{
    if (D_00096C9C != 0) {
        if (D_00096C94 >= -9999) {
            D_00096C94 -= 100;
            if (D_00096C94 < -10000)
                D_00096C94 = -10000;
            SetMasterVolume(D_00096C94);
            func_0004DB84(0, D_00096C94);
            func_001C6990();
        }
    }
}
