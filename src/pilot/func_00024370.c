int func_00024370(int x, int y, int *t)
{
    int x0 = t[0], y1 = t[3], y0 = t[1], x1 = t[2];
    int y2, x2;
    if ((x - x0) * (y1 - y0) - (y - y0) * (x1 - x0) < 0) return 0;
    y2 = t[5];
    x2 = t[4];
    if ((x - x1) * (y2 - y1) - (y - y1) * (x2 - x1) < 0) return 0;
    return ((x - x2) * (y0 - y2) - (y - y2) * (x0 - x2)) >= 0;
}
