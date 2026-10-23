/* FAKEMATCH: local `nm` (= D_001C0A40) only steers register allocation */
extern int current, D_00097524, D_00096834;
extern volatile int D_00097534; 
extern char cameraview;
extern char D_001C0A40[], D_001C0A50[];
extern void SetDispMask();
extern void func_0007457C();
extern void func_00073F88();
extern void func_00079800();
extern void ShowLoadingScreen();
extern int Loadfile();
extern void func_00022818();
extern void func_001CBA18();
extern void func_000513B4();
extern void func_00051540();
extern void func_00051374();
extern void func_00077D7C();

void func_001CB290(void)
{
    int s;
    char *nm = D_001C0A40;
    SetDispMask(0);
    func_0007457C(*(int *)(current + 4), 0x1080);
    func_0007457C(*(int *)(D_00097524 + 4), 0x1080);
    func_00073F88(1);
    func_00079800(0);
    ShowLoadingScreen(nm, 1, 0x17);
    Loadfile(D_001C0A50, 0xE6600);
    s = D_00096834;
    D_00096834 = 4;
    func_00022818();
    D_00096834 = s;
    func_001CBA18();
    func_000513B4();
    func_00051540();
    func_00051374();
    func_00077D7C(0x1234);
    cameraview = 0;
    D_00097534 = 0;
    SetDispMask(1);
}
