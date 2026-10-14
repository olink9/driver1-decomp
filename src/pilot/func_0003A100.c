extern int D_00097398, D_00097368, D_0009733C, D_00096A24;
extern void func_0003A270();

void func_0003A100(int *p, int a, int *ox, int *oz)
{
    int cx, cz, q1, q2, rx, rz;
    int x = p[0] + D_00097398;
    int z = p[2] + D_00097368;
    cx = x / 3000;
    cz = z / 3000;
    *ox = cx;
    *oz = cz;
    q1 = cz / D_0009733C;
    q2 = D_00096A24 / D_0009733C;
    rx = cx / D_0009733C;
    {
        int idx = rx + q1 * q2;
        int sub = (cz - q1 * D_0009733C) * D_0009733C + (cx - rx * D_0009733C);
        func_0003A270((rx & 1) | ((q1 & 1) << 1), idx, sub, a);
    }
}
