/* Driver 1 PC port - the CD drive (replaces PsyCross LIBCD.C for this game; that archive member is never linked).
 *
 * Disc source: the extracted tree from port/pc/pc_extract.py (pcdata/, see port/README.md). Its DISC.LBA index
 * keeps every file at its original sector, so the game's own sector arithmetic stays untouched:
 * CdSearchFile/CdPosToInt hand out the real LBAs, and every sector read (CdRead, the CdlReadS stream that
 * spool.c and loadsectors() use) is served from the file that covers that LBA. Replacing a file in pcdata/ with
 * one of the same sector count is enough to mod it.
 *
 * Timing model: a read is instantaneous. CdRead completes before it returns (CdReadSync then says 0 sectors
 * left). A CdlReadS stream runs synchronously on the game thread when the game would first see its data:
 *   - when a ready callback is installed while a stream is pending (loadsectors(): ReadS, then callbacks), or
 *   - at the next VSync()/CdSync()/CdReadSync() (UpdateSpool(): callbacks, then ReadS).
 * The stream then delivers CdlDataReady per sector (ready callback -> CdGetSector -> data callback) until the
 * game pauses it or clears the ready callback, exactly the PS1 callback order. Re-seeks issued from inside a
 * callback (spool.c's UpdateSpool from data_cb_*) continue the same loop. No thread: deterministic.
 * XA audio streams (CdlModeRT set) go to pc_xa.c, which plays them in real time (see there). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include "psx/libcd.h"

extern const char *PC_DataDir(void);       /* pc_main.c */
/* pc_xa.c: XA-ADPCM streams (CdlModeRT) */
extern void PC_XaSetFilter(const u_char *param);
extern void PC_XaStart(int lba, int mode);
extern void PC_XaStop(void);
extern int PC_XaActive(void);
extern void PC_XaService(CdlCB ready, void (*data)(void));
extern int PC_XaGetSector(void *madr, int size);

typedef struct {
    int lba, nsect, bytes, secsize;
    char path[96];
} DiscFile;

static DiscFile *s_files;
static int s_nfiles;
static FILE *s_fp;
static int s_fpidx = -1;

static int s_lba;                 /* Setloc / current read position */
static int s_mode;                /* last CdlSetmode */
static u_char s_lastcom;

static CdlCB s_ready;
static void (*s_data)(void);
static int s_pending;             /* CdlReadS issued, stream not paused */
static int s_pumping;
static int s_stream_lba;
static u_char s_sector[2048];
static int s_getpos;
static int s_size_warned;

/* ---- the disc index ---- */
static void load_index(void)
{
    char path[512], line[256];
    FILE *f;
    int cap = 0;

    if (s_files)
        return;
    snprintf(path, sizeof(path), "%s/DISC.LBA", PC_DataDir());
    f = fopen(path, "r");
    if (!f) {
        fprintf(stderr, "[pc_cd] %s not found - extract the disc first (port/README.md)\n", path);
        exit(1);
    }
    while (fgets(line, sizeof(line), f)) {
        DiscFile d;
        if (line[0] == '#')
            continue;
        if (sscanf(line, "%d %d %d %d %95s", &d.lba, &d.nsect, &d.bytes, &d.secsize, d.path) != 5)
            continue;
        if (s_nfiles == cap) {
            cap = cap ? cap * 2 : 256;
            s_files = realloc(s_files, cap * sizeof(DiscFile));
        }
        s_files[s_nfiles++] = d;
    }
    fclose(f);
    printf("[pc_cd] %d files from %s\n", s_nfiles, path);
}

static int find_lba(int lba)
{
    int lo = 0, hi = s_nfiles - 1;
    while (lo <= hi) {
        int m = (lo + hi) / 2;
        if (lba < s_files[m].lba)
            hi = m - 1;
        else if (lba >= s_files[m].lba + s_files[m].nsect)
            lo = m + 1;
        else
            return m;
    }
    return -1;
}

