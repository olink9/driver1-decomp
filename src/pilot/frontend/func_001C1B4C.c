/* FAKEMATCH: the unused `char buf[128]` only reproduces the 0x98 frame (0x80 gap); the original probably used it in code that was compiled out. */
extern int D_00097098;
extern int D_00096C94;
extern char D_001C0148[];
extern void func_0004D5F8();
extern int Loadfile();
extern void LoadSoundBank();
extern void SetMasterVolume();
extern void func_0004E08C();
extern void func_0006F2DC();
extern void func_0006F57C();

void func_001C1B4C(void)
{
    char buf[128];
    func_0004D5F8(0);
    D_00097098 = 0x114600;
    LoadSoundBank(D_00097098, Loadfile(D_001C0148, 0x114600), 0);
    SetMasterVolume(D_00096C94);
    func_0004E08C();
    func_0006F2DC(0, -1);
    func_0006F57C();
}
