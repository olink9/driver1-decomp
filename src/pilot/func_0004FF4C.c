extern unsigned char D_000BDD40[];
extern int func_000776E4();

void func_0004FF4C(void) {
    int i;
    int a, b;
    unsigned char t;
    unsigned char *p;
    i = 255; p = D_000BDD40; p += 255;
    for (; i >= 0; p--, i--) *p = i;
    for (i = 0; i < 1000; i++) {
        a = func_000776E4();
        b = func_000776E4();
        t = D_000BDD40[a & 0xff];
        D_000BDD40[a & 0xff] = D_000BDD40[b & 0xff];
        D_000BDD40[b & 0xff] = t;
    }
}
