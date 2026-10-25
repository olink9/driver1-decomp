#include "common.h"
typedef struct {
    volatile short hires;              /* 0x00 */
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
typedef struct { unsigned short left, right; } SNDVOL;
extern int D_8012675C, D_80126768;
extern SNDVOL D_8012676C;
extern void func_80101958(void), func_801014C0();
extern unsigned short func_80101D0C(unsigned short x);
extern u_long *func_80101D60(int n);
extern void func_80107158(u_long *buf, int n);
extern void DecDCTvlcSize(int), DecDCToutCallback(void (*)());
extern void SsGetSerialVol(char, SNDVOL *), SsSetSerialVol(short, short, short);

void func_801015CC(MOVIE *m)
{
    u_short x = m->x;
    u_short y = m->y;
    u_short w = m->w;
    u_short h = m->h;
    short dispw = m->dispw;
    int size;
    int n;

    D_801268A8.hires = m->hires;
    D_80126768 = D_8012675C;
    D_801268A8.dispw = dispw;
    D_801268A8.x = x;
    D_801268A8.y = y;
    D_801268A8.width = w;
    D_801268A8.height = h;
    D_801268A8.endFrame = m->endFrame;
    D_801268A8.vol = m->vol;
    D_801268A8.frameCount = 0;
    D_801268A8.error = 0;
    D_801268A8.isdone = 0;
    D_801268A8.rectid = 0;
    D_801268A8.vlcid = 0;
    D_801268A8.imgid = 0;
    SetDefDispEnv(&D_801268A8.disp[0], 0, 240, dispw, 240);
    SetDefDispEnv(&D_801268A8.disp[1], 0, 0, dispw, 240);
    setRECT(&D_801268A8.disp[0].screen, 0, 0, 0, 240);
    setRECT(&D_801268A8.disp[1].screen, 0, 0, 0, 240);
    D_801268A8.disp[0].isrgb24 = D_801268A8.disp[1].isrgb24 = (m->hires == 1);
    if (m->clear || D_801268A8.hires)
        func_80101958();
    VSync(0);
    PutDispEnv(&D_801268A8.disp[D_801268A8.rectid]);
    setRECT(&D_801268A8.rect[0], func_80101D0C(x), y, func_80101D0C(w), h);
    setRECT(&D_801268A8.rect[1], func_80101D0C(x), y + 240, func_80101D0C(w), h);
    D_801268A8.slice = D_801268A8.rect[D_801268A8.rectid];
    D_801268A8.slice.w = func_80101D0C(16);
    size = 0x20400;
    if (m->vlcsize)
        size = m->vlcsize;
    D_801268A8.vlcbuf[0] = func_80101D60(size);
    D_801268A8.vlcbuf[1] = func_80101D60(size);
    if (D_801268A8.hires)
        n = (short)h * 48;
    else
        n = (short)h * 32;
    D_801268A8.imgbuf[0] = func_80101D60(n);
    D_801268A8.imgbuf[1] = func_80101D60(n);
    DecDCTReset(0);
    DecDCTvlcSize(0);
    DecDCToutCallback(func_801014C0);
    func_80107158(func_80101D60(0x20000), 64);
    StSetStream(D_801268A8.hires, 0, -1, 0, 0);
    SsGetSerialVol(0, &D_8012676C);
    SsSetSerialVol(0, D_801268A8.vol, D_801268A8.vol);
}
