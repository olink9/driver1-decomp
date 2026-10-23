/* DoEEffects (Driver 2 name) */
typedef struct {
    char pad0[0xC];
    unsigned short reppos;
    unsigned short repcnt;
    unsigned short vbtick;
    char pad12[2];
    unsigned char patdly;
    unsigned char patdly2;
    char pad16[0x1A - 0x16];
    unsigned short PatternPos;
} XMSONG;
typedef struct {
    char pad0[0x3B];
    unsigned char oldfvslide;
    unsigned char oldfslide;
    char pad3D[0x41 - 0x3D];
    unsigned char kick;
    char pad42[0x4C - 0x42];
    unsigned char panning;
    char pad4D[0x5C - 0x4D];
    unsigned char retrig;
    char pad5D[0x60 - 0x5D];
    unsigned char tmpvolume;
    char pad61;
    unsigned short tmpperiod;
    char pad64[0x6E - 0x64];
    unsigned char glissando;
    unsigned char wavecontrol;
} XMCHANNEL;

extern XMSONG *D_00097814;
extern XMCHANNEL *XMC;
#define ms D_00097814
#define XMC XMC

void DoEEffects(unsigned char dat)
{
    unsigned char nib;

    nib = dat & 0xf;

    switch (dat >> 4) {
    case 0x0:
        break;
    case 0x1:
        if (!ms->vbtick) {
            if (nib == 0)
                nib = XMC->oldfslide;
            XMC->oldfslide = nib;
            XMC->tmpperiod += (nib << 2);
        }
        break;
    case 0x2:
        if (!ms->vbtick) {
            if (nib == 0)
                nib = XMC->oldfslide;
            XMC->oldfslide = nib;
            XMC->tmpperiod -= (nib << 2);
        }
        break;
    case 0x3:
        XMC->glissando = nib;
        break;
    case 0x4:
        XMC->wavecontrol &= 0xf0;
        XMC->wavecontrol |= nib;
        break;
    case 0x5:
        break;
    case 0x6:
        if (ms->vbtick)
            break;
        if (nib) {
            if (ms->repcnt > 0)
                ms->repcnt--;
            else
                ms->repcnt = nib;
            if (ms->repcnt)
                ms->PatternPos = ms->reppos;
        } else {
            ms->reppos = ms->PatternPos - 1;
        }
        break;
    case 0x7:
        XMC->wavecontrol &= 0x0f;
        XMC->wavecontrol |= nib << 4;
        break;
    case 0x8:
        XMC->panning = nib << 4;
        break;
    case 0x9:
        if (nib > 0) {
            if (XMC->retrig == 0) {
                XMC->kick = 1;
                XMC->retrig = nib;
            }
            XMC->retrig--;
        }
        break;
    case 0xA:
        if (ms->vbtick)
            break;
        if (nib == 0)
            nib = XMC->oldfvslide;
        XMC->oldfvslide = nib;
        XMC->tmpvolume += nib;
        if (XMC->tmpvolume > 128)
            XMC->tmpvolume = 128;
        break;
    case 0xB:
        if (ms->vbtick)
            break;
        if (nib == 0)
            nib = XMC->oldfvslide;
        XMC->oldfvslide = nib;
        XMC->tmpvolume -= nib;
        if (XMC->tmpvolume < 64)
            XMC->tmpvolume = 64;
        break;
    case 0xC:
        if (ms->vbtick >= nib)
            XMC->tmpvolume = 64;
        break;
    case 0xD:
        if (ms->vbtick == nib)
            XMC->kick = 1;
        else
            XMC->kick = 0;
        break;
    case 0xE:
        if (ms->vbtick)
            break;
        if (!ms->patdly2)
            ms->patdly = nib + 1;
        break;
    case 0xF:
        break;
    }
}
