/* JPlayEffects (Driver 2 name) */
typedef struct {
    char pad0[0x28];
    unsigned char SongVolume;
} XMSONG;
typedef struct {
    char pad0[0x34];
    unsigned char note;
    unsigned char vol;
    unsigned char dat;
    unsigned char ins;
    unsigned char eff;
    char pad39[0x4C - 0x39];
    unsigned char panning;
    char pad4D;
    unsigned char volume;
    char pad4F;
    unsigned short period;
    char pad52[0x56 - 0x52];
    short ownper;
    short ownvol;
    char pad5A[0x60 - 0x5A];
    unsigned char tmpvolume;
    char pad61;
    unsigned short tmpperiod;
} XMCHANNEL;

extern XMSONG *D_00097814;
extern XMCHANNEL *XMC;
extern void SPE(unsigned char, unsigned char);
extern void SetNote(unsigned char);
extern void DoXMPanSlide(unsigned char);
extern void DoS3MRetrig(unsigned char);

void JPlayEffects(void)
{
    unsigned char vol;
    unsigned char eff;
    unsigned char dat;

    vol = XMC->vol;
    eff = XMC->eff;
    dat = XMC->dat;

    XMC->ownper = 0;
    XMC->ownvol = 0;

    switch (vol >> 4) {
    case 0x6:
        if (vol & 0xf)
            SPE(0xa, vol & 0xf);
        break;
    case 0x7:
        if (vol & 0xf)
            SPE(0xa, vol << 4);
        break;
    case 0x8:
        SPE(0xe, 0xb0 | (vol & 0xf));
        break;
    case 0x9:
        SPE(0xe, 0xa0 | (vol & 0xf));
        break;
    case 0xa:
        SPE(0x4, vol << 4);
        break;
    case 0xb:
        SPE(0x4, vol & 0xf);
        break;
    case 0xc:
        XMC->panning = (vol & 0xf) << 4;
        break;
    case 0xd:
        if (vol & 0xf)
            DoXMPanSlide(vol & 0xf);
        break;
    case 0xe:
        if (vol & 0xf)
            DoXMPanSlide(vol << 4);
        break;
    case 0xf:
        SPE(0x3, vol << 4);
        break;
    default:
        if (vol >= 0x10 && vol <= 0x50)
            SPE(0xc, vol - 0x10);
    }

    switch (eff) {
    case 'G' - 55:
        D_00097814->SongVolume = dat * 2;
        break;
    case 'H' - 55:
        break;
    case 'K' - 55:
        SetNote(96);
        break;
    case 'L' - 55:
        break;
    case 'P' - 55:
        XMC->panning = dat;
        break;
    case 'R' - 55:
        DoS3MRetrig(dat);
        break;
    case 'T' - 55:
        SPE(0x6, dat);
        break;
    case 'X' - 55:
        break;
    default:
        if (eff <= 0xf)
            SPE(eff, dat);
        break;
    }

    if (!XMC->ownper)
        XMC->period = XMC->tmpperiod;
    if (!XMC->ownvol)
        XMC->volume = XMC->tmpvolume;
}
