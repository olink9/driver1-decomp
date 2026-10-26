/* Driver 1 PC port - XA-ADPCM audio streamed from the disc (xaplay.c: the MV1-5/NMAJ/CUTS.XA music and speech).
 *
 * On the PS1 the game sets Setmode 0xE8 (double speed, XA-ADPCM on, 924h-byte sectors, XA filter), picks a
 * file/channel with CdlSetfilter and starts CdlReadS. The drive then reads every sector at 150 sectors/s:
 *   - audio sectors (submode Audio+RealTime) that match the filter go to the SPU's CD input (XA-ADPCM decoder);
 *   - every other sector is delivered to the CPU as INT1/CdlDataReady (psx-spx "Data/ADPCM Sector
 *     Filtering/Delivery": the first delivery attempt ignores file/channel). xaplay.c's cbready() reads the
 *     first 8 words of those (924h mode: header, subheader, then data) and stops the stream at the end marker
 *     (data 0x0160, channel in bits 10-14 of the next halfword) of the channel it is playing.
 * Here pc_cd.c hands RT-mode streams to this file. The stream is paced by the PsyCross vblank counter (the PS1
 * drive runs on its own clock too): 150 (2x) or 75 (1x) sectors per second. Matching audio sectors are decoded
 * (psx-spx "XA-ADPCM", 4- and 8-bit, mono/stereo, 37800/18900 Hz) and queued on a streaming OpenAL source
 * (PsyX_SPUAL_XAQueue, PsyCross owns the AL context); the others go to the game's ready callback.
 * Raw 2352-byte sectors come straight from the XA files in pcdata/ (DISC.LBA, secsize 2352). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "psx/libcd.h"

extern const char *PC_DataDir(void);                 /* pc_main.c */
extern int __real_VSync(int mode);                   /* PsyCross VSync: -1 = vblank count, no wait */
extern void PsyX_SPUAL_XAQueue(const short *pcm, int frames, int stereo, int rate);
extern void PsyX_SPUAL_XAStop(void);
extern void PsyX_SPUAL_XASetVolume(int left, int right);

typedef struct { int lba, nsect, secsize; char path[96]; } XaFile;
static XaFile *s_xf;
static int s_nxf;

static int s_file = 1, s_chan;      /* CdlSetfilter */
static int s_active, s_mode, s_lba, s_end, s_vbl0, s_done;
static int s_servicing;
static FILE *s_fp;
static int s_fpidx = -1;
static unsigned char s_raw[2352];
static int s_deliver, s_getpos;    /* a data sector is being delivered (CdGetSector reads it) */
static int s_old[2], s_older[2];   /* decoder history, left/right (mono uses [0]) */
static short s_pcm[4032];

static void load_index(void)
{
    char path[512], line[256];
    FILE *f;
    int cap = 0;
    if (s_xf)
        return;
    snprintf(path, sizeof(path), "%s/DISC.LBA", PC_DataDir());
    f = fopen(path, "r");
    if (!f)
        return;
    while (fgets(line, sizeof(line), f)) {
        XaFile x;
        int bytes;
        if (line[0] == '#' || sscanf(line, "%d %d %d %d %95s", &x.lba, &x.nsect, &bytes, &x.secsize, x.path) != 5)
            continue;
        if (s_nxf == cap) {
            cap = cap ? cap * 2 : 256;
            s_xf = realloc(s_xf, cap * sizeof(XaFile));
        }
        s_xf[s_nxf++] = x;
    }
    fclose(f);
}

static int find_file(int lba)
{
    int i;
    for (i = 0; i < s_nxf; i++)
        if (lba >= s_xf[i].lba && lba < s_xf[i].lba + s_xf[i].nsect)
            return i;
    return -1;
}

/* the whole 2352-byte sector; 0 if the LBA is not in a raw (XA/STR) file */
static int read_raw(int lba)
{
    int i = find_file(lba);
    if (i < 0 || s_xf[i].secsize != 2352)
        return 0;
    if (i != s_fpidx) {
        char path[512];
        if (s_fp)
            fclose(s_fp);
        snprintf(path, sizeof(path), "%s/%s", PC_DataDir(), s_xf[i].path);
        s_fp = fopen(path, "rb");
        s_fpidx = s_fp ? i : -1;
        if (!s_fp)
            return 0;
    }
    fseek(s_fp, (long)(lba - s_xf[i].lba) * 2352, SEEK_SET);
    return fread(s_raw, 1, 2352, s_fp) == 2352;
}

/* ---- XA-ADPCM (psx-spx): 18 portions of 128 bytes; 16-byte header (shift/filter per block at 4+blk), then
 * 28 words, each holding one nibble (4-bit, 8 blocks) or byte (8-bit, 4 blocks) of every block ---- */
static const int K0[4] = { 0, 60, 115, 98 };
static const int K1[4] = { 0, 0, -52, -55 };

