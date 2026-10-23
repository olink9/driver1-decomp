/* Driver 2: StartSound */
typedef struct { int pad0[18]; int srcvolume; int pad1[14]; } CHAN;
extern CHAN D_000BD498[];
extern int GetFreeChannel();
extern int CompleteSoundSetup();

int StartSound(int channel, int bank, int sample, int volume, int pitch) {
    int ch = channel;
    if (ch < 0)
        ch = GetFreeChannel();
    channel = ch;
    if ((unsigned)channel >= 16)
        return -1;
    D_000BD498[channel].srcvolume = volume;
    return CompleteSoundSetup(channel, bank, sample, pitch);
}
