/* Driver 2: InitXMData (xmplay.c) */
typedef struct {
    unsigned short version;          /* 0 */
    unsigned short songlength;       /* 2 */
    unsigned short restart;          /* 4 */
    unsigned short pad6;
    unsigned short numpat;           /* 8 */
    unsigned short numins;           /* A */
    unsigned short flags;            /* C */
    unsigned short tempo;            /* E */
    unsigned short bpm;              /* 10 */
    unsigned short XMChannels;       /* 12 */
    unsigned short XMPSXChannels;    /* 14 */
    unsigned short pad16;
    unsigned int *JAP_PAT_ADDR[256];        /* 18 */
    unsigned int *JAP_PAT_ADDR2[256];       /* 418 */
    unsigned int *JAP_InstrumentOffset[128];/* 818 */
    unsigned int *JAP_SampAddr[128];        /* A18 */
    unsigned int *JAP_SampHdrAddr[128];     /* C18 */
    unsigned char jorders[256];             /* E18 */
    int S3MPanning;                         /* F18 */
} XMHEADER;

extern XMHEADER *D_00097828;
extern XMHEADER *D_000C7970[];
extern unsigned long func_0006FD8C(unsigned char *mpp);

#define mhu D_00097828
#define getWord(p) ((p)[0] + ((p)[1] << 8))

int func_0006FB14(unsigned char *mpp, int XM_ID, int S3MPan)
{
    int t;
    int a;
    int c;
    unsigned long b;
    unsigned short b2;

    mhu = D_000C7970[XM_ID];
    mhu->S3MPanning = S3MPan;
    mhu->version = getWord(mpp + 0x3A);
    mhu->songlength = getWord(mpp + 0x40);
    mhu->restart = getWord(mpp + 0x42);
    mhu->XMChannels = getWord(mpp + 0x44);
    mhu->numpat = getWord(mpp + 0x46);
    mhu->numins = getWord(mpp + 0x48);
    mhu->flags = getWord(mpp + 0x4A);
    mhu->tempo = getWord(mpp + 0x4C);
    mhu->bpm = getWord(mpp + 0x4E);
    mhu->XMPSXChannels = mhu->XMChannels;
    if (mhu->XMPSXChannels > 24)
        mhu->XMPSXChannels = 24;

    for (t = 0; t < mhu->songlength; t++)
        mhu->jorders[t] = mpp[0x50 + t];

    a = 0x150;
    c = mhu->numpat;
    for (t = 0; t < c; t++) {
        mhu->JAP_PAT_ADDR[t] = (unsigned int *)(mpp + a);
        b = func_0006FD8C(mpp + a);
        mhu->JAP_PAT_ADDR2[t] = (unsigned int *)(mpp + a + b);
        b2 = mpp[a + 7] | (mpp[a + 8] << 8);
        b += b2;
        a += b;
    }

    c = mhu->numins;
    for (t = 0; t < c; t++) {
        b = func_0006FD8C(mpp + a);
        b2 = mpp[a + 27] | (mpp[a + 28] << 8);
        if (b2 != 0) {
            mhu->JAP_SampAddr[t] = (unsigned int *)(mpp + 29 + a);
            mhu->JAP_SampHdrAddr[t] = (unsigned int *)(mpp + b + a);
            b += 40 * b2;
        }
        mhu->JAP_InstrumentOffset[t] = (unsigned int *)(mpp + a);
        a += b;
    }
    return 0;
}
