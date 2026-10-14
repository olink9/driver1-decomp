int func_00072B8C(int a, int b)
{
    int d = b - a;
    if (d < 0)
        d = a - b;
    d >>= 1;
    d++;
    if (a < b)
        return a + d;
    return b + d;
}
