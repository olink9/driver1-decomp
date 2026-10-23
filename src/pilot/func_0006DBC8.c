typedef struct { int x, y, z; } V3;
typedef struct { int a, b, c, d; } E16;
extern short D_00097538[];
extern int D_00096F80;
extern int D_00096F84;
extern int D_000977D4;
extern int D_000977D8;
extern char cameraview[8];
extern E16 D_000C71EC[];
extern E16 D_000C74CC[];
extern V3 D_000C77C4;
extern char camera_position[];
extern short D_00096C48[4];
extern short D_00096C4A[4];
extern short D_00096C4C[4];
extern void func_0004E024();
extern void func_0006D688();

void func_0006DBC8(void) {
    V3 v;
    if (D_00097538[0] != 0) {
        D_00096F80 = 4;
        func_0004E024();
        D_000977D4 = 0;
        cameraview[0] = 0;
        D_000977D8 = D_000977D8 + 1;
    }
    func_0006D688(&D_000C71EC[D_000977D8], camera_position, 0, D_000977D4);
    v.x = D_000C77C4.x; v.y = D_000C77C4.y; v.z = D_000C77C4.z;
    func_0006D688(&D_000C74CC[D_000977D8], &v, 0, D_000977D4);
    D_000C77C4.x = v.x; D_000C77C4.y = v.y; D_000C77C4.z = v.z;
    D_000977D4 = D_000977D4 + 1;
    if (D_000977D4 >= 21) {
        D_000977D4 = 1;
        D_000977D8 = D_000977D8 + 1;
        if (D_000977D8 == D_00096F84 - 3) {
            D_00096F80 = 4;
            D_000977D8 = D_00096F84 - 2;
            D_000977D4 = 0;
            cameraview[0] = 0;
            func_0004E024();
        }
    }
    D_00096C48[0] = D_000C77C4.x & 0xfff;
    D_00096C4A[0] = D_000C77C4.y & 0xfff;
    D_00096C4C[0] = D_000C77C4.z & 0xfff;
}
