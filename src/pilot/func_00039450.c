extern void func_0004FB70();

void func_00039450(int *a, int *b) {
    func_0004FB70(a, b);
    func_0004FB70(a + 1, b + 1);
    a[2] = (a[0] + 1) * 750;
    a[3] = a[1] * 750;
}