/* reads the 2048 user-data bytes of one sector */
static void read_sector(int lba, u_char *dst)
{
    int i = find_lba(lba);
    memset(dst, 0, 2048);
    if (i < 0) {
        fprintf(stderr, "[pc_cd] read of sector %d outside every file\n", lba);
        return;
    }
    if (i != s_fpidx) {
        char path[512];
        if (s_fp)
            fclose(s_fp);
        snprintf(path, sizeof(path), "%s/%s", PC_DataDir(), s_files[i].path);
        s_fp = fopen(path, "rb");
        s_fpidx = i;
        if (!s_fp) {
            fprintf(stderr, "[pc_cd] can't open %s\n", path);
            s_fpidx = -1;
            return;
        }
    }
    if (s_files[i].secsize == 2048)
        fseek(s_fp, (long)(lba - s_files[i].lba) * 2048, SEEK_SET);
    else
        fseek(s_fp, (long)(lba - s_files[i].lba) * s_files[i].secsize + 24, SEEK_SET);
    fread(dst, 1, 2048, s_fp);
}

/* "\DRIVER\GFX\GLOAD.TIM;1" -> index entry */
static DiscFile *lookup(const char *name)
{
    char want[128];
    int i, n = 0;
    load_index();
    if (*name == '\\' || *name == '/')
        name++;
    for (; *name && *name != ';' && n < (int)sizeof(want) - 1; name++)
        want[n++] = *name == '\\' ? '/' : toupper((u_char)*name);
    want[n] = 0;
    for (i = 0; i < s_nfiles; i++)
        if (!strcmp(s_files[i].path, want))
            return &s_files[i];
    return NULL;
}

int PC_CdFileSize(const char *name)
{
    DiscFile *d = lookup(name);
    return d ? d->bytes : -1;
}

/* ---- the CdlReadS stream ---- */
static void pump(void)
{
    int guard = 0;
    if (s_pumping)
        return;
    s_pumping = 1;
    while (s_pending && s_ready) {
        static u_char res[8];
        CdlCB ready = s_ready;
        read_sector(s_stream_lba++, s_sector);
        s_getpos = 0;
        res[0] = CdlStatStandby | CdlStatRead;
        ready(CdlDataReady, res);
        if (s_getpos && s_data)
            s_data();
        if (++guard > 400000) {      /* more than the whole disc: the game never stops this stream */
            fprintf(stderr, "[pc_cd] runaway CdlReadS stream at sector %d - pausing it\n", s_stream_lba);
            s_pending = 0;
        }
    }
    s_pumping = 0;
}

/* the points where the PS1 would have taken CD interrupts: pump a pending stream */
void PC_CdService(void)
{
    if (PC_XaActive())           /* an XA stream runs on the drive's clock (pc_xa.c) */
        PC_XaService(s_ready, s_data);
    if (s_pending && s_ready && !s_pumping)
        pump();
}

/* ---- libcd ---- */
int CdInit(void)
{
    load_index();
    PC_XaStop();
    s_ready = NULL;
    s_data = NULL;
    s_pending = 0;
    return 1;
}

int CdPosToInt(CdlLOC *p)
{
    return 75 * (60 * DECODE_BCD(p->minute) + DECODE_BCD(p->second)) + DECODE_BCD(p->sector) - 150;
}

CdlLOC *CdIntToPos(int i, CdlLOC *p)
{
    i += 150;
    p->sector = ENCODE_BCD(i % 75);
    p->second = ENCODE_BCD(i / 75 % 60);
    p->minute = ENCODE_BCD(i / 75 / 60);
    return p;
}

CdlFILE *CdSearchFile(CdlFILE *fp, char *name)
{
    DiscFile *d = lookup(name);
    const char *base;
    if (!d) {
        fprintf(stderr, "[pc_cd] CdSearchFile: %s not on the disc\n", name);
        return NULL;
    }
    memset(fp, 0, sizeof(*fp));
    CdIntToPos(d->lba, &fp->pos);
    fp->size = d->bytes;
    base = strrchr(d->path, '/');
    base = base ? base + 1 : d->path;
    snprintf(fp->name, sizeof(fp->name), "%s;1", base);
    return fp;
}

