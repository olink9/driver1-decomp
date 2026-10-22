/* UpdateHardware */
typedef struct {
    char pad0[2];
    short LVol;          /* 0x02 */
    short RVol;          /* 0x04 */
    short OldLVol;       /* 0x06 */
    short OldRVol;       /* 0x08 */
    unsigned short OldPeriod; /* 0x0A */
    char pad1[0x19 - 0xC];
    unsigned char ChDead; /* 0x19 */
    char pad2[0x41 - 0x1A];
    unsigned char kick;  /* 0x41 */
    unsigned char sample; /* 0x42 */
    char pad3[0x50 - 0x43];
    unsigned short period; /* 0x50 */
    unsigned short SPUPitch; /* 0x52 */
    char pad4[0x76 - 0x54];
    unsigned char SPUChannel; /* 0x76 */
    char pad5[0x78 - 0x77];
} XMCHANNEL;
typedef struct { char pad[0x14]; unsigned short XMPSXChannels; } MH;
typedef struct {
    char pad[0x30];
    int NotAmiga;        /* 0x30 */
    char pad1[0x50 - 0x34];
    int PlayMask;        /* 0x50 */
    char pad2[0x58 - 0x54];
    XMCHANNEL XM_Chnl[24]; /* 0x58 */
    char pad2b[0x10];
    unsigned char VabID; /* 0xBA8 */
    char pad3[3];
    unsigned char MaxChans; /* 0xBAC */
} MS;
extern MS *D_00097814;
extern MH *D_00097818;
extern XMCHANNEL *XMC;
extern int D_00097820;
extern int D_00097824;
extern void SpuSetKey();
extern void SpuFlush();
extern void DoDolbySS();
extern int IntVols();
extern int GetFreq2();
extern void PlaySFX();
extern void SetVol();
extern void SetFrq();

void UpdateHardware(void)
{
    int t;
    int prd;
    int SPUKeyOn = 0;
    int pmsk;
    int Ch2;
    unsigned char MaxChans;

    MaxChans = D_00097814->MaxChans;
    for (t = 0; t < D_00097818->XMPSXChannels; t++) {
        if (MaxChans == 0)
            break;
        pmsk = D_00097814->PlayMask;
        pmsk &= (1 << t);
        if (pmsk != 0) {
            XMC = &D_00097814->XM_Chnl[t];
            MaxChans--;
            if (XMC->kick) {
                Ch2 = XMC->SPUChannel;
                prd = XMC->period;
                if (D_00097814->NotAmiga == 0)
                    prd = (0xA3BA68 / prd) >> 3;
                else
                    prd = GetFreq2(prd);
                SpuSetKey(0, 1 << Ch2);
                XMC->OldLVol = XMC->LVol;
                XMC->OldRVol = XMC->RVol;
                DoDolbySS();
                PlaySFX(D_00097814->VabID, Ch2, XMC->sample, prd, D_00097824, D_00097820);
                SPUKeyOn |= (1 << Ch2);
                XMC->SPUPitch = prd;
                XMC->OldPeriod = XMC->period;
            }
        }
    }
    if (SPUKeyOn != 0) {
        SpuSetKey(1, SPUKeyOn);
        SpuFlush(0);
    }

    MaxChans = D_00097814->MaxChans;
    for (t = 0; t < D_00097818->XMPSXChannels; t++) {
        if (MaxChans == 0)
            return;
        pmsk = D_00097814->PlayMask;
        pmsk &= (1 << t);
        if (pmsk != 0) {
            XMC = &D_00097814->XM_Chnl[t];
            MaxChans--;
            if (!XMC->ChDead) {
                Ch2 = XMC->SPUChannel;
                if (XMC->kick == 0) {
                    if ((XMC->LVol != XMC->OldLVol) || (XMC->RVol != XMC->OldRVol)) {
                        if (XMC->OldLVol != XMC->LVol)
                            XMC->OldLVol = IntVols(XMC->LVol, XMC->OldLVol);
                        if (XMC->OldRVol != XMC->RVol)
                            XMC->OldRVol = IntVols(XMC->RVol, XMC->OldRVol);
                        DoDolbySS();
                        SetVol(Ch2, D_00097824, D_00097820);
                    }
                    if (XMC->period != XMC->OldPeriod) {
                        XMC->OldPeriod = XMC->period;
                        prd = XMC->period;
                        if (D_00097814->NotAmiga == 0)
                            prd = (0xA3BA68 / prd) >> 3;
                        else
                            prd = GetFreq2(prd);
                        XMC->SPUPitch = prd;
                        SetFrq(Ch2, prd);
                    }
                } else
                    XMC->kick = 0;
            }
        }
    }
}
