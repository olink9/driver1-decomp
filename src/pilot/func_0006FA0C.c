/* XM_Update */
int VSync(int);
void func_00071A10(void);
extern int D_00096FA0;
extern int D_00097844;
void func_0006FA0C(void) {
    D_00096FA0 = VSync(1);
    if (D_00097844 == 0) {
        func_00071A10();
        D_00096FA0 = VSync(1) - D_00096FA0;
    }
}
