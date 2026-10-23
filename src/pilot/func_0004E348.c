/* FreeXM */
extern int D_00096C88;
extern int D_00096C8C;
void func_0006F8DC(void);
void func_000738C0(void);
void XM_CloseVAB2(int);
void func_0004E348(void) {
    if (D_00096C88 != -1) {
        func_0006F8DC();
        func_000738C0();
        XM_CloseVAB2(D_00096C8C);
        D_00096C88 = -1;
        D_00096C8C = -1;
    }
}
