extern int D_0009E714[];
extern long long D_00097374;
extern int *D_0009D8F4[];

int func_00021690(int a, int b)
{
    int x, y, t, xc, yc, xr, yr;

    x = (a + D_0009E714[2]) / 1500;
    t = D_0009E714[3] + 0x2EE;
    y = (t - b) / 1500;
    if (x < 0 || x >= D_0009E714[0] || y < 0 || y >= D_0009E714[1])
        return 0;
    yc = y / 60;
    xc = x / 60;
    xr = x % 60;
    yr = y % 60;
    return D_0009D8F4[(xc & 1) + (((*(int *)&D_00097374 - yc) & 1) << 1)][yr * 60 + xr];
}
