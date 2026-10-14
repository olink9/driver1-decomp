extern int SquareRoot0();
extern int ratan2();
int func_0003D600(int *a, int *b, short *out)
{
    int d[3];
    int r, l;
    d[0] = b[0] - a[0];
    d[1] = b[1] - a[1];
    d[2] = b[2] - a[2];
    l = SquareRoot0(d[0] * d[0] + d[2] * d[2]);
    out[0] = (0x400 - ratan2(l, d[1])) & 0xFFF;
    out[1] = -ratan2(d[0], d[2]) & 0xFFF;
    out[2] = 0;
    return l;
}
