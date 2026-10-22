/* Driver 2: XM_CloseVAB2 */
extern short D_0008D208[];
extern int D_000C78E8[][34];

void XM_CloseVAB2(int VabID) {
    int Amount, i;
    if (D_0008D208[VabID] != -1) {
        Amount = D_0008D208[VabID];
        for (i = 0; i < Amount; i++) {
            D_000C78E8[VabID][i] = 0;
        }
        D_0008D208[VabID] = -1;
    }
}
