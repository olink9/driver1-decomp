extern unsigned char D_0008B6D8[];
extern char D_00010E44[];
extern int D_00096DE0;
extern int D_0009766C;
extern int func_0005C3D0();
extern void func_00079DB0();
int func_0005C318(int a, int b) {
    int i = 0;
    while (D_0008B6D8[i] != 0xFF) {
        if (D_0008B6D8[i] == a) break;
        i++;
    }
    if (D_0008B6D8[i] != 0xFF) {
        if (func_0005C3D0(D_00010E44, i * 20 + b, 0xD8600, D_00096DE0)) {
            func_00079DB0(D_0009766C, 0xD8600, D_00096DE0);
            return 1;
        }
    }
    return 0;
}
