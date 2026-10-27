/* Driver 1 PC port - NFMV.EXE (the FMV player) in-process.
 *
 * On the PS1, fmvplay.c PlayRender() loads \DRIVER\NFMV\NFMV.EXE at 0x800FF800 and BIOS-Exec()s it with argc 1 and
 * the movie number as argv; NFMV's main() plays \DRIVER\NFMV\<n/10>\RENDER<n>.STR and returns to the game, which
 * then re-initialises its libraries (ReInitSystem). Here NFMV's C (src/nfmv: main.c nfmv_main, movie.c, pad.c) is
 * linked into the exe and PC_FmvExec() (called by the Exec stub in pc_psyq.c) runs it in-process, as REDRIVER2
 * does for Driver 2 (FMV_main). This file supplies what NFMV.EXE carried itself:
 *   - its data (.data/.sdata/.bss: the movie record, decoder state, pad buffers, the FMV end-frame table), reset
 *     to the EXE's initial values on every Exec as a fresh load would; its heap (0x80160000 on the PS1) is a
 *     static buffer;
 *   - the libcd streaming library (StSetRing/StSetStream/StGetNext/StFreeRing/StUnSetRing, CdRead2), served from
 *     the raw 2352-byte .STR sectors in pcdata/ (pc_xa.c reader) at the drive's speed (2x = 150 sectors/s on the
 *     vblank clock): video sectors are assembled into frames (20h-byte STR headers, psx-spx "CDROM File Video
 *     Streaming STR"), the interleaved XA-ADPCM audio sectors go to pc_xa.c's decoder as the PS1 drive feeds them
 *     to the SPU;
 *   - libsnd/libspu calls NFMV makes that PsyCross lacks (stubs: the movie audio is the CD input, volume through
 *     SsSetSerialVol = pc_psyq.c);
 *   - the display: the PS1 shows the 24-bit frame straight from VRAM (DISPENV isrgb24). PsyCross only displays
 *     what is drawn, so PC_FmvPresent() reads the displayed VRAM area back, converts it as the 24-bit (or 15-bit)
 *     display would, and draws it as one full-screen RGBA texture quad (PsyCross DR_PSYX_TEX).
 * The MDEC is port/pc/pc_mdec.c.
 *
 * D1_FMV=0 skips every movie (the old behaviour), D1_FMV=1 plays them. Default: play, except in scripted runs
 * (D1_PAD set), whose vblank-timed scripts were written without the movies. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "common.h"
#include "nfmv.h"
#include <SDL.h>

extern int __real_VSync(int mode);                 /* PsyCross: -1 = vblank counter */
extern int PC_XaReadRaw(int lba, unsigned char *dst);
extern void PC_XaStrReset(void);
extern void PC_XaStrAudio(const unsigned char *raw, int lead_ms);
extern void PC_XaStrStop(void);
extern int PC_XaFileEnd(int lba);
extern void PC_DctService(void);
extern void PC_PadRead(unsigned char *pad);
extern int CdPosToInt(CdlLOC *p);
extern int nfmv_main(int argc, int movie);         /* src/nfmv/main.c */

/* ---------------- NFMV.EXE data (addresses = the PS1 ones, for reference) ---------------- */
typedef struct { unsigned short left, right; } SNDVOL;

DB D_8012DB48[2];                                  /* 8012DB48 draw/display double buffer */
short D_80126750;                                  /* 80126750 current buffer */
volatile int D_80126754;                           /* 80126754 vblank counter (VSyncCallback) */
int D_80126758;                                    /* 80126758 movie number */
int D_8012675C;                                    /* 8012675C heap pointer (0x80160000) */
int D_80126768;                                    /* 80126768 heap mark */
SNDVOL D_8012676C;                                 /* 8012676C saved CD volume */
int D_80126770;                                    /* 80126770 debug font stream */
int D_80126778[76];                                /* 80126778 last frame of each RENDER<n>.STR */
MOVIE D_80112868;                                  /* 80112868 the movie record */
DECENV D_801268A8;                                 /* 801268A8 decoder state */
long long D_8012DAA0;                              /* 8012DAA0 "CD interrupt pending" (24-bit mode) */
volatile short D_80126760;                         /* 80126760 pad connected flags */
volatile short D_80126762;                         /* 80126762 pad to read */
unsigned char D_80126920[0x44] __attribute__((aligned(4)));   /* 80126920 PADBUF[2] (0x22 each) */
__asm__(".globl D_80126942\n.set D_80126942, D_80126920 + 0x22");

