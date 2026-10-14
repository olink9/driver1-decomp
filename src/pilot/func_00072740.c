/* DoDolbySS */
typedef struct {
    char pad0[6];
    short OldLVol;
    short OldRVol;
    char pad1[0x6D];
    unsigned char Dolby;
} XMCH;
extern unsigned char D_00096FA8;
extern XMCH *D_0009781C;
extern int D_00097820;
extern int D_00097824;

void func_00072740(void)
{
    if (D_00096FA8 == 0) {
        D_00097824 = (D_0009781C->OldLVol + D_0009781C->OldRVol) / 2;
        D_00097820 = D_00097824;
        return;
    }
    if (D_0009781C->Dolby == 1) {
        D_00097824 = D_0009781C->OldLVol;
        if (D_0009781C->OldRVol > D_0009781C->OldLVol) {
            D_00097820 = -D_0009781C->OldRVol;
            D_00097824 = D_0009781C->OldRVol;
        } else
            D_00097820 = -D_0009781C->OldRVol;
    } else if (D_0009781C->Dolby == 2) {
        D_00097820 = D_0009781C->OldLVol;
        if (D_0009781C->OldRVol > D_0009781C->OldLVol) {
            D_00097824 = -D_0009781C->OldRVol;
            D_00097820 = D_0009781C->OldRVol;
        } else
            D_00097824 = -D_0009781C->OldRVol;
    } else {
        D_00097824 = D_0009781C->OldLVol;
        D_00097820 = D_0009781C->OldRVol;
    }
}
