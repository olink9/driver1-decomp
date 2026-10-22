extern long long D_00096770;
extern int D_00097704;
extern int D_00097708;
extern char D_000BC9BC[];
extern void PlaySpeech();

void func_000650E4(char *base, int idx, short amt)
{
    short *p;
    int t;
    unsigned char i;

    if (*(int *)&D_00096770 == 0)
        return;
    p = (short *)(base + (idx & 0xff) * 4);
    if (p[0] < p[1])
        return;
    if (D_00097704 == 0)
        return;
    if (p[0x16] != 0)
        return;
    p[0x16] = p[0x17];
    if (D_00097708 >= 0x293)
        t = (p[0x2D] * *(int *)(base + 0x84)) >> 12;
    else
        t = p[0x2C];
    D_00097708 += (t * amt) >> 12;
    if (D_00097708 > 0x1000)
        D_00097708 = 0x1000;
    i = idx;
    if (i == 4)
        PlaySpeech(D_000BC9BC, 7);
    if (i == 3)
        PlaySpeech(D_000BC9BC, 10);
}
