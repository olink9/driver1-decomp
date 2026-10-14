typedef struct {
    char pad0[0x48];
    int srcvolume;
    char pad1[0x50 - 0x4C];
    short srcpitch;
    char pad2[0x5C - 0x52];
    int *srcposition;
    int pos[3];
    char pad3[132 - 0x6C];
} CH;
extern CH D_000BD498[];
extern int func_0004E4B0();
extern int func_0004D968();

/* Start3DSoundVolPitch */
int func_0004D7E4(int ch, int bank, int sample, int x, int y, int z, int volume, int pitch)
{
    CH *c;
    int n;

    n = ch;
    if (n < 0)
        n = func_0004E4B0();
    if ((unsigned)n >= 16)
        return -1;
    ch = n;
    c = &D_000BD498[ch];
    c->srcposition = D_000BD498[ch].pos;
    c->pos[0] = x;
    c->pos[1] = y;
    c->pos[2] = z;
    c->srcvolume = volume;
    c->srcpitch = pitch;
    return func_0004D968(ch, bank, sample, pitch);
}
