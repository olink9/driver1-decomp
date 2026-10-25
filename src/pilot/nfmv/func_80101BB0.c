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

void func_80101BB0(void)
{
    int cnt = 0x800000;
    while (!D_801268A8.isdone) {
        if (--cnt == 0)
            break;
    }
    D_801268A8.isdone = 0;
    D_801268A8.rectid ^= 1;
    D_801268A8.slice.x = D_801268A8.rect[D_801268A8.rectid].x;
    D_801268A8.slice.y = D_801268A8.rect[D_801268A8.rectid].y;
}
