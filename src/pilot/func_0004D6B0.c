/* Driver 2: StartSound */
typedef struct { int pad0[18]; int srcvolume; int pad1[14]; } CHAN;
extern CHAN D_000BD498[];
extern int func_0004E4B0();
extern int func_0004D968();

int func_0004D6B0(int channel, int bank, int sample, int volume, int pitch) {
    int ch = channel;
    if (ch < 0)
        ch = func_0004E4B0();
    channel = ch;
    if ((unsigned)channel >= 16)
        return -1;
    D_000BD498[channel].srcvolume = volume;
    return func_0004D968(channel, bank, sample, pitch);
}