static int control(u_char com, u_char *param, u_char *result)
{
    s_lastcom = com;
    if (result)
        result[0] = CdlStatStandby;
    switch (com) {
    case CdlSetloc:
        if (param)
            s_lba = CdPosToInt((CdlLOC *)param);
        break;
    case CdlSetmode:
        if (param)
            s_mode = param[0];
        break;
    case CdlReadS:
    case CdlReadN:
        if (param)
            s_lba = CdPosToInt((CdlLOC *)param);
        s_stream_lba = s_lba;
        PC_XaStop();
        if (s_mode & CdlModeRT) {
            /* XA-ADPCM (xaplay.c): pc_xa.c plays the audio sectors and delivers the others */
            s_pending = 0;
            PC_XaStart(s_lba, s_mode);
            break;
        }
        s_pending = 1;           /* runs at the next service point, see the head comment */
        break;
    case CdlPause:
    case CdlStop:
    case CdlStandby:
        s_pending = 0;
        PC_XaStop();
        break;
    case CdlSetfilter:           /* XA file/channel */
        PC_XaSetFilter(param);
        break;
    default:                     /* Demute, Nop, GetlocP ...: nothing to emulate */
        break;
    }
    return 1;
}

int CdControl(u_char com, u_char *param, u_char *result) { return control(com, param, result); }
int CdControlB(u_char com, u_char *param, u_char *result) { return control(com, param, result); }
int CdControlF(u_char com, u_char *param) { return control(com, param, NULL); }

int CdRead(int sectors, u_long *buf, int mode)
{
    u_char *p = (u_char *)buf;
    if ((mode & (CdlModeSize1 | CdlModeSize0)) && !s_size_warned++)
        printf("[pc_cd] CdRead mode 0x%x: raw sector sizes not emulated, reading 2048-byte sectors\n", mode);
    s_pending = 0;               /* a CdlReadN replaces any running stream */
    PC_XaStop();
    while (sectors-- > 0) {
        read_sector(s_lba++, p);
        p += 2048;
    }
    return 1;
}

int CdReadSync(int mode, u_char *result)
{
    PC_CdService();
    if (result)
        result[0] = CdlStatStandby;
    return 0;                    /* no sectors left */
}

int CdSync(int mode, u_char *result)
{
    PC_CdService();
    if (result)
        result[0] = CdlStatStandby;
    return CdlComplete;
}

int CdReady(int mode, u_char *result)
{
    if (result)
        result[0] = CdlStatStandby;
    return CdlNoIntr;
}

CdlCB CdReadyCallback(CdlCB func)
{
    CdlCB old = s_ready;
    s_ready = func;
    if (func)
        PC_CdService();
    return old;
}

void (*CdDataCallback(void (*func)()))
{
    void (*old)(void) = s_data;
    s_data = (void (*)(void))func;
    return (void *)old;
}

CdlCB CdSyncCallback(CdlCB func) { return NULL; }
CdlCB CdReadCallback(CdlCB func) { return NULL; }

/* copies the next `size` words of the current stream sector (successive calls continue, as on PS1) */
int CdGetSector(void *madr, int size)
{
    int n = size * 4;
    if (PC_XaGetSector(madr, size))  /* a data sector of an XA stream */
        return 1;
    if (s_getpos + n > 2048)
        n = 2048 - s_getpos;
    if (n > 0) {
        memcpy(madr, s_sector + s_getpos, n);
        s_getpos += n;
    }
    return 1;
}

int CdReadFile(char *file, u_long *addr, int nbyte)
{
    CdlFILE f;
    int n;
    if (!CdSearchFile(&f, file))
        return 0;
    n = nbyte ? nbyte : (int)f.size;
    s_lba = CdPosToInt(&f.pos);
    {
        /* whole sectors, like the PS1 DMA: up to 2047 bytes past the file get the rest of its last sector */
        u_char *p = (u_char *)addr;
        int k;
        for (k = (n + 2047) / 2048; k > 0; k--, p += 2048)
            read_sector(s_lba++, p);
    }
    return n;
}

int CdLastCom(void) { return s_lastcom; }
int CdMode(void) { return s_mode; }
int CdStatus(void) { return CdlStatStandby; }
int CdDiskReady(int mode) { return CdlComplete; }
int CdSetDebug(int level) { return 0; }
void CdFlush(void) { }
