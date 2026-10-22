extern int *D_00097048;
extern int D_0009D904[];
extern void Getlong();

void func_00021CA8(void) {
    int n;
    short *p; int *base;
    int i;
    int t, cnt;
    base = D_00097048;
    p = (short *)(base + 1);
    Getlong(&n, base);
    for (i = 0x383; i >= 0; i--) D_0009D904[i] = 0;
    cnt = n;
    for (i = 0; i < cnt; i++) {
        D_0009D904[*p] = (int)p;
        t = p[3];
        p = (short *)((char *)p + (t * 0xD * 4 + 8));
    }
}
