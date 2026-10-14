extern unsigned char D_000BDD40[];
extern int func_000776E4();

void func_0004FF4C(void) {
    int i;
    int a, b;
    unsigned char t;
    unsigned char *d;
    unsigned char *p = D_000BDD40 + 255;
    for (i = 255; i >= 0; i--) *p-- = i;
    d = D_000BDD40;
    for (i = 0; i < 1000; i++) {
        a = func_000776E4();
        b = func_000776E4();
        t = d[a & 0xff];
        d[a & 0xff] = d[b & 0xff];
        d[b & 0xff] = t;
    }
}
