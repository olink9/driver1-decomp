#include "common.h"
typedef struct {
    volatile unsigned short hires;     /* 0x00 */
    volatile unsigned short dispw;     /* 0x02 */
    volatile unsigned short x;         /* 0x04 */
    volatile unsigned short y;         /* 0x06 */
    volatile unsigned short width;     /* 0x08 */
    volatile unsigned short height;    /* 0x0A */
    volatile unsigned int endFrame;    /* 0x0C */
    volatile short vol;                /* 0x10 */
    char pad12[2];
    volatile unsigned int frameCount;  /* 0x14 */
    RECT16 rect[2];                    /* 0x18 */
    RECT16 slice;                      /* 0x28 */
    volatile short error;              /* 0x30 */
    volatile short isdone;             /* 0x32 */
    volatile short rectid;             /* 0x34 */
    volatile short vlcid;              /* 0x36 */
    volatile short imgid;              /* 0x38 */
    char pad3A[2];
    volatile u_long *vlcbuf[2];        /* 0x3C */
    volatile u_long *imgbuf[2];        /* 0x44 */
    DISPENV disp[2];                   /* 0x4C */
} DECENV;
typedef struct {
    char *name;         /* 0x00 */
    short hires;        /* 0x04 */
    short clear;        /* 0x06 */
    u_short dispw;      /* 0x08 */
    u_short x, y;       /* 0x0A */
    u_short w, h;       /* 0x0E */
    int endFrame;       /* 0x14 */
    int vlcsize;        /* 0x18 */
    short vol;          /* 0x1C */
} MOVIE;
extern DECENV D_801268A8;
extern void func_801015CC(MOVIE *m), func_80101904(void), func_80101B48(u_long *addr), func_80101BB0(void);
extern short func_801019C8(CdlLOC *loc);
extern u_long *func_80101A7C(void);
extern int func_80101D40(void), func_80101CA0(void), func_80101C3C(short *vol);
extern void DecDCTin(u_long *buf, unsigned short mode);

short func_8010127C(MOVIE *m, short (*cb)())
{
    CdlFILE file;
    short vol;
    short retry = 0;
    short result = 0;
    u_long *addr;

    vol = 0;
    if (!CdSearchFile(&file, m->name))
        return 3;
    func_801015CC(m);
    if (!func_801019C8(&file.pos)) {
        func_80101904();
        return 3;
    }
    while ((addr = func_80101A7C()) == 0) {
        if (++retry == 5) {
            func_80101904();
            return 3;
        }
    }
    func_80101B48(addr);
    retry = 0;
    while (!D_801268A8.error && !result) {
        DecDCTin((u_long *)D_801268A8.vlcbuf[D_801268A8.vlcid], func_80101D40());
        DecDCTout((u_int *)D_801268A8.imgbuf[D_801268A8.imgid], func_80101CA0());
        if ((addr = func_80101A7C()) == 0) {
            if (++retry == 5)
                result = 3;
        } else
            retry = 0;
        func_80101B48(addr);
        if (!vol) {
            if (cb())
                vol = D_801268A8.vol;
        }
        if (vol)
            result = func_80101C3C(&vol);
        func_80101BB0();
        DrawSync(0);
        VSync(0);
        PutDispEnv(&D_801268A8.disp[D_801268A8.rectid]);
    }
    func_80101904();
    if (D_801268A8.error)
        result = 1;
    return result;
}
