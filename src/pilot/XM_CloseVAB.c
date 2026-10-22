/* XM_CloseVAB */
extern short D_0008D208[];
void ClearSPU(int);
void XM_CloseVAB(int a) {
    if (D_0008D208[a] != -1) {
        ClearSPU(a);
        D_0008D208[a] = -1;
    }
}
