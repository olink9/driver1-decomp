extern int D_0009E714[];
extern long long D_00097374, D_0009737C;
extern int D_000A8024[];

int func_0005BDF8(int a, int b)
{
    int x, y, t, idx;

    x = (a + D_0009E714[2]) / 1500;
    t = D_0009E714[3] + 0x2EE;
    y = (t - b) / 1500;
    if (x < 0 || x >= D_0009E714[0] || y < 0 || y >= D_0009E714[1])
        return 0;
    idx = (*(int *)&D_00097374 - y / 60) * *(int *)&D_0009737C + x / 60;
    if (idx == D_000A8024[0] || idx == D_000A8024[1] ||
        idx == D_000A8024[2] || idx == D_000A8024[3])
        return idx + 1;
    return 0;
}