/* the big buffers are malloc'ed (once): the exe's .bss must stay below 16 MB (24-bit OT links, see pc_main.c), and
 * none of these is ever linked into an OT */
#define HEAP_SIZE 0x80000
static unsigned char *s_heap;
static unsigned short *s_shadow;                   /* 1024x512: what the movie wrote to VRAM (PC_FmvPresent) */
static void alloc_buffers(void);

/* movie.c func_801014C0: every LoadImage of a decoded slice is mirrored here; the VRAM clears at the start of a
 * movie (func_801011C4/func_80101958) are the memset in PC_FmvExec */
void PC_FmvShadowLoad(RECT16 *r, const void *p)
{
    const unsigned short *src = (const unsigned short *)p;
    int y;
    if (r->x < 0 || r->w <= 0 || r->x + r->w > 1024)
        return;
    for (y = 0; y < r->h; y++)
        memcpy(s_shadow + ((r->y + y) & 511) * 1024 + r->x, src + y * r->w, r->w * 2);
}

/* NFMV.EXE as loaded: .data 0x80112868 = { name 0x80100000, hires 0, clear 1, dispw 320, x 0, y 32, w 320, h 176,
 * endFrame 0x2742, vlcsize 0, vol 127 }; the rest of what main() reads is .bss (0) */
static void reset_image(void)
{
    static const MOVIE movie0 = { 0, 0, 1, 320, 0, 32, 320, 176, 0x2742, 0, 127 };
    memset(D_8012DB48, 0, sizeof(D_8012DB48));
    D_80126750 = 0;
    D_80126754 = 0;
    D_80126758 = 0;
    D_8012675C = (int)s_heap;                       /* PC_FmvExec allocated it */
    D_80126768 = 0;
    memset(&D_8012676C, 0, sizeof(D_8012676C));
    D_80126770 = 0;
    memset(D_80126778, 0, sizeof(D_80126778));
    D_80112868 = movie0;
    memset(&D_801268A8, 0, sizeof(D_801268A8));
    D_8012DAA0 = 0;
    D_80126760 = 0;
    D_80126762 = 0;
    memset(D_80126920, 0xFF, sizeof(D_80126920));
}

/* ---------------- Exec entry ---------------- */
static int fmv_enabled(void)
{
    const char *e = getenv("D1_FMV");
    if (e && *e)
        return atoi(e) != 0;
    return getenv("D1_PAD") == NULL;
}

static int s_st_frames, s_st_audio;            /* per-movie statistics (log) */
static unsigned int s_st_last;

static int play_one(int argc, int movie)
{
    int r, v0 = __real_VSync(-1);
    printf("[fmv] movie %d: \\DRIVER\\NFMV\\%d\\RENDER%d.STR (vblank %d)\n", movie, movie / 10,
           movie == 70 || movie == 71 ? 73 : movie, v0);
    fflush(stdout);
    reset_image();
    memset(s_shadow, 0, 1024 * 512 * 2);
    s_st_frames = s_st_audio = 0;
    s_st_last = 0;
    r = nfmv_main(argc, movie);
    /* NFMV ends with SsEnd/ResetGraph; the game's ReInitSystem re-inits sound from scratch (InitSound), as after
     * the SpuQuit PlayRender did before the Exec */
    SpuQuit();
    printf("[fmv] movie %d done (vblank %d, %d vblanks): %d frames shown (last STR frame %u, end frame %d), "
           "%d audio sectors, %s\n", movie, __real_VSync(-1), __real_VSync(-1) - v0, s_st_frames, s_st_last,
           (int)D_80112868.endFrame, s_st_audio,
           s_st_last > D_80112868.endFrame ? "ended at its end frame" : "stopped early (skipped, or the file ended)");
    fflush(stdout);
    return r;
}

