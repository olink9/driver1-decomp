/* Driver 2: main */
extern long long D_000943E8;
extern long long D_000943EC;
extern int D_000967C0;
extern int D_000967F8;
extern int D_000970E4;
extern char D_00010258[];
extern char D_00010268[];
extern char D_00010274[];
extern char D_00010280[];
extern char D_0001028C[];
extern void __main();
extern void SetDispMask();
extern void func_000777E8();
extern void func_000776F4();
extern void func_00073F88();
extern void SsSetSerialVol();
extern void func_000740FC();
extern void func_00079800();
extern void CdInit();
extern void SpuInit();
extern void ShowLoadingScreen();
extern void VSync();
extern void InitGeom();
extern void func_000739B4();
extern void func_00077D6C();
extern void func_00077E1C();
extern void func_000357B4();
extern void func_000503F4();
extern void InitSound();
extern void Loadfile();
extern void func_EC89C();
extern int func_00079AE0();
extern void PlayRender();
extern void func_00079AF0();
extern void ReInitSystem();
extern void func_1C0AE8();

/* gcc inserts the __main() call itself because the function is named main */
void main(void)
{
    int i;
    int ev;

    i = 0;
    *(int *)&D_000943E8 = 0x4000;
    *(int *)&D_000943EC = 0x200000;
    SetDispMask(i);
    D_000967C0 = 1;
    func_000777E8();
    func_000776F4();
    func_00073F88(i);
    SsSetSerialVol(i, i, i);
    SetDispMask(i);
    func_000740FC(i);
    func_00079800(i);
    CdInit();
    SsSetSerialVol(i, i, i);
    SpuInit();
    ShowLoadingScreen(D_00010258, 1, 1);
    for (i = 0; i < 250;) {
        i++;
        VSync(0);
    }
    InitGeom();
    i = 0;
    func_000739B4(256, 120);
    D_000970E4 = 256;
    func_00077D6C(256);
    func_00077E1C(1);
    func_000357B4();
    func_000503F4();
    InitSound();
    ShowLoadingScreen(D_00010258, 2, 1);
    SetDispMask(0);
    ShowLoadingScreen(D_00010268, 1, 1);
    for (i = 0; i < 250;) {
        i++;
        VSync(0);
    }
    ShowLoadingScreen(D_00010268, 2, 1);
    SetDispMask(0);
    i = 0;
    ShowLoadingScreen(D_00010274, 1, 1);
    for (i = 0; i < 250;) {
        i++;
        VSync(0);
    }
    ShowLoadingScreen(D_00010274, 2, 1);
    SetDispMask(0);
    Loadfile(D_00010280, 0xE6600);
    func_EC89C();
    ev = func_00079AE0();
    D_000967F8 = 0;
    PlayRender(0);
    func_00079AF0(ev);
    ReInitSystem();
    Loadfile(D_0001028C, 0x1C0000);
    func_1C0AE8();
}
