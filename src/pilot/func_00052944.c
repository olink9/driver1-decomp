extern long long D_00097098;
extern void func_00079DB0();
#define D (*(int *)&D_00097098)

int *func_00052944(int *src, int x, int y)
{
    int *h;
    int n;
    int s;
    s = (int)src + 4;
    h = (int *)D;
    n = *src;
    func_00079DB0(h, s, n);
    D = D + n;
    if (x == 0)
        D = (int)h + h[5];
    if (y == 0)
        D = (int)h + h[7];
    D = (D + 3) & ~3;
    h[4] = h[4] + (int)h;
    h[6] = h[6] + (int)h;
    h[7] = y ? h[7] + (int)h : h[7] + s;
    h[5] = x ? h[5] + (int)h : h[5] + s;
    return h;
}
