struct Q { int a, b, c, d; };
extern int func_0006B320();
int func_0006B450(struct Q *p, int n) {
    int r = 0;
    while (n--) {
        r = func_0006B320(p++);
    }
    return r;
}
