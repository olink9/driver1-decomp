/* SPE (Driver 2 name) */
typedef struct {
    char pad0[0x10];
    unsigned short vbtick;
    unsigned short patbrk;
    char pad14;
    unsigned char patdly2;
    unsigned short numrow;
    short posjmp;
    char pad1A[2];
    short SongPos;
    char pad1E[2];
    short SongSpeed;
    short SongBPM;
    char pad24[0xBA0 - 0x24];
    short BPlayFlag;
} XMSONG;
typedef struct {
    char pad0[0x14];
    unsigned int SOffset;
    char pad18[0x3A - 0x18];
    unsigned char oldvslide;
    char pad3B[0x41 - 0x3B];
    unsigned char kick;
    char pad42[0x4C - 0x42];
    unsigned char panning;
    char pad4D[0x56 - 0x4D];
    short ownper;
    short ownvol;
    char pad5A[0x60 - 0x5A];
    unsigned char tmpvolume;
    char pad61;
    unsigned short tmpperiod;
    char pad64[2];
    unsigned short slidespeed;
    unsigned short portspeed;
    char pad6A[0x71 - 0x6A];
    unsigned char vibspd;
    unsigned char vibdepth;
    char pad73;
    unsigned char trmspd;
    unsigned char trmdepth;
    char pad76;
    unsigned char Dolby;
} XMCHANNEL;

extern XMSONG *D_00097814;
extern XMCHANNEL *D_0009781C;
#define ms D_00097814
#define XMC D_0009781C

extern void func_000712E4(unsigned char dat); /* Arpeggio */
extern void func_00071724(void);              /* DoToneSlide */
extern void func_0007178C(void);              /* DoVibrato */
extern void func_000718A8(void);              /* DoTremolo */
extern void func_000713DC(unsigned char dat); /* DoVolSlide */
extern void func_00070B68(unsigned char dat); /* DoEEffects */

void func_00070728(unsigned char eff, unsigned char dat)
{
    int hi;
    int lo;

    switch (eff) {
    case 0:
        func_000712E4(dat);
        break;
    case 1:
        if (dat != 0)
            XMC->slidespeed = ((unsigned short)(dat)) << 2;
        if (ms->vbtick)
            XMC->tmpperiod -= XMC->slidespeed;
        break;
    case 2:
        if (dat != 0)
            XMC->slidespeed = (unsigned short)(dat) << 2;
        if (ms->vbtick)
            XMC->tmpperiod += XMC->slidespeed;
        break;
    case 3:
        XMC->kick = 0;
        if (dat != 0) {
            XMC->portspeed = dat;
            XMC->portspeed <<= 2;
        }
        func_00071724();
        XMC->ownper = 1;
        break;
    case 4:
        if (dat & 0x0f) XMC->vibdepth = dat & 0x0f;
        if (dat & 0xf0) XMC->vibspd = (dat & 0xf0) >> 2;
        func_0007178C();
        XMC->ownper = 1;
        break;
    case 5:
        XMC->kick = 0;
        func_00071724();
        if (dat == 0)
            dat = XMC->oldvslide;
        XMC->oldvslide = dat;
        func_000713DC(dat);
        XMC->ownper = 1;
        break;
    case 6:
        func_0007178C();
        if (dat == 0)
            dat = XMC->oldvslide;
        XMC->oldvslide = dat;
        func_000713DC(dat);
        XMC->ownper = 1;
        break;
    case 7:
        if (dat & 0x0f) XMC->trmdepth = dat & 0xf;
        if (dat & 0xf0) XMC->trmspd = (dat & 0xf0) >> 2;
        func_000718A8();
        XMC->ownvol = 1;
        break;
    case 8:
        XMC->panning = dat;
        break;
    case 9:
        if (ms->vbtick)
            break;
        if (dat == 0)
            break;
        XMC->SOffset = dat;
        XMC->SOffset *= 256;
        XMC->SOffset *= 2;
        XMC->SOffset /= 7;
        XMC->SOffset *= 2;
        XMC->SOffset &= (~31);
        break;
    case 0xA:
        if (dat == 0)
            dat = XMC->oldvslide;
        XMC->oldvslide = dat;
        func_000713DC(dat);
        break;
    case 0xB:
        if (ms->patdly2)
            break;
        ms->patbrk = 0;
        ms->SongPos = dat - 1;
        ms->posjmp = 2;
        break;
    case 0xC:
        if (ms->vbtick)
            break;
        if (dat == 0xff)
            ms->BPlayFlag = 1;
        else if (dat == 0xfe)
            ms->BPlayFlag = 0;
        else if (dat == 0xfd)
            ms->BPlayFlag = 2;
        else if (dat == 0xfc)
            XMC->Dolby = 0;
        else if (dat == 0xfb)
            XMC->Dolby = 1;
        else if (dat == 0xfa)
            XMC->Dolby = 2;
        else {
            if (dat > 64) dat = 64;
            XMC->tmpvolume = dat + 64;
        }
        break;
    case 0xD:
        if (ms->patdly2)
            break;
        hi = (dat & 0xf0) >> 4;
        lo = (dat & 0xf);
        ms->patbrk = ((hi * 10) + lo) + 1;
        if (ms->patbrk > (ms->numrow + 1))
            ms->patbrk = ms->numrow + 1;
        ms->posjmp = 2;
        break;
    case 0xE:
        func_00070B68(dat);
        break;
    case 0xF:
        if (ms->vbtick || ms->patdly2)
            break;
        if (dat < 0x20) {
            ms->SongSpeed = dat;
            ms->vbtick = 0;
        } else {
            ms->SongBPM = dat;
            ms->vbtick = 0;
        }
        break;
    default:
        break;
    }
}
