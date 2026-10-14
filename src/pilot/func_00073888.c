/* XM_SetSongAddress */
extern int D_00096FAC;
extern char *D_00097810;
extern int D_000C7888[];
void func_00073888(char *a0) {
    D_00096FAC = 0;
    D_000C7888[0] = (int)a0;
    D_00097810 = a0;
    *a0 = 0;
    D_00097810[0x34] = 0;
    D_00096FAC++;
}
