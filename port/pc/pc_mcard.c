/* Driver 1 PC port - libmcrd (PsyQ memory card library) on a raw PS1 memory card image.
 *
 * The 12 MemCard* calls the game makes by address (func_00077E1C..func_0007932C, port/README.md "PsyQ functions
 * known only by address") implemented on a standard 128 KB raw card image (.mcd/.mcr: 16 blocks x 64 frames x
 * 128 bytes, block 0 = header + directory, psx-spx "Memory Card Data Format"), so a card written here loads in
 * DuckStation/PCSX and vice versa.
 *
 * Slot 1 (chan 0x00) is the image file: env D1_MCARD, else card1.mcd in the working directory; a missing file is
 * created as a freshly formatted card at MemCardStart. Slot 2 (chan 0x10) and multitap channels: no card.
 * A file that is not 128 KB is left alone and the slot reads as "no card" (never overwritten); a 128 KB file
 * without the "MC" header reads as an unformatted card (the game offers to format it).
 *
 * Semantics follow the game's own libmcrd (asm_fn/nonmatchings/main/func_00077E1C.. , state block D_000C9378):
 * - Exist/Accept/ReadFile/WriteFile register an asynchronous command and return 1 (0 = refused: not started,
 *   or ofs/bytes not a multiple of 128 for Read/WriteFile). Here every command completes at once; the result is
 *   delivered by the next MemCardSync, which then reports -1 (nothing registered) until the next command.
 * - MemCardSync(mode, *cmds, *rslt): -1 = nothing registered, 1 = finished (cmds/rslt filled, consumed).
 *   (0 = still running in mode 1 never happens here.)
 * - Exist: 0 = same card, 1 = no card, 3 = new card (set at power-on / when the image file changed on disk).
 *   Accept: 0, 1, 3 (new card - clears the flag), 4 = unformatted.
 * - GetDirentry/CreateFile/DeleteFile/Format are synchronous and return the McErr code (0 = ok); CreateFile 6 =
 *   exists, 7 = not enough free blocks; DeleteFile 5 = no such file; no card -> 1.
 * Every change is written back to the image at once (temp file + rename). Directory frames are kept the way the
 * BIOS writes them: 51h/52h/53h in use, A1h/A2h/A3h deleted, A0h free; size/name only in the first entry;
 * checksum = XOR of bytes 00h..7Eh. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <windows.h>
/* Windows: rename() won't replace an existing file; no symlinks to follow */
#define realpath(p, r) _fullpath((r), (p), PATH_MAX)
#define rename(from, to) (MoveFileExA((from), (to), MOVEFILE_REPLACE_EXISTING) ? 0 : -1)
#endif

#define MC_SIZE   0x20000
#define MC_BLOCK  0x2000
#define MC_FRAME  0x80

enum { McFuncExist = 1, McFuncAccept, McFuncReadFile, McFuncWriteFile };
enum { McErrNone, McErrCardNotExist, McErrCardInvalid, McErrNewCard, McErrNotFormat, McErrFileNotExist,
       McErrAlreadyExist, McErrBlockFull };

/* libapi DIRENTRY (40 bytes on the PS1 and in this 32-bit build) */
typedef struct { char name[20]; int attr; int size; void *next; int head; char system[4]; } MC_DIRENTRY;

static unsigned char s_card[MC_SIZE];
static enum { CARD_NONE, CARD_OK } s_state = CARD_NONE;
static int s_newcard = 1;          /* the card's "new card" flag: set at power-on, cleared by Accept */
static int s_started;              /* MemCardStart called (the library refuses Read/WriteFile before) */
static char s_path[1024];
static struct stat s_st;           /* the image as last loaded/written (change detection) */
static int s_st_valid;

static int s_pending, s_res_cmd, s_res_val;   /* the finished command MemCardSync will report */

#define LOG(...) do { printf("[mcard] " __VA_ARGS__); fflush(stdout); } while (0)

/* ---- image ---- */
static unsigned char *frame(int block, int f) { return s_card + block * MC_BLOCK + f * MC_FRAME; }
static unsigned char *dirent(int i) { return frame(0, i); }       /* i = 1..15 (block i) */

static void checksum(unsigned char *f)
{
    unsigned char x = 0;
    int i;
    for (i = 0; i < 0x7F; i++)
        x ^= f[i];
    f[0x7F] = x;
}

static int formatted(void) { return s_card[0] == 'M' && s_card[1] == 'C'; }

