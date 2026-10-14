/* SetChannelPitch */
typedef struct {
    int mask0; int mask; int p2[3]; unsigned short pitch; short p3;
    char p4[0x42 - 0x18]; unsigned short time; int rate;
    int p5[2]; short srcpitch; unsigned short dop; char pad[0x84 - 0x54];
} C;
extern int D_00096C74;
extern C D_000BD498[];
extern void SpuSetVoiceAttr();
void func_0004DACC(unsigned channel, int pitch) {
    C *chan;
    C *base;
    if (channel < 16 && D_00096C74 == 0) {
        base = D_000BD498;
        chan = base + channel;
        if (chan->time != 0 && pitch > 128) {
            chan->srcpitch = pitch;
            pitch = (chan->dop * pitch) >> 12;
            pitch = (chan->rate * pitch) / 44100;
            if (pitch >= 0x4000) pitch = 0x3FFF;
            chan->mask = 16;
            chan->pitch = pitch;
            SpuSetVoiceAttr(chan);
        }
    }
}
