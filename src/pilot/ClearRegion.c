extern long long D_0009733C;
extern long long D_000970E0;
extern short *D_0009E724[];
extern int *D_0009D8F4[];
extern int *D_000AA7E8[];

void ClearRegion(int a0)
{
    int s;
    int n;
    int i;
    unsigned short *p;
    short *q;
    int *r;
    int *z;

    s = *(int *)&D_0009733C;
    n = a0 * s * s;
    q = D_0009E724[a0];
    r = D_0009D8F4[a0];
    p = (unsigned short *)*(int *)&D_000970E0 + n;
    for (i = s * s; i != 0; i--)
        *p++ = 0xFFFF;
    for (i = 0xE10; i != 0; i--) {
        *q++ = -1;
        *r++ = 0x38000000;
    }
    z = D_000AA7E8[a0];
    for (i = 0x384; i != 0; i--)
        *z++ = 0;
}
