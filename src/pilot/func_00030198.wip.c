extern int D_0008A8F0[];
typedef struct { short x, y; } SV;
extern SV D_0008EAA8[];
extern short D_00096C4A[];
extern long long D_00097368, D_0009733C, D_00096A24, D_00097398, D_00097254, D_000972AC, D_000972C0;
extern int D_000A8024[];
#define G(x) (*(int *)&x)
int func_00030198(void)
{
    int pos[3];
    SV *tab = D_0008EAA8;
    int *bp = D_0008A8F0;
    int ang, sn, cs, x, z, a, b, c, r, t;
    unsigned short s;
    ang = (-D_00096C4A[0]) & 0xFFF;
    pos[0] = bp[0] + tab[ang].x * 7;
    pos[2] = bp[2] + tab[ang].y * 7;
    a = ((pos[2] + G(D_00097368)) / 3000) / G(D_0009733C);
    c = G(D_00096A24) / G(D_0009733C);
    b = ((pos[0] + G(D_00097398)) / 3000) / G(D_0009733C);
    t = a * c;
    r = D_000A8024[(b & 1) + ((a & 1) << 1)] != b + t;
    s = (*(unsigned short **)&D_000972AC)[b + t];
    if (s != 0xFFFF) {
        t = (*(unsigned char **)&D_00097254)[s + 8];
        if (t != G(D_000972C0) && t != 0xFF)
            r |= 2;
    }
    return r;
}
