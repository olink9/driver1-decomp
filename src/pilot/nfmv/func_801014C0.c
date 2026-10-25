#include "common.h"
typedef struct {
    volatile unsigned short hires;     /* 0x00 */
    volatile unsigned short width0;    /* 0x02 */
    char pad04[4];
    volatile unsigned short width;     /* 0x08 */
    volatile unsigned short height;    /* 0x0A */
    volatile unsigned int endFrame;    /* 0x0C */
    char pad10[4];
    volatile unsigned int frameCount;  /* 0x14 */
    RECT16 rect[2];             /* 0x18 */
    RECT16 slice;               /* 0x28 */
    volatile short error;              /* 0x30 */
    volatile short isdone;             /* 0x32 */
    volatile short rectid;             /* 0x34 */
    volatile short vlcid;              /* 0x36 */
    volatile short imgid;              /* 0x38 */
    char pad3A[2];
    volatile u_long *vlcbuf[2];        /* 0x3C */
    volatile u_long *imgbuf[2];        /* 0x44 */
} DECENV;
extern DECENV D_801268A8;
extern long long D_8012DAA0;
extern void StCdInterrupt(void);
extern int func_80101CA0(void);

void func_801014C0(void)
{
    if (D_801268A8.hires) {
        if (*(int *)&D_8012DAA0) {
            StCdInterrupt();
            *(int *)&D_8012DAA0 = 0;
        }
    }
    LoadImage(&D_801268A8.slice, (u_long *)D_801268A8.imgbuf[D_801268A8.imgid]);
    D_801268A8.imgid ^= 1;
    D_801268A8.slice.x += D_801268A8.slice.w;
    if (D_801268A8.slice.x < D_801268A8.rect[D_801268A8.rectid].x + D_801268A8.rect[D_801268A8.rectid].w)
        DecDCTout((u_long *)D_801268A8.imgbuf[D_801268A8.imgid], func_80101CA0());
    else
        D_801268A8.isdone = 1;
}