/* a freshly formatted card, as psx-spx describes it (and DuckStation's MemoryCardImage::Format writes it) */
static void format_image(void)
{
    int i;
    memset(s_card, 0xFF, MC_SIZE);
    memset(frame(0, 0), 0, MC_FRAME);
    s_card[0] = 'M';
    s_card[1] = 'C';
    checksum(frame(0, 0));
    for (i = 1; i < 16; i++) {                     /* directory: free */
        unsigned char *d = dirent(i);
        memset(d, 0, MC_FRAME);
        d[0] = 0xA0;
        d[8] = d[9] = 0xFF;
        checksum(d);
    }
    for (i = 16; i < 36; i++) {                    /* broken sector list: none */
        unsigned char *d = frame(0, i);
        memset(d, 0, MC_FRAME);
        d[0] = d[1] = d[2] = d[3] = 0xFF;
        d[8] = d[9] = 0xFF;
        checksum(d);
    }
    /* frames 36..62 (broken-sector replacement data, unused): psx-spx says "usually FFh" on real cards;
     * DuckStation zero-fills them - zero, so a new card is byte-identical to a DuckStation-formatted one */
    memset(frame(0, 36), 0, 27 * MC_FRAME);
    memcpy(frame(0, 63), frame(0, 0), MC_FRAME);   /* write-test frame: a copy of the header */
}

static const char *card_path(void)
{
    if (!s_path[0]) {
        const char *e = getenv("D1_MCARD");
        snprintf(s_path, sizeof s_path, "%s", e && *e ? e : "card1.mcd");
    }
    return s_path;
}

/* written as <file>.tmp + rename, so a crash never leaves half a card; a symlinked card (e.g. to an emulator's
 * card file) is written through to its target, not replaced by a plain file */
static int save_image(void)
{
    char tmp[PATH_MAX + 8], real[PATH_MAX];
    const char *dst = realpath(card_path(), real) ? real : card_path();
    FILE *f;
    snprintf(tmp, sizeof tmp, "%s.tmp", dst);
    f = fopen(tmp, "wb");
    if (!f || fwrite(s_card, 1, MC_SIZE, f) != MC_SIZE || fflush(f) != 0) {
        LOG("can't write %s\n", tmp);
        if (f)
            fclose(f);
        remove(tmp);
        return 0;
    }
    fclose(f);
    if (rename(tmp, dst) != 0) {
        LOG("can't replace %s\n", dst);
        remove(tmp);
        return 0;
    }
    s_st_valid = stat(card_path(), &s_st) == 0;
    return 1;
}

/* (re)load the image if the file changed since we last saw it; missing + create -> a new formatted card */
static void refresh(int create)
{
    struct stat st;
    FILE *f;
    static unsigned char buf[MC_SIZE];

    if (stat(card_path(), &st) != 0) {
        if (!create) {
            if (s_state != CARD_NONE)
                LOG("%s removed: no card\n", card_path());
            s_state = CARD_NONE;
            s_st_valid = 0;
            return;
        }
        format_image();
        if (save_image()) {
            LOG("created formatted card %s\n", card_path());
            s_state = CARD_OK;
            s_newcard = 1;
        } else
            s_state = CARD_NONE;
        return;
    }
    if (s_st_valid && st.st_mtime == s_st.st_mtime && st.st_size == s_st.st_size && st.st_ino == s_st.st_ino
#ifndef _WIN32
        && st.st_mtim.tv_nsec == s_st.st_mtim.tv_nsec
#endif
        )
        return;
    s_st = st;
    s_st_valid = 1;
    if (st.st_size != MC_SIZE || !(f = fopen(card_path(), "rb"))) {
        LOG("%s: not a 128 KB raw card image (%lld bytes): treated as no card\n", card_path(),
            (long long)st.st_size);
        s_state = CARD_NONE;
        return;
    }
    if (fread(buf, 1, MC_SIZE, f) != MC_SIZE) {
        fclose(f);
        LOG("%s: read error: no card\n", card_path());
        s_state = CARD_NONE;
        return;
    }
    fclose(f);
    if (s_state != CARD_OK || memcmp(buf, s_card, MC_SIZE) != 0) {
        if (s_state == CARD_OK)
            LOG("%s changed on disk: new card\n", card_path());
        memcpy(s_card, buf, MC_SIZE);
        s_newcard = 1;
    }
    s_state = CARD_OK;
    LOG("card %s%s\n", card_path(), formatted() ? "" : " (unformatted)");
}

static int card_ok(int chan) { return chan == 0 && s_state == CARD_OK; }

