extern int D_00097058;
extern unsigned char D_00097054;
extern unsigned char D_00097050;
extern unsigned char D_00097038;

void func_00021C10(void) {
    if (--D_00097058 == 0) {
        D_00097054++;
        if (D_00097054 >= 5) D_00097054 = 1;
        D_00097050++;
        if (D_00097050 >= 5) D_00097050 = 1;
        if (D_00097054 == 1 || D_00097054 == 3) D_00097058 = 300;
        else D_00097058 = 100;
    }
    D_00097038++;
}