/* D1_FMV_LIST=n1,n2,...: test aid - the first movie the game asks for is replaced by this list, played in order */
int PC_FmvExec(int argc, int movie)
{
    static int list_done;
    const char *list = getenv("D1_FMV_LIST");
    int r = 1;
    if (!fmv_enabled()) {
        printf("[fmv] movie %d skipped (D1_FMV=0, or a D1_PAD run without D1_FMV=1)\n", movie);
        return 1;
    }
    alloc_buffers();
    if (list && *list && !list_done++) {
        while (*list) {
            r = play_one(argc, atoi(list));
            list = strchr(list, ',');
            if (!list)
                break;
            list++;
        }
        return r;
    }
    return play_one(argc, movie);
}

/* VSync(n), n > 1, as PsyQ: wait n vblanks (PsyCross's VSync returns at once for n > 0) */
void PC_FmvWaitVbl(int n)
{
    int end = __real_VSync(-1) + n;
    while (__real_VSync(-1) < end)
        VSync(0);
}

/* ---------------- libcd streaming ---------------- */
#define FRAME_SLOTS 8
#define FRAME_MAX (64 * 2016)
#define AUDIO_LEAD 24                   /* sectors the audio is read ahead of the video (and the AL lead-in) */

typedef struct {
    int state;                          /* 0 free, 1 filling, 2 ready, 3 handed out */
    unsigned int frame;
    int got, need;
    unsigned int hdr[8];                /* the frame's first STR header (StHEADER) */
    unsigned char *data;                /* FRAME_MAX bytes */
} Frame;
static Frame s_fr[FRAME_SLOTS];
static unsigned int s_seq;              /* ready order */
static unsigned int s_fr_seq[FRAME_SLOTS];

static int s_loc = -1;                  /* CdlSeekL target (movie.c PC hook) */
static int s_run, s_lba, s_end, s_alba, s_rate, s_last, s_audio_started;
static int s_credit;                    /* sectors due, in 1/60 sector units */
static unsigned char s_sec[2352];

void PC_FmvSetLoc(CdlLOC *loc)
{
    s_loc = CdPosToInt(loc);
}

static Frame *slot_for(unsigned int frame, int need)
{
    int i;
    for (i = 0; i < FRAME_SLOTS; i++)
        if (s_fr[i].state == 1 && s_fr[i].frame == frame)
            return &s_fr[i];
    for (i = 0; i < FRAME_SLOTS; i++)
        if (s_fr[i].state == 1) {
            /* a frame that never completed (sector missing): drop it */
            s_fr[i].state = 0;
        }
    for (i = 0; i < FRAME_SLOTS; i++)
        if (s_fr[i].state == 0) {
            s_fr[i].state = 1;
            s_fr[i].frame = frame;
            s_fr[i].got = 0;
            s_fr[i].need = need;
            return &s_fr[i];
        }
    return NULL;
}

static int free_slots(void)
{
    int i, n = 0;
    for (i = 0; i < FRAME_SLOTS; i++)
        n += s_fr[i].state == 0;
    return n;
}

