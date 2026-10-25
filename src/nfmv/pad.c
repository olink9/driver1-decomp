/* pad.c: 4 functions func_80101D88..func_80101F54 (0x80101D88-0x80101FA8)
 * Step 3: D_80126920 is the PsyQ pad buffer pair (volatile PADBUF[2]; D_80126942 = its [1], kept as its own
 * symbol); D_80126760/D_80126762 volatile short, func_80101F54 tests D_80126760 non-volatile through
 * `*(unsigned short *)&`. No views.
 */

#include "common.h"

/* ---- types ---- */
typedef struct { unsigned char status; unsigned char len; unsigned short buttons; char pad[30]; } PADBUF;

/* ---- externs ---- */
extern volatile PADBUF D_80126920[2];
extern void ChangeClearPad();
extern volatile short D_80126760;
extern volatile short D_80126762;
extern void StopPAD(void);
extern volatile unsigned char D_80126942[34];

/* ---- functions defined in this file ---- */
void func_80101D88();
void func_80101DD8();
void func_80101E00();
int func_80101F54(short mask);

/* ---------------- func_80101D88 (src/pilot/nfmv/func_80101D88.c) ---------------- */
void func_80101D88(void)
{
    InitPAD(D_80126920, 0x22, (unsigned char *)D_80126920 + 0x22, 0x22);
    StartPAD();
    ChangeClearPad(0);
    VSync(0);
    func_80101E00();
}

/* ---------------- func_80101DD8 (src/pilot/nfmv/func_80101DD8.c) ---------------- */
void func_80101DD8(void)
{
    StopPAD();
    D_80126760 = 0;
    D_80126762 = 0;
}

/* ---------------- func_80101E00 (src/pilot/nfmv/func_80101E00.c) ---------------- */
void func_80101E00(void)
{
    D_80126760 = 0;
    if (D_80126920[0].status != 0xFF && (D_80126920[0].len >> 4) == 4)
        D_80126760 |= 1;
    if (D_80126920[0].status != 0xFF && (D_80126920[0].len >> 4) == 7)
        D_80126760 |= 1;
    if (D_80126942[0] != 0xFF && (D_80126942[1] >> 4) == 4)
        D_80126760 |= 2;
    if (D_80126942[0] != 0xFF && (D_80126942[1] >> 4) == 7)
        D_80126760 |= 1;
    if (D_80126760 == 3 || D_80126760 == 1)
        D_80126762 = 0;
    else if (D_80126760 == 2)
        D_80126762 = 1;
}

/* ---------------- func_80101F54 (src/pilot/nfmv/func_80101F54.c) ---------------- */
/* Caller func_80100DF8 declares this as returning short (sll 16 on the result); defined int here. */
int func_80101F54(short mask)
{
    int r = 0;
    if (*(unsigned short *)&D_80126760)
        if ((D_80126920[D_80126762].buttons & mask) == 0) r = 1;
    return r;
}