static void decode_block(const unsigned char *grp, int blk, int bits8, short *dst, int step, int *old, int *older)
{
    int hdr = grp[4 + blk];
    int sh = hdr & 0xF, f = (hdr >> 4) & 3, j;
    int f0 = K0[f], f1 = K1[f];
    if (sh > 12)
        sh = 9;                                 /* 13..15: reserved, act as 9 */
    sh = (bits8 ? 8 : 12) - sh;
    for (j = 0; j < 28; j++) {
        int t, s;
        if (bits8)
            t = (signed char)grp[16 + j * 4 + blk];
        else {
            t = (grp[16 + j * 4 + (blk >> 1)] >> ((blk & 1) * 4)) & 0xF;
            if (t & 8)
                t -= 16;
        }
        s = sh >= 0 ? t << sh : t >> -sh;
        s += (*old * f0 + *older * f1 + 32) >> 6;
        if (s > 32767) s = 32767;
        if (s < -32768) s = -32768;
        *older = *old;
        *old = s;
        dst[j * step] = (short)s;
    }
}

static void decode_audio(void)
{
    int ci = s_raw[19];
    int stereo = (ci & 3) == 1, bits8 = ((ci >> 4) & 3) == 1;
    int rate = (ci & 4) ? 18900 : 37800;
    int nblk = bits8 ? 4 : 8;
    int g, b, n = 0;
    for (g = 0; g < 18; g++) {
        const unsigned char *grp = s_raw + 24 + g * 128;
        if (stereo) {
            for (b = 0; b < nblk; b += 2) {
                decode_block(grp, b, bits8, s_pcm + n, 2, &s_old[0], &s_older[0]);
                decode_block(grp, b + 1, bits8, s_pcm + n + 1, 2, &s_old[1], &s_older[1]);
                n += 56;
            }
        } else {
            for (b = 0; b < nblk; b++) {
                decode_block(grp, b, bits8, s_pcm + n, 1, &s_old[0], &s_older[0]);
                n += 28;
            }
        }
    }
    PsyX_SPUAL_XAQueue(s_pcm, stereo ? n / 2 : n, stereo, rate);
}

/* ---- called by pc_cd.c ---- */
void PC_XaSetFilter(const unsigned char *param)
{
    if (param) {
        s_file = param[0];
        s_chan = param[1];
    }
}

void PC_XaStop(void)
{
    if (!s_active)
        return;
    s_active = 0;
    PsyX_SPUAL_XAStop();
}

void PC_XaStart(int lba, int mode)
{
    int i;
    load_index();
    PC_XaStop();
    i = find_file(lba);
    if (i < 0 || s_xf[i].secsize != 2352) {
        fprintf(stderr, "[pc_xa] XA stream at sector %d is not in a raw XA file - ignored\n", lba);
        return;
    }
    s_end = s_xf[i].lba + s_xf[i].nsect;
    s_mode = mode;
    s_lba = lba;
    s_done = 0;
    s_vbl0 = __real_VSync(-1);
    s_old[0] = s_old[1] = s_older[0] = s_older[1] = 0;
    s_active = 1;
}

int PC_XaActive(void) { return s_active; }

/* catches the stream up to the drive's position; non-audio sectors go to the ready/data callbacks */
void PC_XaService(CdlCB ready, void (*data)(void))
{
    int due;
    if (!s_active || s_servicing)
        return;
    s_servicing = 1;
    due = (__real_VSync(-1) - s_vbl0) * ((s_mode & CdlModeSpeed) ? 150 : 75) / 60;
    while (s_active && s_done < due) {
        int sub;
        if (s_lba >= s_end || !read_raw(s_lba)) {
            PC_XaStop();
            break;
        }
        s_lba++;
        s_done++;
        sub = s_raw[18];
        if ((sub & 0x44) == 0x44) {                     /* audio + real-time: the XA decoder's */
            if (!(s_mode & CdlModeSF) || (s_raw[16] == s_file && s_raw[17] == s_chan))
                decode_audio();
            continue;
        }
        if (ready) {                                     /* data: INT1 to the CPU */
            static u_char res[8];
            s_deliver = 1;
            s_getpos = 0;
            res[0] = CdlStatStandby | CdlStatRead;
            ready(CdlDataReady, res);
            if (s_getpos && data)
                data();
            s_deliver = 0;
        }
    }
    s_servicing = 0;
}

/* CdGetSector during a delivery: 924h mode starts at the header (raw + 12), 800h mode at the user data */
int PC_XaGetSector(void *madr, int size)
{
    int base = (s_mode & CdlModeSize1) ? 12 : 24;
    int max = (s_mode & CdlModeSize1) ? 0x924 : 0x800;
    int n = size * 4;
    if (!s_deliver)
        return 0;
    if (s_getpos + n > max)
        n = max - s_getpos;
    if (n > 0) {
        memcpy(madr, s_raw + base + s_getpos, n);
        s_getpos += n;
    }
    return 1;
}

/* SsSetSerialVol (pc_psyq.c) - the CD input volume, 0..127 per side */
void PC_XaSetVolume(int left, int right)
{
    PsyX_SPUAL_XASetVolume(left, right);
}
