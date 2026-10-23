/* Driver 2: ClearSPU */
extern short D_0008D208[];
extern int D_000C78E8[][34];
extern void func_000736C4();

void ClearSPU(int vb) {
    int i;
    int amount = D_0008D208[vb];
    for (i = 0; i < amount; i++) {
        if (D_000C78E8[vb][i] != 0) {
            func_000736C4(D_000C78E8[vb][i]);
            D_000C78E8[vb][i] = 0;
        }
    }
}
