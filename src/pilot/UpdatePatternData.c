/* UpdatePatternData (Driver 2 name) */
typedef struct {
    char pad0[0x35];
    unsigned char vol;
    unsigned char dat;
    char pad37;
    unsigned char eff;
    char pad39[0x78 - 0x39];
} XMCHANNEL;
typedef struct {
    unsigned char Status;
    char pad1[0x8 - 0x1];
    int CurPos;
    unsigned short reppos;
    char padE[0x10 - 0xE];
    unsigned short vbtick;
    unsigned short patbrk;
    unsigned char patdly;
    unsigned char patdly2;
    unsigned short numrow;
    short posjmp;
    unsigned short PatternPos;
    short SongPos;
    short CurrentPattern;
    unsigned short SongSpeed;
    char pad22[0x24 - 0x22];
    int SongLoop;
    char pad28[0x34 - 0x28];
    unsigned char XMPlay;
    char pad35[0x44 - 0x35];
    unsigned short PatSize;
    char pad46[0x48 - 0x46];
    int PatAdr;
    int PatAdr2;
    int PlayMask;
    int SFXNum;
    XMCHANNEL XM_Chnl[24];
    char padB98[0xB9C - 0xB98];
    short PlayNext;
    short BPlayNext;
    short BPlayFlag;
    char padBA2[0xBA4 - 0xBA2];
    int CurrentStart;
} XMSONG;
typedef struct {
    unsigned short version;
    unsigned short songlength;
    char pad4[0x12 - 0x4];
    unsigned short XMChannels;
    unsigned short XMPSXChannels;
    char pad16[0x18 - 0x16];
    int JAP_PAT_ADDR[256];
    int JAP_PAT_ADDR2[256];
    char pad818[0xE18 - 0x818];
    unsigned char jorders[256];
} XMHEADER;

extern XMSONG *D_00097814;
extern XMHEADER *D_00097818;
extern XMCHANNEL *XMC;
extern int D_0009783C;
extern void func_00073738(int);
extern int JPlayNote(unsigned char *j, int pmsk);
extern int CalcPlayPos(int StartPos);
extern int func_0007220C(int n);

#define ms D_00097814
#define mh D_00097818
#define XMC XMC
#define CurrentCh D_0009783C

static inline int getWord(unsigned char *p)
{
    return p[0] + (p[1] << 8);
}

void UpdatePatternData(int SC)
{
    int SP, t, pmsk;
    int FindOffset;
    unsigned char *pataddr;
    unsigned char patdat;

    if (++ms->vbtick >= ms->SongSpeed) {
        ms->PatternPos++;
        ms->vbtick = 0;

        if (ms->patdly) {
            ms->patdly2 = ms->patdly;
            ms->patdly = 0;
        }

        if (ms->patdly2) {
            if (--ms->patdly2)
                ms->PatternPos--;
        }

        FindOffset = 0;

        if (ms->PatternPos == ms->numrow)
            ms->posjmp = 2;

        if (ms->posjmp) {
            if (ms->posjmp == 2 && ms->Status == 1) {
                if (ms->patbrk == 0) {
                    func_00073738(SC);
                    return;
                }
            }

            if (ms->patbrk != 0)
                ms->PatternPos = ms->patbrk - 1;
            else
                ms->PatternPos = 0;

            ms->CurPos = ms->PatternPos;
            if (ms->CurPos != 0)
                FindOffset = 1;

            if (ms->BPlayFlag == 2) {
                func_00073738(SC);
                return;
            }

            if (ms->BPlayNext != -1 && ms->BPlayFlag != 0) {
                ms->BPlayFlag = 0;
                ms->SongPos = ms->BPlayNext;
                ms->CurrentStart = ms->SongPos;
                ms->BPlayNext = -1;
            } else if (ms->PlayNext != -1) {
                ms->SongPos = ms->PlayNext;
                ms->CurrentStart = ms->SongPos;
                ms->PlayNext = -1;
            } else {
                ms->SongPos += ms->posjmp - 1;
                t = ms->SongPos;
            }

            ms->patbrk = ms->posjmp = 0;
            if (ms->SongPos >= mh->songlength) {
                if (!ms->SongLoop) {
                    ms->XMPlay = 0;
                    return;
                }
                ms->SongPos = ms->reppos;
            }
            if (ms->SongPos < 0)
                ms->SongPos = mh->songlength - 1;
            if (ms->Status == 0)
                SP = mh->jorders[ms->SongPos];
            else
                SP = ms->SFXNum;

            ms->CurrentPattern = SP;
            ms->PatAdr = mh->JAP_PAT_ADDR[SP];
            ms->PatAdr2 = mh->JAP_PAT_ADDR2[SP];
            ms->numrow = getWord((unsigned char *)ms->PatAdr + 5);
            ms->PatSize = getWord((unsigned char *)ms->PatAdr + 7);
        }
        if (!ms->patdly2) {
            if (ms->PatSize > 0) {
                if (mh->version == 0xddba) {
                    if (FindOffset != 0)
                        ms->CurPos = func_0007220C(ms->CurPos);
                    SP = ms->CurPos;
                    for (t = 0; t < mh->XMPSXChannels; t++) {
                        XMC = &ms->XM_Chnl[t];
                        XMC->eff = 0;
                        XMC->vol = 0;
                        XMC->dat = 0;
                    }

                    while (1) {
                        pataddr = ((unsigned char *)ms->PatAdr2 + SP);
                        patdat = *pataddr++;
                        SP++;
                        if (patdat == 0xff)
                            break;

                        CurrentCh = patdat;
                        XMC = &ms->XM_Chnl[patdat];
                        pmsk = ms->PlayMask;
                        pmsk &= (1 << patdat);
                        SP += JPlayNote((unsigned char *)ms->PatAdr2 + SP, pmsk);
                    }
                    ms->CurPos = SP;
                } else {
                    if (FindOffset != 0)
                        ms->CurPos = CalcPlayPos(ms->CurPos);
                    SP = ms->CurPos;
                    for (t = 0; t < mh->XMChannels; t++) {
                        CurrentCh = t;
                        XMC = &ms->XM_Chnl[t];
                        pmsk = ms->PlayMask;
                        pmsk &= (1 << t);
                        SP += JPlayNote((unsigned char *)ms->PatAdr2 + SP, pmsk);
                    }
                    ms->CurPos = SP;
                }
            }
        }
    }
}
