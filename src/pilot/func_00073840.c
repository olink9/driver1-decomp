/* XM_SetVAGAddress */
extern int D_000C78E8[][34];
extern short D_0008D208[];
void func_00073840(int a, int b, int c) {
    D_000C78E8[a][b] = c;
    D_0008D208[a] = b + 1;
}
