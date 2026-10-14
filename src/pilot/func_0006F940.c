/* XM_CloseVAB */
extern short D_0008D208[];
void func_00073630(int);
void func_0006F940(int a) {
    if (D_0008D208[a] != -1) {
        func_00073630(a);
        D_0008D208[a] = -1;
    }
}
