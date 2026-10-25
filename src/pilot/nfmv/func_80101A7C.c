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
    char pad38[4];
    volatile u_long *vlcbuf[2];        /* 0x3C */
} DECENV;
extern DECENV D_801268A8;

u_long *func_80101A7C(void)
{
    u_long *addr;
    StHEADER *hdr;
    DECENV *d;
    int cnt = 0x800000;

    while (StGetNext((u_int **)&addr, (u_int **)&hdr)) {
        if (--cnt == 0)
            return 0;
    }
    d = &D_801268A8;
    d->frameCount++;
    if (hdr->frameCount > d->endFrame || hdr->frameCount < d->frameCount ||
        hdr->width != d->width || hdr->height != d->height) {
        d->error = 1;
        return addr;
    }
    return addr;
}
