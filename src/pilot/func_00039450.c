extern void Getlong();

void func_00039450(int *a, int *b) {
    Getlong(a, b);
    Getlong(a + 1, b + 1);
    a[2] = (a[0] + 1) * 750;
    a[3] = a[1] * 750;
}
