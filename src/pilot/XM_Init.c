/* XM_Init - XM_Init (Driver 2 name, C/xmplay.c) */
typedef struct XMCHANNEL {
    char pad00[0x6];
    short OldLVol;      /* 0x06 */
    short OldRVol;      /* 0x08 */
    short OldPeriod;    /* 0x0A */
    unsigned char OldSample; /* 0x0C */
    char pad0D[0x18 - 0x0D];
    unsigned char nothing;   /* 0x18 */
    unsigned char ChDead;    /* 0x19 */
    char pad1A[0x34 - 0x1A];
    unsigned char ins;   /* 0x34 */
    unsigned char vol;   /* 0x35 */
    unsigned char dat;   /* 0x36 */
    char pad37;
    unsigned char eff;   /* 0x38 */
    char pad39[0x41 - 0x39];
    unsigned char UserVol; /* 0x41 */
    unsigned char sample;  /* 0x42 */
    char pad43[0x4C - 0x43];
    unsigned char panning; /* 0x4C */
    char pad4D[0x55 - 0x4D];
    unsigned char note;    /* 0x55 */
    char pad56[0x5A - 0x56];
    short kick;            /* 0x5A */
    unsigned char retrig;  /* 0x5C */
    char pad5D[0x60 - 0x5D];
    unsigned char tmpvolume; /* 0x60 */
    char pad61[0x6E - 0x61];
    unsigned char glissando;   /* 0x6E */
    unsigned char wavecontrol; /* 0x6F */
    char pad70[0x76 - 0x70];
    unsigned char SPUChannel; /* 0x76 */
    unsigned char Dolby;   /* 0x77 */
} XMCHANNEL;

typedef struct XMSONG {
    unsigned char Status;   /* 0x00 */
    char pad01[3];
    int HeaderNum;          /* 0x04 */
    int CurPos;             /* 0x08 */
    short reppos;           /* 0x0C */
    short repcnt;           /* 0x0E */
    short vbtick;           /* 0x10 */
    short patbrk;           /* 0x12 */
    unsigned char patdly;   /* 0x14 */
    unsigned char patdly2;  /* 0x15 */
    char pad16[2];
    short posjmp;           /* 0x18 */
    short PatternPos;       /* 0x1A */
    short SongPos;          /* 0x1C */
    char pad1E[2];
    short SongSpeed;        /* 0x20 */
    short SongBPM;          /* 0x22 */
    int SongLoop;           /* 0x24 */
    unsigned char SongVolume; /* 0x28 */
    char pad29[7];
    int NotAmiga;           /* 0x30 */
    unsigned char XMPlay;   /* 0x34 */
    char pad35[3];
    int FirstCh;            /* 0x38 */
    int JBPM;               /* 0x3C */
    int PCounter;           /* 0x40 */
    char pad44[0x50 - 0x44];
    int PlayMask;           /* 0x50 */
    int SFXNum;             /* 0x54 */
    XMCHANNEL XM_Chnl[24];  /* 0x58 */
    char padB98[0xB98 - 0x58 - 24 * 0x78];
    int JUp;                /* 0xB98 */
    short PlayNext;         /* 0xB9C */
    short BPlayNext;        /* 0xB9E */
    short BPlayFlag;        /* 0xBA0 */
    char padBA2[2];
    int CurrentStart;       /* 0xBA4 */
    unsigned char VabID;    /* 0xBA8 */
    char padBA9;
    short UserPan;          /* 0xBAA */
    unsigned char MaxChans; /* 0xBAC */
} XMSONG;

typedef struct XMHEADER {
    char pad00[4];
    unsigned short restart;   /* 0x04 */
    char pad06[6];
    unsigned short flags;     /* 0x0C */
    unsigned short tempo;     /* 0x0E */
    unsigned short bpm;       /* 0x10 */
    unsigned short XMChannels;    /* 0x12 */
    unsigned short XMPSXChannels; /* 0x14 */
} XMHEADER;

extern short D_0008D218[];   /* XMSongIDs */
extern short D_0008D248[];   /* XMSPU_SFX */
extern int D_00096FAC;       /* XM_NSA */
extern XMSONG *D_00097810;   /* mu */
extern XMCHANNEL *XMC; /* XMCU */
extern XMHEADER *D_00097828; /* mhu */
extern int D_000C7888[];     /* XM_SngAddress */
extern int D_000C7970[];     /* XM_HeaderAddress */

extern void InitSPUChannel(int);
extern int GetFreeSongID(void);
extern void func_00073738(int);  /* XM_Quit */
extern void JPClearSPUFlags(int);

