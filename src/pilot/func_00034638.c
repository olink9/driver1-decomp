/* Driver 2: UnpackRegion */
extern short D_0009728C[];
extern int D_000A8024[];
extern int D_0009E734[];
extern int D_0009727C;
extern int func_000342D4();

void func_00034638(int a, int b) {
    if (D_0009728C[b] == -1) {
        if (func_000342D4()) {
            D_0009727C++;
        }
        D_000A8024[b] = a;
        D_0009E734[b] = a;
    }
}
