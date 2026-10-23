extern int D_00096C90;
extern int D_00096C98;
extern void func_0006F478();
extern void func_001C6990();

void func_001C6D78(void)
{
    if (D_00096C98 != 0) {
        if (D_00096C90 >= -9999) {
            D_00096C90 -= 100;
            if (D_00096C90 < -10000)
                D_00096C90 = -10000;
            func_0006F478(D_00096C90);
            func_001C6990();
        }
    }
}
