extern long long D_00097098;
extern int D_0009D8F4[];
extern int D_0009E724[];
extern int D_0009E734[];
extern void func_00012498();

void func_000212F0(void)
{
    int i;
    unsigned short *q;
    unsigned short *base = (unsigned short *)*(int *)&D_00097098;

    for (i = 0; i < 4; i++) {
        D_0009E734[i] = -1;
        D_0009E724[i] = (int)base;
        for (q = base; q < base + 0x1C20 / 2; q++)
            *q = 0x4000;
        base = (unsigned short *)((char *)base + 0x1C20);
        D_0009D8F4[i] = (int)base;
        func_00012498(base, 0x3840);
        base = (unsigned short *)((char *)base + 0x3840);
    }
    *(int *)&D_00097098 = (int)base;
}
