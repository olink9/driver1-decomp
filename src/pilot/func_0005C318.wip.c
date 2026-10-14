extern unsigned char D_0008B6D8[];
extern char D_00010E44[];
extern int D_00096DE0;
extern int D_0009766C;
extern int func_0005C3D0();
extern void func_00079DB0();
int func_0005C318(int a, int b) {
    int i = 0;
    unsigned char *base;
    unsigned char *p;
    unsigned char end;
    if (D_0008B6D8[0] != 0xFF) {
        end = 0xFF;
        base = D_0008B6D8;
        __asm__ volatile("");
        p = base;
        do {
            if (*p == a) break;
            p++;
            i++;
        } while (*p != end);
        if (base[i] != 0xFF) {
            if (func_0005C3D0(D_00010E44, i * 20 + b, 0xD8600, D_00096DE0)) {
                func_00079DB0(D_0009766C, 0xD8600, D_00096DE0);
                return 1;
            }
        }
    }
    return 0;
}
