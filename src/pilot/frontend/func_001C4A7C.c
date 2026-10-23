/* FAKEMATCH: SetDispMask is declared `extern int` here (PsyQ libgpu declares it void). It matches as if the original called it without a prototype (implicit int); unconfirmed. */
/* func_001C4A7C: menu exit. Plain C once SetDispMask is declared returning int (v0 live after the call - probably an
   implicit-int K&R call in the original; PsyQ's SetDispMask is void). */
extern int D_00096808;
extern int D_001D2DC0;
extern char D_001CDFC0[];
extern char D_001CDFE8[];
extern void func_001C11E4();
extern void DrawSync();
extern void VSync();
extern void func_001C2114();
extern int SetDispMask();

int func_001C4A7C(void)
{
    func_001C11E4();
    DrawSync(0);
    VSync(0);
    func_001C2114(D_001CDFC0, D_001CDFE8, 0);
    SetDispMask(0);
    D_00096808 += D_001D2DC0;
    return D_001D2DC0 + 1;
}
