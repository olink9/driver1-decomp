/* DoDolbySS */
typedef struct {
    char pad0[6];
    short OldLVol;
    short OldRVol;
    char pad1[0x6D];
    unsigned char Dolby;
} XMCH;
extern unsigned char D_00096FA8;
extern XMCH *XMC;
extern int D_00097820;
extern int D_00097824;

void DoDolbySS(void)
{
    if (D_00096FA8 == 0) {
        D_00097824 = (XMC->OldLVol + XMC->OldRVol) / 2;
        D_00097820 = D_00097824;
        return;
    }
    if (XMC->Dolby == 1) {
        D_00097824 = XMC->OldLVol;
        if (XMC->OldRVol > XMC->OldLVol) {
            D_00097820 = -XMC->OldRVol;
            D_00097824 = XMC->OldRVol;
        } else
            D_00097820 = -XMC->OldRVol;
    } else if (XMC->Dolby == 2) {
        D_00097820 = XMC->OldLVol;
        if (XMC->OldRVol > XMC->OldLVol) {
            D_00097824 = -XMC->OldRVol;
            D_00097820 = XMC->OldRVol;
        } else
            D_00097824 = -XMC->OldRVol;
    } else {
        D_00097824 = XMC->OldLVol;
        D_00097820 = XMC->OldRVol;
    }
}
