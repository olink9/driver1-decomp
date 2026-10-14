/* cbready */
extern unsigned short D_000C782C[];
extern unsigned short D_000977F4, D_000977F6, D_00096F9C;
extern int D_00096F94, D_00096F98;
extern void func_0007A4FC();
extern void func_0007A27C();
extern void SsSetSerialVol();
void func_0006F760(int intr) {
    if (intr == 1) {
        func_0007A4FC(D_000C782C, 8);
        D_000977F4 = (D_000C782C[7] & 0x7C00) >> 10;
        D_000977F6 = D_000C782C[6];
        if (D_000977F6 == 0x160) {
            D_00096F94 |= intr << (D_000977F4 + 1);
            if (D_000977F4 == D_00096F9C || D_00096F94 == 0xFF) {
                SsSetSerialVol(0, 0, 0);
                func_0007A27C(9, 0);
                D_00096F98 = 0;
                D_00096F94 = 0;
            }
        }
    }
}
