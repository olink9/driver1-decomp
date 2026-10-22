/* Start3DSound (Driver 2 name) */
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
extern int GetFreeChannel();
extern int CompleteSoundSetup();
int func_0004D8B0(int ch, int bank, int sample, int x, int y, int z)
{
    CH *c;
    CH *b;
    int n;

    n = ch;
    if (ch < 0)
        n = GetFreeChannel();
    ch = n;
    if ((unsigned)ch >= 16)
        return -1;
    b = D_000BD498;
    c = &b[ch];
    c->srcposition = D_000BD498[ch].pos;
    c->pos[0] = x;
    c->pos[1] = y;
    c->pos[2] = z;
    c->srcvolume = 0;
    return CompleteSoundSetup(ch, bank, sample, 0x1000);
}