/* one sector at the video cursor; 0 = no room for it (the ring is full: the drive waits) */
static int video_sector(void)
{
    const unsigned char *h = s_sec + 24;
    unsigned short magic = h[0] | h[1] << 8, type = h[2] | h[3] << 8;
    unsigned short sn = h[4] | h[5] << 8, ns = h[6] | h[7] << 8;
    unsigned int frame = h[8] | h[9] << 8 | h[10] << 16 | (unsigned)h[11] << 24;
    Frame *f;
    if ((s_sec[18] & 0x44) == 0x44)     /* audio: the audio cursor handled it */
        return 1;
    if (magic != 0x0160 || type != 0x8001 || ns == 0 || ns > 64 || sn >= ns)
        return 1;                       /* not a video sector */
    if (sn == 0 && free_slots() == 0)
        return 0;
    f = slot_for(frame, ns);
    if (!f)
        return 0;
    if (sn == 0)
        memcpy(f->hdr, h, 32);
    memcpy(f->data + sn * 2016, s_sec + 24 + 32, 2016);
    if (++f->got == f->need) {
        f->state = 2;
        s_fr_seq[f - s_fr] = s_seq++;
    }
    return 1;
}

static void stream_service(void)
{
    int now;
    if (!s_run)
        return;
    now = __real_VSync(-1);
    s_credit += (now - s_last) * s_rate;
    s_last = now;
    if (s_credit > 60 * 64)
        s_credit = 60 * 64;
    while (s_credit >= 60 && s_lba < s_end) {
        /* the audio cursor stays AUDIO_LEAD sectors ahead */
        while (s_alba < s_end && s_alba < s_lba + AUDIO_LEAD) {
            if (PC_XaReadRaw(s_alba, s_sec) && (s_sec[18] & 0x44) == 0x44) {
                PC_XaStrAudio(s_sec, s_audio_started ? 0 : AUDIO_LEAD * 1000 / 150);
                s_audio_started = 1;
                s_st_audio++;
            }
            s_alba++;
        }
        if (!PC_XaReadRaw(s_lba, s_sec)) {
            s_lba = s_end;
            break;
        }
        if (!video_sector()) {
            s_credit = 60;              /* the ring is full: the drive waits (no burst afterwards) */
            break;
        }
        s_lba++;
        s_credit -= 60;
    }
}

/* CdRead2(mode): start streaming at the CdlSeekL position (movie.c func_801019C8) */
int CdRead2(int mode)
{
    int i;
    if (s_loc < 0)
        return 0;
    for (i = 0; i < FRAME_SLOTS; i++)
        s_fr[i].state = 0;
    s_lba = s_alba = s_loc;
    s_end = PC_XaFileEnd(s_loc);        /* the drive reads on; the file's end is where the movie data stops */
    s_rate = (mode & CdlModeSpeed) ? 150 : 75;
    s_last = __real_VSync(-1);
    s_credit = 0;
    s_audio_started = 0;
    PC_XaStrReset();
    s_run = 1;
    return 1;
}

void func_80107158(u_long *ring, int size) /* StSetRing(ring_addr, ring_size): the ring is s_fr here */
{
    (void)ring;
    (void)size;
}

void StSetStream(u_int mode, u_int start_frame, u_int end_frame, void (*func1)(), void (*func2)())
{
}

void StCdInterrupt(void)
{
}

/* StGetNext(&addr, &header): 0 and the oldest complete frame, or 1 when none is there yet. PC: waits (up to 1 s
 * real time) for the drive to deliver one - the PS1 caller spins on it anyway (func_80101A7C) - unless the stream
 * has ended. */
u_int StGetNext(u_int **addr, u_int **header)
{
    int tries = 0;
    for (;;) {
        int i, best = -1;
        stream_service();
        for (i = 0; i < FRAME_SLOTS; i++)
            if (s_fr[i].state == 2 && (best < 0 || (int)(s_fr_seq[i] - s_fr_seq[best]) < 0))
                best = i;
        if (best >= 0) {
            s_fr[best].state = 3;
            s_st_frames++;
            s_st_last = s_fr[best].hdr[2];
            *addr = (u_int *)s_fr[best].data;
            *header = (u_int *)s_fr[best].hdr;
            return 0;
        }
        if (!s_run || s_lba >= s_end || ++tries > 1000)
            return 1;
        usleep(1000);
    }
}

