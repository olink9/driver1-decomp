void func_0004C9EC(int *a, int *b, int n) {
    int k, v;
    while (n--) {
        v = *b++;
        if (a[0] < 7) {
            k = a[0]++;
            while (--k != -1) a[k + 2] = a[k + 1];
            a[1] = v;
        }
    }
}