/* ---- directory ---- */
static int dir_used(int i) { return dirent(i)[0] == 0x51; }      /* first block of a file */
static int dir_free(int i) { return (dirent(i)[0] & 0xF0) == 0xA0; }
static int dir_next(int i) { int n = dirent(i)[8] | dirent(i)[9] << 8; return n == 0xFFFF ? 0 : n + 1; }
static int dir_size(int i)
{
    unsigned char *d = dirent(i);
    return d[4] | d[5] << 8 | d[6] << 16 | d[7] << 24;
}

/* BIOS-style name match: '?' any one char, '*' the rest */
static int name_match(const char *pat, const char *name)
{
    int i;
    for (i = 0; i < 20; i++) {
        if (pat[i] == '*')
            return 1;
        if (pat[i] != '?' && pat[i] != name[i])
            return 0;
        if (!pat[i])
            return 1;
    }
    return 1;
}

static int find_file(const char *name)
{
    int i;
    for (i = 1; i < 16; i++)
        if (dir_used(i) && strncmp((char *)dirent(i) + 0x0A, name, 20) == 0)
            return i;
    return 0;
}

/* the block holding byte `ofs` of the file starting at block `first` (0 = past the end) */
static int file_block(int first, int ofs)
{
    int b = first, k;
    for (k = ofs / MC_BLOCK; k > 0 && b; k--)
        b = dir_next(b);
    return b;
}

/* copy between the file and memory, frame by frame along the block chain */
static int file_io(const char *name, unsigned char *mem, int ofs, int n, int write)
{
    int first = find_file(name);
    if (!first)
        return McErrFileNotExist;
    if (ofs + n > dir_size(first))
        return McErrCardInvalid;   /* PC-PORT: unverified - the BIOS read/write past the file end just fails */
    while (n > 0) {
        int b = file_block(first, ofs), in = ofs % MC_BLOCK, k = MC_BLOCK - in;
        if (!b)
            return McErrCardInvalid;
        if (k > n)
            k = n;
        if (write)
            memcpy(s_card + b * MC_BLOCK + in, mem, k);
        else
            memcpy(mem, s_card + b * MC_BLOCK + in, k);
        mem += k;
        ofs += k;
        n -= k;
    }
    if (write && !save_image())
        return McErrCardInvalid;
    return McErrNone;
}

/* ---- async command plumbing ---- */
static int finish(int cmd, int result)
{
    s_pending = 1;
    s_res_cmd = cmd;
    s_res_val = result;
    return 1;
}

static int card_status(int chan, int cmd)
{
    if (chan != 0)
        return McErrCardNotExist;
    refresh(0);
    if (s_state != CARD_OK)
        return McErrCardNotExist;
    if (cmd == McFuncAccept) {
        int was_new = s_newcard;
        s_newcard = 0;     /* Accept writes the card (write-test frame), which clears its flag */
        if (!formatted())
            return McErrNotFormat;
        return was_new ? McErrNewCard : McErrNone;
    }
    return s_newcard ? McErrNewCard : McErrNone;
}

/* ---- libmcrd ---- */

/* MemCardInit(val) */
void func_00077E1C(int val)
{
    (void)val;
}

/* MemCardStart: also (re)loads the card image, creating a formatted one if the file is missing */
void func_00077E48(void)
{
    s_started = 1;
    s_pending = 0;
    refresh(1);
}

/* MemCardStop */
void func_00077E98(void)
{
    s_started = 0;
}

/* MemCardExist(chan) */
int func_00077ED8(int chan)
{
    return finish(McFuncExist, card_status(chan, McFuncExist));
}

/* MemCardAccept(chan) */
int func_00078114(int chan)
{
    return finish(McFuncAccept, card_status(chan, McFuncAccept));
}

/* MemCardReadFile(chan, name, buf, ofs, bytes) */
int func_00078798(int chan, char *name, void *buf, int ofs, int n)
{
    int r;
    if (!s_started || (ofs & 0x7F) || (n & 0x7F))
        return 0;
    r = card_ok(chan) ? (formatted() ? file_io(name, buf, ofs, n, 0) : McErrNotFormat) : McErrCardNotExist;
    LOG("read  %-20s ofs %5X n %5X -> %d\n", name, ofs, n, r);
    return finish(McFuncReadFile, r);
}

/* MemCardWriteFile(chan, name, buf, ofs, bytes) */
int func_000789B8(int chan, char *name, void *buf, int ofs, int n)
{
    int r;
    if (!s_started || (ofs & 0x7F) || (n & 0x7F))
        return 0;
    r = card_ok(chan) ? (formatted() ? file_io(name, buf, ofs, n, 1) : McErrNotFormat) : McErrCardNotExist;
    LOG("write %-20s ofs %5X n %5X -> %d\n", name, ofs, n, r);
    return finish(McFuncWriteFile, r);
}