#define mu D_00097810
#define mhu D_00097828
#define XMCU XMC

int XM_Init(int VabID, int XM_ID, int SongID, int FirstCh,
                  int Loop, int PlayMask, int PlayType, int SFXNum)
{
    int i;
    int pmsk;
    int Chn;
    int t;
    int SngID;
    int fr;

    if (SongID == -1) {
        SngID = GetFreeSongID();
        if (SngID == -1)
            return -1;
    } else {
        if (SongID >= D_00096FAC)
            return -1;
        func_00073738(SongID);
        D_0008D218[SongID] = 0;
        SngID = SongID;
    }

    mu = (XMSONG *)D_000C7888[SngID];
    mhu = (XMHEADER *)D_000C7970[XM_ID];

    mu->XMPlay = 0;
    mu->JUp = 0;
    mu->VabID = VabID;
    mu->SongLoop = Loop;
    mu->PlayMask = PlayMask;
    mu->SFXNum = SFXNum;

    mu->Status = PlayType & 0x7f;
    mu->HeaderNum = XM_ID;
    mu->JBPM = 0;
    mu->PCounter = 0;
    mu->reppos = mhu->restart;
    mu->repcnt = 0;
    mu->CurPos = 0;
    mu->SongPos = 0;
    if ((PlayType & 0x7f) == 0) {
        mu->SongPos = SFXNum;
        mu->CurrentStart = SFXNum;
    }
    mu->SongSpeed = mhu->tempo;
    mu->vbtick = mu->SongSpeed;
    mu->SongBPM = mhu->bpm;

    mu->NotAmiga = mhu->flags;
    mu->patdly = 0;
    mu->patdly2 = 0;
    mu->SongVolume = 128;
    mu->FirstCh = FirstCh;
    mu->PatternPos = 0;
    mu->posjmp = 1;
    mu->patbrk = 0;
    mu->PlayNext = -1;
    mu->BPlayNext = -1;
    mu->BPlayFlag = 0;
    mu->UserPan = 0;
    mu->MaxChans = 0;

    if (FirstCh != -1) {
        Chn = FirstCh;
        for (i = 0; i < mhu->XMPSXChannels; i++) {
            pmsk = mu->PlayMask;
            pmsk &= (1 << i);
            if (pmsk != 0) {
                XMCU = &mu->XM_Chnl[i];
                XMCU->SPUChannel = Chn;
                mu->MaxChans++;
                if (Chn < 24)
                    InitSPUChannel(Chn);
                Chn++;
            }
        }
    } else {
        for (i = 0; i < mhu->XMPSXChannels; i++) {
            pmsk = mu->PlayMask;
            pmsk &= (1 << i);
            if (pmsk != 0) {
                XMCU = &mu->XM_Chnl[i];
                fr = 0;
                for (t = 0; t < 24; t++) {
                    if (D_0008D248[t] == 0) {
                        D_0008D248[t] = SngID + 1;
                        XMCU->SPUChannel = t;
                        mu->MaxChans++;
                        InitSPUChannel(t);
                        fr = 1;
                        break;
                    }
                }
                if (fr == 0) {
                    JPClearSPUFlags(SngID + 1);
                    D_0008D218[SngID] = -1;
                    return -2;
                }
            }
        }
    }

    for (i = 0; i < mhu->XMChannels; i++) {
        mu->XM_Chnl[i].kick = 0;
        mu->XM_Chnl[i].UserVol = 0;
        mu->XM_Chnl[i].tmpvolume = 0 + 64;
        mu->XM_Chnl[i].retrig = 0;
        mu->XM_Chnl[i].wavecontrol = 0;
        mu->XM_Chnl[i].glissando = 0;
        mu->XM_Chnl[i].panning = 128;
        mu->XM_Chnl[i].note = 0;
        mu->XM_Chnl[i].vol = 0;
        mu->XM_Chnl[i].eff = 0;
        mu->XM_Chnl[i].dat = 0;
        mu->XM_Chnl[i].ins = 0;
        mu->XM_Chnl[i].ChDead = 1;
        mu->XM_Chnl[i].nothing = 1;
        mu->XM_Chnl[i].OldLVol = -1;
        mu->XM_Chnl[i].OldRVol = -1;
        mu->XM_Chnl[i].OldPeriod = 0;
        mu->XM_Chnl[i].sample = 254;
        mu->XM_Chnl[i].OldSample = -1;
        mu->XM_Chnl[i].Dolby = 0;
    }
    if (PlayType < 0x80)
        mu->XMPlay = 1;

    return SngID;
}