u_int StFreeRing(u_int *base)
{
    int i;
    for (i = 0; i < FRAME_SLOTS; i++)
        if (s_fr[i].state == 3 && (u_int *)s_fr[i].data == base) {
            s_fr[i].state = 0;
            return 0;
        }
    return 1;
}

void StUnSetRing(void)
{
    s_run = 0;
    s_loc = -1;
    PC_XaStrStop();                     /* the drive pauses next (CdlPause): the CD audio stops with it */
}

void StClearRing(void)
{
}

/* ---------------- libsnd / libspu / libcd odds NFMV calls ---------------- */
int CdMix(CdlATV *vol) { return 1; }
void func_8010C8F8(void) { SpuInit(); }            /* SpuInit (_SpuInit(0)) */
void func_801109D8(void) { }                        /* SsUtReverbOff (SpuSetReverb(0)) */
void func_801109F8(void) { }                        /* SsUtReverbOn (SpuSetReverb(1)) */
unsigned int SpuWrite0(unsigned int size) { return size; }   /* zero-fills SPU RAM: nothing to clear here */
void SsInit(void) { }
void SsEnd(void) { }
void SsUtSetReverbType(short type) { }
void SsUtSetReverbDepth(short l, short r) { }
void SsSetTickMode(long mode) { }
void SsSetMVol(short l, short r) { }
void SsSetSerialAttr(char s_num, char attr, char mode) { }
void SsGetSerialVol(char s_num, SNDVOL *v) { v->left = v->right = 127; }
void ChangeClearPad(int val) { }

/* ---------------- display ---------------- */
#define TEX_W 512
#define TEX_H 256
static unsigned int s_tex;
static unsigned char *s_rgba;                      /* TEX_W x TEX_H RGBA */
static unsigned short *s_vram;                     /* the displayed area, 1024x512 at most */
static DR_PSYX_TEX s_texprim, s_texoff;
static POLY_FT4 s_quad;
extern unsigned int GR_CreateRGBATexture(int width, int height, unsigned char *data);
extern unsigned int g_lastBoundTexture;             /* PsyX_render.cpp: its bind cache */

/* Draws the VRAM area `d` displays as the PS1 display hardware would show it (24-bit: 3 bytes per pixel). The
 * texture holds the picture at (1,1) and the quad's texcoords are given at half scale (PsyCross's RGBA shader
 * samples at (uv + 0.5) / size, see the declared size below), which puts every screen pixel on its texel centre. */
