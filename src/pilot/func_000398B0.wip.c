extern int D_00097398, D_00097368, D_000972FC, D_00097300, D_0009735C, D_00097364;
extern unsigned char D_000A80C4[];

int func_000398B0(int *p)
{
    int x, z;
    int r;

    x = (p[0] + D_00097398) / 3000 - D_000972FC;
    z = (p[2] + D_00097368) / 3000 - D_00097300;
    return abs(x) <= D_0009735C && abs(z) <= D_0009735C
        && (x += 9, z += 9, D_000A80C4[x + z * D_00097364] != 0);
}
