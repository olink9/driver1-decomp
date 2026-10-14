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
extern XMCHANNEL *D_0009781C;
extern int D_00097820;
extern int D_00097824;
extern void SpuSetKey();
extern void SpuFlush();
extern void func_00072740();
extern int func_00072B8C();
extern int func_00072BC0();
extern void func_00073104();
extern void func_000732D8();
extern void func_00073304();

void func_0007282C(void)
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
            D_0009781C = &D_00097814->XM_Chnl[t];
            MaxChans--;
            if (D_0009781C->kick) {
                Ch2 = D_0009781C->SPUChannel;
                prd = D_0009781C->period;
                if (D_00097814->NotAmiga == 0)
                    prd = (0xA3BA68 / prd) >> 3;
                else
                    prd = func_00072BC0(prd);
                SpuSetKey(0, 1 << Ch2);
                D_0009781C->OldLVol = D_0009781C->LVol;
                D_0009781C->OldRVol = D_0009781C->RVol;
                func_00072740();
                func_00073104(D_00097814->VabID, Ch2, D_0009781C->sample, prd, D_00097824, D_00097820);
                SPUKeyOn |= (1 << Ch2);
                D_0009781C->SPUPitch = prd;
                D_0009781C->OldPeriod = D_0009781C->period;
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
            D_0009781C = &D_00097814->XM_Chnl[t];
            MaxChans--;
            if (!D_0009781C->ChDead) {
                Ch2 = D_0009781C->SPUChannel;
                if (D_0009781C->kick == 0) {
                    if ((D_0009781C->LVol != D_0009781C->OldLVol) || (D_0009781C->RVol != D_0009781C->OldRVol)) {
                        if (D_0009781C->OldLVol != D_0009781C->LVol)
                            D_0009781C->OldLVol = func_00072B8C(D_0009781C->LVol, D_0009781C->OldLVol);
                        if (D_0009781C->OldRVol != D_0009781C->RVol)
                            D_0009781C->OldRVol = func_00072B8C(D_0009781C->RVol, D_0009781C->OldRVol);
                        func_00072740();
                        func_000732D8(Ch2, D_00097824, D_00097820);
                    }
                    if (D_0009781C->period != D_0009781C->OldPeriod) {
                        D_0009781C->OldPeriod = D_0009781C->period;
                        prd = D_0009781C->period;
                        if (D_00097814->NotAmiga == 0)
                            prd = (0xA3BA68 / prd) >> 3;
                        else
                            prd = func_00072BC0(prd);
                        D_0009781C->SPUPitch = prd;
                        func_00073304(Ch2, prd);
                    }
                } else
                    D_0009781C->kick = 0;
            }
        }
    }
}
