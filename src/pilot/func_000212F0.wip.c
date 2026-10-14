extern long long D_00097098;
extern int D_0009D8F4[];
extern int D_0009E724[];
extern int D_0009E734[];
extern void func_00012498();

void func_000212F0(void)
{
    unsigned short *q;
    unsigned short *end;
    unsigned short *base = (unsigned short *)*(int *)&D_00097098;
    int a = (int)D_0009D8F4;
    int e = a;
    int *b = D_0009E724;
    int *c = D_0009E734;

    do {
        *c = -1;
        *b = (int)base;
        for (q = base, end = base + 0x1C20 / 2; q < end; q++)
            *q = 0x4000;
        base = (unsigned short *)((char *)base + 0x1C20);
        *(int *)a = (int)base;
        func_00012498(base, 0x3840);
        base = (unsigned short *)((char *)base + 0x3840);
        a += 4; b++; c++;
    } while (a < e + 16);
    *(int *)&D_00097098 = (int)base;
}
