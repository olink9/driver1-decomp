/* Driver 2: ReInitFrontend */
extern int D_000967C8;
extern int D_000967D4;
extern char D_000967BD;
extern int D_000BDFA4[];
extern char D_00010280[];
extern char D_0001028C[];
extern void func_00051374();
extern void DrawSync();
extern void VSync();
extern void SetDispMask();
extern void func_000359BC();
extern void func_0004E348();
extern void Loadfile();
extern void func_1C2DAC();
extern void func_00074374();
extern void func_1C2F78();
extern void func_1C1B4C();
extern void func_0007457C();

void ReInitFrontend(void)
{
    short rect[4];
    int *p;

    func_00051374();
    DrawSync(0);
    VSync(0);
    D_000967C8 = 1;
    D_000967D4 = 0;
    SetDispMask(0);
    func_000359BC();
    p = D_000BDFA4;
    func_0004E348();
    Loadfile(D_0001028C, 0x1C0000);
    Loadfile(D_00010280, 0xE6600);
    func_1C2DAC(0x280);
    DrawSync(0);
    func_00051374();
    rect[0] = 0;
    rect[1] = 0;
    rect[2] = 320;
    rect[3] = 512;
    func_00074374(rect, 0, 0, 0);
    DrawSync(0);
    func_1C2F78();
    func_1C1B4C();
    DrawSync(0);
    VSync(0);
    func_0007457C(p[1], 0x1080);
    func_0007457C(p[0x21], 0x1080);
    SetDispMask(1);
    D_000967BD = 0xFF;
}
