/* UpdateEffs */
typedef struct { char pad[0x14]; unsigned short XMPSXChannels; } MH;
typedef struct { char pad[0x50]; int PlayMask; char pad2[0x58 - 0x54]; char chnl[0xBAC - 0x58]; unsigned char MaxChans; } MS;
typedef struct { char pad[0x18]; unsigned char nothing; } CH;
extern MS *D_00097814;
extern MH *D_00097818;
extern CH *XMC;
extern void JPlayEffects();
void UpdateEffs(void) {
    int t;
    int pmsk;
    unsigned char MaxChans;
    MaxChans = D_00097814->MaxChans;
    for (t = 0; t < D_00097818->XMPSXChannels; t++) {
        if (MaxChans == 0) return;
        XMC = (CH *)(D_00097814->chnl + t * 0x78);
        pmsk = D_00097814->PlayMask;
        pmsk &= (1 << t);
        if (pmsk != 0) {
            MaxChans--;
            if (!XMC->nothing) JPlayEffects();
        }
    }
}
