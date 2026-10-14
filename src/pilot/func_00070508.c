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
extern XMCHANNEL *D_0009781C;
extern void func_00070728(unsigned char, unsigned char); /* SPE */
extern void func_00070EFC(unsigned char); /* SetNote */
extern void func_0007146C(unsigned char); /* DoXMPanSlide */
extern void func_00071504(unsigned char); /* DoS3MRetrig */

void func_00070508(void)
{
    unsigned char vol;
    unsigned char eff;
    unsigned char dat;

    vol = D_0009781C->vol;
    eff = D_0009781C->eff;
    dat = D_0009781C->dat;

    D_0009781C->ownper = 0;
    D_0009781C->ownvol = 0;

    switch (vol >> 4) {
    case 0x6:
        if (vol & 0xf)
            func_00070728(0xa, vol & 0xf);
        break;
    case 0x7:
        if (vol & 0xf)
            func_00070728(0xa, vol << 4);
        break;
    case 0x8:
        func_00070728(0xe, 0xb0 | (vol & 0xf));
        break;
    case 0x9:
        func_00070728(0xe, 0xa0 | (vol & 0xf));
        break;
    case 0xa:
        func_00070728(0x4, vol << 4);
        break;
    case 0xb:
        func_00070728(0x4, vol & 0xf);
        break;
    case 0xc:
        D_0009781C->panning = (vol & 0xf) << 4;
        break;
    case 0xd:
        if (vol & 0xf)
            func_0007146C(vol & 0xf);
        break;
    case 0xe:
        if (vol & 0xf)
            func_0007146C(vol << 4);
        break;
    case 0xf:
        func_00070728(0x3, vol << 4);
        break;
    default:
        if (vol >= 0x10 && vol <= 0x50)
            func_00070728(0xc, vol - 0x10);
    }

    switch (eff) {
    case 'G' - 55:
        D_00097814->SongVolume = dat * 2;
        break;
    case 'H' - 55:
        break;
    case 'K' - 55:
        func_00070EFC(96);
        break;
    case 'L' - 55:
        break;
    case 'P' - 55:
        D_0009781C->panning = dat;
        break;
    case 'R' - 55:
        func_00071504(dat);
        break;
    case 'T' - 55:
        func_00070728(0x6, dat);
        break;
    case 'X' - 55:
        break;
    default:
        if (eff <= 0xf)
            func_00070728(eff, dat);
        break;
    }

    if (!D_0009781C->ownper)
        D_0009781C->period = D_0009781C->tmpperiod;
    if (!D_0009781C->ownvol)
        D_0009781C->volume = D_0009781C->tmpvolume;
}
