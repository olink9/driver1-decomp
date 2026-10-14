/* Driver 2: ApplyEffs */
typedef struct {
    char p0[2];
    short LVol;     /* 0x02 */
    short RVol;     /* 0x04 */
    char p6[0x19 - 6];
    unsigned char ChDead;   /* 0x19 */
    unsigned char panenvflg; /* 0x1A */
    char p1b[0x26 - 0x1B];
    unsigned short keyoffspd; /* 0x26 */
    unsigned char envflg;   /* 0x28 */
    char p29[0x3E - 0x29];
    short fadevol;          /* 0x3E */
    unsigned char keyon;    /* 0x40 */
    unsigned char kick;     /* 0x41 */
    unsigned char sample;   /* 0x42 */
    char p43[0x4C - 0x43];
    unsigned char panning;  /* 0x4C */
    char p4d;
    unsigned char volume;   /* 0x4E */
    char p4f;
    unsigned short period;  /* 0x50 */
    char p52[0x5A - 0x52];
    unsigned short UserVol; /* 0x5A */
    char p5c[0x76 - 0x5C];
    unsigned char SPUChannel; /* 0x76 */
} XMCHANNEL;

typedef struct { char pad[0x14]; unsigned short XMPSXChannels; } MH;
typedef struct {
    char p0[0x28];
    unsigned char SongVolume; /* 0x28 */
    char p29[3];
    int XMActiveVoices;       /* 0x2C */
    char p30[0x50 - 0x30];
    int PlayMask;             /* 0x50 */
    char p54[0x58 - 0x54];
    char chnl[0xBAA - 0x58];
    unsigned short UserPan;   /* 0xBAA */
    unsigned char MaxChans;   /* 0xBAC */
} MS;

extern MS *D_00097814;
extern MH *D_00097818;
extern XMCHANNEL *D_0009781C;
extern short func_00072C18();
extern short func_00072E1C();
extern void func_000732B4();

#define ms D_00097814
#define mh D_00097818
#define XMC D_0009781C
#define ABS(x) ((x) < 0 ? -(x) : (x))

static inline short DoPan(short envpan, short pan)
{
    return (pan + (((envpan - 128) * (128 - ABS(pan - 128))) / 128));
}

static inline short DoVol(unsigned int a, short b, short c)
{
    a *= b;
    a *= c;
    a >>= 23;
    return (a);
}

void func_0007232C(void)
{
    short envpan, envvol;
    int t;
    int pmsk;
    short vlm;
    short Pn;
    unsigned char MaxChans;

    ms->XMActiveVoices = 0;
    MaxChans = ms->MaxChans;
    for (t = 0; t < mh->XMPSXChannels; t++) {
        if (MaxChans == 0)
            return;

        pmsk = ms->PlayMask;
        pmsk &= (1 << t);
        if (pmsk != 0) {
            MaxChans--;
            XMC = (XMCHANNEL *)(ms->chnl + t * 0x78);

            if (XMC->kick) {
                XMC->keyon = 1;
                XMC->fadevol = 32767;
                XMC->ChDead = 0;
            }
            if (!XMC->ChDead) {
                ms->XMActiveVoices++;

                if (XMC->period < 1)
                    XMC->period = 1;
                else if (XMC->period > 32768)
                    XMC->period = 32768;

                if (XMC->envflg & 1)
                    envvol = func_00072C18(256, XMC->keyon, XMC->sample);
                else
                    envvol = 256;
                if (XMC->panenvflg & 1)
                    envpan = func_00072E1C(128, XMC->keyon, XMC->sample);
                else
                    envpan = 128;

                vlm = XMC->volume - XMC->UserVol;
                if (vlm <= 64) {
                    XMC->LVol = XMC->RVol = 0;
                } else {
                    vlm = ((DoVol(XMC->fadevol, envvol, vlm - 64) * ms->SongVolume) / 128);

                    Pn = XMC->panning;
                    Pn += ms->UserPan;
                    if (Pn < 0)
                        Pn = 0;
                    else if (Pn > 255)
                        Pn = 255;
                    Pn = DoPan(envpan, Pn);

                    XMC->LVol = (vlm * (255 - Pn));
                    XMC->RVol = (vlm * Pn);
                }

                if ((!XMC->LVol) && (!XMC->RVol) && (!XMC->keyon)) {
                    func_000732B4(XMC->SPUChannel);
                    XMC->ChDead = 1;
                } else if (!XMC->keyon) {
                    XMC->fadevol -= XMC->keyoffspd;
                    if (XMC->fadevol < 0)
                        XMC->fadevol = 0;
                }
            }
        }
    }
}