void PC_FmvPresent(DISPENV *d)
{
    typedef void (*BindTex)(unsigned, unsigned);
    typedef void (*TexSub)(unsigned, int, int, int, int, int, unsigned, unsigned, const void *);
    static BindTex bind;
    static TexSub sub;
    RECT16 r = d->disp;
    int w = d->disp.w, h = d->disp.h, x, y;
    if (w <= 0 || h <= 0 || w > TEX_W - 2 || h > TEX_H - 2)
        return;
    if (!s_tex) {
        s_tex = GR_CreateRGBATexture(TEX_W, TEX_H, NULL);
        bind = (BindTex)SDL_GL_GetProcAddress("glBindTexture");
        sub = (TexSub)SDL_GL_GetProcAddress("glTexSubImage2D");
        memset(s_rgba, 0, TEX_W * TEX_H * 4);
    }
    if (d->isrgb24)
        r.w = (w * 3 + 1) / 2;
    /* the picture comes from the shadow VRAM (see PC_FmvShadowLoad), not from PsyCross's: PsyCross copies every
     * presented window back into the displayed VRAM area as 15-bit pixels (GR_StoreFrameBuffer), which garbles a
     * 24-bit picture's borders (the movie never redraws them) */
    for (y = 0; y < r.h; y++)
        memcpy(s_vram + y * r.w, s_shadow + ((r.y + y) & 511) * 1024 + r.x, r.w * 2);
    for (y = 0; y < h; y++) {
        unsigned char *o = s_rgba + ((y + 1) * TEX_W + 1) * 4;
        if (d->isrgb24) {
            const unsigned char *s = (const unsigned char *)(s_vram + y * r.w);
            for (x = 0; x < w; x++, o += 4, s += 3) {
                o[0] = s[0];
                o[1] = s[1];
                o[2] = s[2];
                o[3] = 255;
            }
        } else {
            const unsigned short *s = s_vram + y * r.w;
            for (x = 0; x < w; x++, o += 4) {
                unsigned short c = s[x];
                o[0] = (c & 31) << 3 | (c & 31) >> 2;
                o[1] = (c >> 5 & 31) << 3 | (c >> 5 & 31) >> 2;
                o[2] = (c >> 10 & 31) << 3 | (c >> 10 & 31) >> 2;
                o[3] = 255;
            }
        }
    }
    {   /* D1_FMV_DUMP=1 (test aid): the shown picture of frames 30 and endFrame/2 -> fmv_<movie>_<frame>.ppm */
        static const char *dump;
        if (!dump)
            dump = getenv("D1_FMV_DUMP") ? getenv("D1_FMV_DUMP") : "";
        if (*dump == '1' && (s_st_frames == 30 || s_st_frames == (int)D_80112868.endFrame / 2)) {
            char name[64];
            FILE *f;
            snprintf(name, sizeof(name), "fmv_%d_%d.ppm", D_80126758, s_st_frames);
            f = fopen(name, "wb");
            if (f) {
                fprintf(f, "P6 %d %d 255\n", w, h);
                for (y = 0; y < h; y++)
                    for (x = 0; x < w; x++)
                        fwrite(s_rgba + ((y + 1) * TEX_W + 1 + x) * 4, 1, 3, f);
                fclose(f);
            }
        }
    }
    bind(0x0DE1 /* GL_TEXTURE_2D */, s_tex);
    sub(0x0DE1, 0, 0, 0, TEX_W, TEX_H, 0x1908 /* GL_RGBA */, 0x1401 /* GL_UNSIGNED_BYTE */, s_rgba);
    bind(0x0DE1, 0);
    g_lastBoundTexture = (unsigned int)-1;          /* make PsyCross bind its own texture again */

    {   /* draw environment = the displayed area, so the quad covers the screen */
        DRAWENV de;
        SetDefDrawEnv(&de, d->disp.x, d->disp.y, w, h);
        de.isbg = 0;
        de.dtd = 0;
        PutDrawEnv(&de);
    }
    /* declared size = half the real one: u 0..w/2 spans w texels, starting on texel 1's centre */
    SetPsyXTexture(&s_texprim, s_tex, TEX_W / 2, TEX_H / 2);
    setPolyFT4(&s_quad);
    setRGB0(&s_quad, 128, 128, 128);
    setXYWH(&s_quad, 0, 0, w, h);
    setUVWH(&s_quad, 0, 0, w / 2, h / 2);
    s_quad.tpage = 0;
    s_quad.clut = 0;
    SetPsyXTexture(&s_texoff, 0, 0, 0);    /* the override stays on until reset: later prims use VRAM again */
    setaddr(&s_texprim, &s_quad);
    setaddr(&s_quad, &s_texoff);
    termPrim(&s_texoff);
    DrawOTag((u_long *)&s_texprim);
}

static void alloc_buffers(void)
{
    int i;
    if (s_heap)
        return;
    s_heap = malloc(HEAP_SIZE);
    s_shadow = malloc(1024 * 512 * 2);
    s_vram = malloc(1024 * 512 * 2);
    s_rgba = calloc(TEX_W * TEX_H, 4);
    for (i = 0; i < FRAME_SLOTS; i++)
        s_fr[i].data = malloc(FRAME_MAX);
    if (!s_heap || !s_shadow || !s_vram || !s_rgba || !s_fr[FRAME_SLOTS - 1].data) {
        fprintf(stderr, "[fmv] out of memory\n");
        abort();
    }
}
