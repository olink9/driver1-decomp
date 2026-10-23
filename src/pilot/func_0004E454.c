extern int D_00096C90, D_00096C88;
extern void XM_SetMasterVol();

void func_0004E454(int x) {
    int v;

    D_00096C90 = x;
    v = x + 10000;
    /* clamp 0..10000; the sign-bit test keeps the redundant bltz into the else branch */
    if (v & 0x80000000)
        goto lo;
    if (v <= 10000) {
lo:
        if (v < 0)
            v = 0;
    } else
        v = 10000;
    XM_SetMasterVol(D_00096C88, ((v >> 7) + (v >> 8)) & 0xFF);
}
