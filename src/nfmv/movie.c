/* movie.c: 14 functions func_8010127C..func_80101D60 (0x8010127C-0x80101D88)
 * Step 3: types from include/nfmv.h (DECENV D_801268A8 for every function, MOVIE). func_80101D0C/D40 read hires
 * non-volatile through `*(unsigned short *)&`;
 * func_801015CC passes it to StSetStream as a signed short (`*(volatile short *)&`). func_801019C8 returns short (its caller func_8010127C was compiled
 * against short; its own bytes are the same). No views.
 */

#include "common.h"

#include "nfmv.h"

/* ---- types ---- */
typedef struct { unsigned short left, right; } SNDVOL;

/* ---- externs ---- */
extern DECENV D_801268A8;
extern void DecDCTin(u_long *buf, unsigned short mode);
extern long long D_8012DAA0;
extern void StCdInterrupt(void);
extern int D_8012675C;
extern int D_80126768;
extern SNDVOL D_8012676C;
extern void func_80107158(u_long *buf, int n);
extern void DecDCTvlcSize(int);
extern void DecDCToutCallback(void(*)());
extern void SsGetSerialVol(char, SNDVOL *);
extern void SsSetSerialVol(short, short, short);
extern int DecDCTvlc(u_long *bs, u_long *buf);

/* ---- functions defined in this file ---- */
short func_8010127C();
void func_801014C0();
void func_801015CC();
void func_80101904();
void func_80101958();
short func_801019C8();
u_long *func_80101A7C();
void func_80101B48();
void func_80101BB0();
int func_80101C3C();
int func_80101CA0();
unsigned short func_80101D0C(unsigned short x);
int func_80101D40();
int func_80101D60();

/* ---------------- func_8010127C (src/pilot/nfmv/func_8010127C.c) ---------------- */
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

/* ---------------- func_801014C0 (src/pilot/nfmv/func_801014C0.c) ---------------- */
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

/* ---------------- func_801015CC (src/pilot/nfmv/func_801015CC.c) ---------------- */
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
    StSetStream(*(volatile short *)&D_801268A8.hires, 0, -1, 0, 0);
    SsGetSerialVol(0, &D_8012676C);
    SsSetSerialVol(0, D_801268A8.vol, D_801268A8.vol);
}

/* ---------------- func_80101904 (src/pilot/nfmv/func_80101904.c) ---------------- */
void func_80101904(void)
{
    StUnSetRing();
    CdControlB(CdlPause, 0, 0);
    DecDCToutCallback(0);
    D_801268A8.hires;
    SsSetSerialVol(0, 0, 0);
}

/* ---------------- func_80101958 (src/pilot/nfmv/func_80101958.c) ---------------- */
void func_80101958(void)
{
    RECT16 r;
    r.x = 0;
    r.y = 0;
    r.w = func_80101D0C(D_801268A8.dispw);
    r.h = 240;
    ClearImage(&r, 0, 0, 0);
    r.y = 240;
    ClearImage(&r, 0, 0, 0);
}

/* ---------------- func_801019C8 (src/pilot/nfmv/func_801019C8.c) ---------------- */
short func_801019C8(CdlLOC *loc)
{
    short retry = 0;
    short n = 0;

    for (;;) {
        while (CdControl(CdlSeekL, (u_char *)loc, 0) == 0) {
            if (++retry == 5)
                return 0;
        }
        retry = 0;
        if (++n == 6)
            return 0;
        if (CdRead2(CdlModeStream | CdlModeSpeed | CdlModeRT))
            return 1;
    }
}

/* ---------------- func_80101A7C (src/pilot/nfmv/func_80101A7C.c) ---------------- */
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

/* ---------------- func_80101B48 (src/pilot/nfmv/func_80101B48.c) ---------------- */
void func_80101B48(u_long *addr)
{
    if (addr) {
        D_801268A8.vlcid ^= 1;
        DecDCTvlc(addr, D_801268A8.vlcbuf[D_801268A8.vlcid]);
        StFreeRing((u_int *)addr);
    }
}

/* ---------------- func_80101BB0 (src/pilot/nfmv/func_80101BB0.c) ---------------- */
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

/* ---------------- func_80101C3C (src/pilot/nfmv/func_80101C3C.c) ---------------- */
int func_80101C3C(short *vol)
{
    *vol -= 8;
    if (*vol < 0)
        *vol = 0;
    SsSetSerialVol(0, *vol, *vol);
    return (*vol == 0) * 2;
}

/* ---------------- func_80101CA0 (src/pilot/nfmv/func_80101CA0.c) ---------------- */
int func_80101CA0(void)
{
    unsigned short h = D_801268A8.height;
    if (D_801268A8.hires)
        return ((h - 1) / 16 + 1) * 192;
    return ((h - 1) / 16 + 1) * 128;
}

/* ---------------- func_80101D0C (src/pilot/nfmv/func_80101D0C.c) ---------------- */
unsigned short func_80101D0C(unsigned short x)
{
    if (*(unsigned short *)&D_801268A8.hires)
        return (x * 3) >> 1;
    return x;
}

/* ---------------- func_80101D40 (src/pilot/nfmv/func_80101D40.c) ---------------- */
int func_80101D40(void)
{
    int r = 2;
    if (*(unsigned short *)&D_801268A8.hires)
        r = 3;
    return r;
}

/* ---------------- func_80101D60 (src/pilot/nfmv/func_80101D60.c) ---------------- */
/* bump allocator: returns the current pointer, advances it by n rounded up to 4 */
int func_80101D60(int n)
{
    int p = D_8012675C;
    if (n & 3)
        n = (n + 4) & ~3;
    D_8012675C = p + n;
    return p;
}
