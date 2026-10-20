extern int D_0008A8F0[];
extern short D_0008EAA8[];
extern long long D_00096C4A;
extern long long D_00097368, D_0009733C, D_00096A24, D_00097398, D_00097254, D_000972AC, D_000972C0;
extern int D_000A8024[];
#define G(x) (*(int *)&x)
int func_00030198(void)
{
    int pos[3];
    int *bp = D_0008A8F0;
    int ang, sn, cs, x, z, a, b, c, r, t, u;
    int s;
    ang = (-*(short *)&D_00096C4A) & 0xFFF;
    pos[0] = bp[0] + D_0008EAA8[ang*2] * 7;
    pos[2] = bp[2] + D_0008EAA8[ang*2+1] * 7;
    a = ((pos[2] + G(D_00097368)) / 3000) / G(D_0009733C);
    c = G(D_00096A24) / G(D_0009733C);
    b = ((pos[0] + G(D_00097398)) / 3000) / G(D_0009733C);
    t = a * c;
    r = D_000A8024[(b & 1) + ((a & 1) << 1)] != b + t;
    if ((*(unsigned short **)&D_000972AC)[b + t] != 0xFFFF) {
        s = (*(unsigned short **)&D_000972AC)[b + t];
        u = (*(unsigned char **)&D_00097254 + s)[8];
        if (u != G(D_000972C0) && u != 0xFF)
            r |= 2;
    }
    return r;
}