/* MemCardGetDirentry(chan, name, dir, *files, ofs, max): matching files ofs..ofs+max-1 into dir[] */
int func_00078BD8(int chan, char *name, void *dir, int *files, int ofs, int max)
{
    MC_DIRENTRY *out = dir;
    int i, k = 0, cnt = 0;

    if (chan == 0)
        refresh(0);
    if (!card_ok(chan))
        return McErrCardNotExist;
    if (!formatted())
        return McErrNotFormat;
    for (i = 1; i < 16 && k < ofs + max; i++) {
        unsigned char *d = dirent(i);
        if (!dir_used(i) || !name_match(name, (char *)d + 0x0A))
            continue;
        if (k >= ofs && out) {
            MC_DIRENTRY *e = &out[cnt];
            memset(e, 0, sizeof *e);
            memcpy(e->name, d + 0x0A, 20);
            e->name[19] = 0;   /* PC-PORT: unverified - a 20-char name; the BIOS copies the terminated string */
            e->attr = d[0] & 0xF0;
            e->size = dir_size(i);
            e->head = i;
            cnt++;
        }
        k++;
    }
    if (files)
        *files = cnt;
    return McErrNone;
}

/* MemCardSync(mode, *cmds, *rslt): -1 = nothing registered, 1 = the registered command finished */
int func_00078E4C(int mode, int *cmds, int *rslt)
{
    (void)mode;
    if (!s_pending)
        return -1;
    if (cmds)
        *cmds = s_res_cmd;
    if (rslt)
        *rslt = s_res_val;
    s_pending = 0;
    return 1;
}

/* MemCardCreateFile(chan, name, blocks) */
int func_00078F68(int chan, char *name, int blocks)
{
    int i, n, prev = 0, free_n = 0, first = 0, r;

    if (chan == 0)
        refresh(0);
    if (!card_ok(chan))
        r = McErrCardNotExist;
    else if (!formatted())
        r = McErrNotFormat;
    else if (find_file(name))
        r = McErrAlreadyExist;
    else {
        if (blocks < 1)
            blocks = 1;
        for (i = 1; i < 16; i++)
            free_n += dir_free(i);
        if (free_n < blocks)
            r = McErrBlockFull;
        else {
            for (i = 1, n = 0; i < 16 && n < blocks; i++) {
                unsigned char *d;
                if (!dir_free(i))
                    continue;
                d = dirent(i);
                memset(d, 0, MC_FRAME);
                d[0] = n == 0 ? 0x51 : n == blocks - 1 ? 0x53 : 0x52;
                d[8] = d[9] = 0xFF;
                if (n == 0) {
                    int sz = blocks * MC_BLOCK;
                    first = i;
                    d[4] = sz;
                    d[5] = sz >> 8;
                    d[6] = sz >> 16;
                    d[7] = sz >> 24;
                    strncpy((char *)d + 0x0A, name, 20);
                } else {
                    unsigned char *p = dirent(prev);
                    p[8] = i - 1;
                    p[9] = 0;
                    checksum(p);
                }
                checksum(d);
                prev = i;
                n++;
            }
            r = save_image() ? McErrNone : McErrCardInvalid;
        }
    }
    LOG("create %-20s %d block(s) -> %d (block %d)\n", name, blocks, r, first);
    return r;
}

/* MemCardDeleteFile(chan, name) */
int func_00079170(int chan, char *name)
{
    int b, r;

    if (chan == 0)
        refresh(0);
    if (!card_ok(chan))
        r = McErrCardNotExist;
    else if (!formatted())
        r = McErrNotFormat;
    else if (!(b = find_file(name)))
        r = McErrFileNotExist;
    else {
        while (b) {
            unsigned char *d = dirent(b);
            int next = dir_next(b);
            d[0] = (d[0] & 0x0F) | 0xA0;   /* 51/52/53 -> A1/A2/A3; the rest stays, as the BIOS leaves it */
            checksum(d);
            b = next;
        }
        r = save_image() ? McErrNone : McErrCardInvalid;
    }
    LOG("delete %-20s -> %d\n", name, r);
    return r;
}

/* MemCardFormat(chan) */
int func_0007932C(int chan)
{
    int r;
    if (chan == 0)
        refresh(0);
    if (!card_ok(chan))
        r = McErrCardNotExist;
    else {
        format_image();
        r = save_image() ? McErrNone : McErrCardInvalid;
    }
    LOG("format -> %d\n", r);
    return r;
}
