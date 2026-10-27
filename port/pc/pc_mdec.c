/* Driver 1 PC port - libpress (DecDCT*) on a software MDEC.
 *
 * Two users: NFMV (src/nfmv/movie.c, the FMV player: 24-bit frames from .STR) and CRED.BIN (src/cred/cred.c, the
 * credits portraits: one 15-bit picture per portrait). Both use the PsyQ flow
 *     DecDCTvlc(bs, vlcbuf)        Huffman bitstream (BS v2/v3) -> MDEC run-level codes (CPU, libpress)
 *     DecDCTin(vlcbuf, mode)       MDEC(1) command + codes -> MDEC (DMA0)
 *     DecDCTout(img, words)        decoded macroblocks -> RAM (DMA1); the out callback runs when that DMA ends
 * Formats (psx-spx "Macroblock Decoder (MDEC)", "CDROM File Video BS Compression ..."):
 *   - BS header: [0] MDEC size/4 (halfword), [1] 3800h, [2] quant scale, [3] version (2 or 3), bitstream from +8,
 *     read as 16-bit little-endian halfwords, MSB first. Blocks Cr,Cb,Y1..Y4 per macroblock, macroblocks in
 *     columns (top to bottom, then the next 16-pixel column).
 *   - DecDCTvlc output = raw MDEC data: word 0 = the MDEC(1) command (3800h<<16 | size in words), then per block
 *     (quant<<10 | DC) + (run<<10 | level)... + FE00h (EOB); padded with FE00h.
 *   - DecDCTin mode (as PSn00bSDK's libpress): bit 0 = 24-bit (command bit 27 cleared: depth 2, else depth 3 =
 *     15-bit), bit 1 = set bit 15 (STP) of 15-bit pixels (command bit 25).
 *   - DecDCTout output (what DMA1 writes): every macroblock as 16 rows of 16 pixels (15-bit: halfwords, 24-bit:
 *     3 bytes R,G,B), so consecutive macroblocks form a 16-pixel-wide column - the slices LoadImage'd by the game.
 * Decoding follows psx-spx's rl_decode_block / real_idct_core / yuv_to_rgb (standard quant + scale tables as
 * DecDCTReset loads them). PC-PORT: unverified - the exact fixed-point rounding of the real MDEC is not known
 * (psx-spx says so too); the results can differ from hardware by +-1 in some pixels.
 *
 * Timing: the PS1 decodes asynchronously and raises the DMA1-done interrupt. Here DecDCTout decodes at once and
 * marks the transfer done; the out callback runs at the next service point (PC_DctService: every VSync, the
 * PC_WaitService busy-waits, and NFMV's frame wait), never inside DecDCTout - the callers set their "busy" state
 * after DecDCTout returns, as on the PS1. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

#define ONCE(msg) do { static int done_; if (!done_++) printf("[mdec] %s\n", msg); } while (0)

/* ---------------- tables ---------------- */
static const u8 s_zigzag[64] = {   /* natural position -> sequence number (psx-spx zigzag[]) */
    0, 1, 5, 6, 14, 15, 27, 28,
    2, 4, 7, 13, 16, 26, 29, 42,
    3, 8, 12, 17, 25, 30, 41, 43,
    9, 11, 18, 24, 31, 40, 44, 53,
    10, 19, 23, 32, 39, 45, 52, 54,
    20, 22, 33, 38, 46, 51, 55, 60,
    21, 34, 37, 47, 50, 56, 59, 61,
    35, 36, 48, 49, 57, 58, 62, 63,
};
static u8 s_zagzig[64];            /* sequence number -> natural position */

/* the standard PSX quant table (MPEG-1 intra matrix with DC 2), natural order; libpress's DecDCTReset loads it
 * for luminance and colour (jpsxdec PSX_DEFAULT_QUANTIZATION_MATRIX) */
static const u8 s_iq_nat[64] = {
    2, 16, 19, 22, 26, 27, 29, 34,
    16, 16, 22, 24, 27, 29, 34, 37,
    19, 22, 26, 27, 29, 34, 34, 38,
    22, 22, 26, 27, 29, 34, 37, 40,
    22, 26, 27, 29, 32, 35, 40, 48,
    26, 27, 29, 32, 35, 40, 48, 58,
    26, 27, 29, 34, 38, 46, 56, 69,
    27, 29, 35, 38, 46, 56, 69, 83,
};
static int s_iq[64];               /* MDEC(2) order = sequence order */

static const u16 s_scale[64] = {   /* MDEC(3) standard scale table (psx-spx) */
    0x5A82, 0x5A82, 0x5A82, 0x5A82, 0x5A82, 0x5A82, 0x5A82, 0x5A82,
    0x7D8A, 0x6A6D, 0x471C, 0x18F8, 0xE707, 0xB8E3, 0x9592, 0x8275,
    0x7641, 0x30FB, 0xCF04, 0x89BE, 0x89BE, 0xCF04, 0x30FB, 0x7641,
    0x6A6D, 0xE707, 0x8275, 0xB8E3, 0x471C, 0x7D8A, 0x18F8, 0x9592,
    0x5A82, 0xA57D, 0xA57D, 0x5A82, 0x5A82, 0xA57D, 0xA57D, 0x5A82,
    0x471C, 0x8275, 0x18F8, 0x6A6D, 0x9592, 0xE707, 0x7D8A, 0xB8E3,
    0x30FB, 0x89BE, 0x7641, 0xCF04, 0xCF04, 0x7641, 0x89BE, 0x30FB,
    0x18F8, 0xB8E3, 0x6A6D, 0x8275, 0x7D8A, 0x9592, 0x471C, 0xE707,
};
static int s_st[64];               /* the upper 13 bits the hardware uses ("/8") */

/* AC Huffman codes, BS v1/v2/v3 (psx-spx "Huffman codes for AC values"): prefix, index bits, MDEC values
 * (run<<10 | level); every code but EOB/escape ends in a sign bit */
static const struct { const char *prefix; int xbits; u16 val[16]; } s_ac_codes[] = {
    { "11", 0, { 0x0001 } },
    { "011", 0, { 0x0401 } },
    { "010", 1, { 0x0002, 0x0801 } },
    { "0011", 1, { 0x1001, 0x0C01 } },
    { "00101", 0, { 0x0003 } },
    { "00100", 3, { 0x3401, 0x0006, 0x3001, 0x2C01, 0x0C02, 0x0403, 0x0005, 0x2801 } },
    { "0001", 2, { 0x1C01, 0x1801, 0x0402, 0x1401 } },
    { "00001", 2, { 0x0802, 0x2401, 0x0004, 0x2001 } },
    { "0000001", 3, { 0x4001, 0x1402, 0x0007, 0x0803, 0x0404, 0x3C01, 0x3801, 0x1002 } },
    { "00000001", 4, { 0x000B, 0x2002, 0x1003, 0x000A, 0x0804, 0x1C02, 0x5401, 0x5001,
                       0x0009, 0x4C01, 0x4801, 0x0405, 0x0C03, 0x0008, 0x1802, 0x4401 } },
    { "000000001", 4, { 0x2802, 0x2402, 0x1403, 0x0C04, 0x0805, 0x0407, 0x0406, 0x000F,
                        0x000E, 0x000D, 0x000C, 0x6801, 0x6401, 0x6001, 0x5C01, 0x5801 } },
    { "0000000001", 4, { 0x001F, 0x001E, 0x001D, 0x001C, 0x001B, 0x001A, 0x0019, 0x0018,
                         0x0017, 0x0016, 0x0015, 0x0014, 0x0013, 0x0012, 0x0011, 0x0010 } },
    { "00000000001", 4, { 0x0028, 0x0027, 0x0026, 0x0025, 0x0024, 0x0023, 0x0022, 0x0021,
                          0x0020, 0x040E, 0x040D, 0x040C, 0x040B, 0x040A, 0x0409, 0x0408 } },
    { "000000000001", 4, { 0x0412, 0x0411, 0x0410, 0x040F, 0x1803, 0x4002, 0x3C02, 0x3802,
                           0x3402, 0x3002, 0x2C02, 0x7C01, 0x7801, 0x7401, 0x7001, 0x6C01 } },
};
#define AC_BITS 17                  /* longest code: 12 prefix + 4 index + sign */
#define AC_EOB 0xFFFE
#define AC_ESC 0xFFFD
#define AC_BAD 0xFFFC
static u16 *s_ac_val;                /* next 17 bits -> MDEC value / AC_EOB / AC_ESC / AC_BAD (malloc: keeps the */
static u8 *s_ac_len;                 /* exe's .bss small, see pc_fmv.c) */

static void init_tables(void)
{
    static int done;
    unsigned i, c;
    if (done)
        return;
    done = 1;
    s_ac_val = malloc((1 << AC_BITS) * sizeof(u16));
    s_ac_len = malloc(1 << AC_BITS);
    for (i = 0; i < 64; i++)
        s_zagzig[s_zigzag[i]] = i;
    for (i = 0; i < 64; i++)
        s_iq[i] = s_iq_nat[s_zagzig[i]];
    for (i = 0; i < 64; i++)
        s_st[i] = (short)s_scale[i] >> 3;
    for (i = 0; i < (1u << AC_BITS); i++)
        s_ac_val[i] = AC_BAD;
    /* fill every 17-bit pattern a code is a prefix of */
#define FILL(bits, len, v) do { unsigned b_ = (bits) << (AC_BITS - (len)), n_ = 1u << (AC_BITS - (len)), k_; \
        for (k_ = 0; k_ < n_; k_++) { s_ac_val[b_ + k_] = (v); s_ac_len[b_ + k_] = (len); } } while (0)
    FILL(2, 2, AC_EOB);                 /* "10" */
    FILL(1, 6, AC_ESC);                 /* "000001" + 16 raw bits */
    for (c = 0; c < sizeof(s_ac_codes) / sizeof(s_ac_codes[0]); c++) {
        const char *p = s_ac_codes[c].prefix;
        unsigned pre = 0, plen = strlen(p), x, s;
        for (i = 0; i < plen; i++)
            pre = pre << 1 | (p[i] == '1');
        for (x = 0; x < (1u << s_ac_codes[c].xbits); x++)
            for (s = 0; s < 2; s++) {
                unsigned len = plen + s_ac_codes[c].xbits + 1;
                unsigned code = ((pre << s_ac_codes[c].xbits | x) << 1) | s;
                u16 v = s_ac_codes[c].val[x];
                if (s)
                    v = (v & 0xFC00) | (-(v & 0x3FF) & 0x3FF);
                FILL(code, len, v);
            }
    }
#undef FILL
}

/* ---------------- DecDCTvlc: BS v2/v3 bitstream -> MDEC codes ---------------- */
typedef struct { const u16 *p; u32 buf; int n; } Bits;   /* buf holds n valid bits, MSB-aligned */

static void bits_fill(Bits *b)
{
    while (b->n <= 16) {
        b->buf |= (u32)*b->p++ << (16 - b->n);
        b->n += 16;
    }
}

static u32 bits_peek(Bits *b, int k)   /* k <= 17 */
{
    bits_fill(b);
    if (k > b->n) {                    /* only after a fill: n >= 17 */
        return b->buf >> (32 - k);
    }
    return b->buf >> (32 - k);
}

static void bits_skip(Bits *b, int k)
{
    b->buf <<= k;
    b->n -= k;
}

static u32 bits_get(Bits *b, int k)
{
    u32 v;
    if (k == 0)
        return 0;
    v = bits_peek(b, k);
    bits_skip(b, k);
    return v;
}

/* v3 DC: JPEG-style size category (psx-spx "DC v3"), value*4 added to the previous DC of that component */
static int dc_v3(Bits *b, int chroma, int *eof)
{
    int size, v;
    *eof = 0;
    if (bits_peek(b, 10) == 0x3FF) {
        *eof = 1;
        return 0;
    }
    if (chroma) {               /* 00:0 01:1 10:2 110:3 1110:4 ... 11111110:8 */
        if (bits_peek(b, 2) < 3) {
            size = bits_get(b, 2);
        } else {
            int ones = 0;
            while (bits_peek(b, 1)) {
                bits_skip(b, 1);
                ones++;
            }
            bits_skip(b, 1);
            size = ones + 1;
        }
    } else {                    /* 00:1 01:2 100:0 101:3 110:4 1110:5 ... 1111110:8 */
        u32 t = bits_peek(b, 3);
        if (t < 2) {
            bits_skip(b, 2);
            size = 1;
        } else if (t < 4) {
            bits_skip(b, 2);
            size = 2;
        } else if (t == 4) {
            bits_skip(b, 3);
            size = 0;
        } else if (t == 5) {
            bits_skip(b, 3);
            size = 3;
        } else {
            int ones = 0;
            while (bits_peek(b, 1)) {
                bits_skip(b, 1);
                ones++;
            }
            bits_skip(b, 1);
            size = ones + 2;
        }
    }
    if (size == 0)
        return 0;
    v = bits_get(b, size);
    if (!(v >> (size - 1)))     /* leading 0: negative */
        v -= (1 << size) - 1;
    return v * 4;
}

static int s_vlc_size;          /* DecDCTvlcSize: 0 = whole frame */

/* returns 0 when the frame is complete (the only case NFMV/CRED use: DecDCTvlcSize(0)) */
int PC_DecDCTvlc(const void *bs_, void *buf_)
{
    const u16 *bs = (const u16 *)bs_;
    u32 *buf = (u32 *)buf_;
    u16 *out = (u16 *)(buf + 1);
    int maxhw = bs[0] * 2, nhw = 0;
    int quant = bs[2] & 0x3F, ver = bs[3];
    int dc[3] = { 0, 0, 0 };    /* v3 predictors: Cr, Cb, Y */
    int blk = 0;
    Bits b;
    init_tables();
    if (bs[1] != 0x3800 || (ver != 2 && ver != 3)) {
        static int warned;
        if (!warned++)
            fprintf(stderr, "[mdec] DecDCTvlc: unsupported bitstream (id %04X version %d)\n", bs[1], ver);
        buf[0] = 0x38000000;
        return 0;
    }
    b.p = bs + 4;
    b.buf = 0;
    b.n = 0;
#define PUT(v) do { if (nhw < maxhw) out[nhw] = (v); nhw++; } while (0)
    for (;;) {
        int comp = blk % 6 < 2 ? blk % 6 : 2, d, k;
        if (nhw >= maxhw)
            break;
        if (ver == 2) {
            d = bits_peek(&b, 10);
            if (d == 0x1FF)                     /* end of frame (in place of a DC) */
                break;
            bits_skip(&b, 10);
            d &= 0x3FF;
        } else {
            int eof;
            int off = dc_v3(&b, comp < 2, &eof);
            if (eof)
                break;
            dc[comp] += off;
            d = dc[comp] & 0x3FF;               /* "newer version": wrap within 10 bits */
        }
        PUT((quant << 10) | d);
        for (k = 1;;) {                         /* AC codes up to EOB */
            u32 w = bits_peek(&b, AC_BITS);
            u16 v = s_ac_val[w];
            if (v == AC_EOB) {
                bits_skip(&b, 2);
                PUT(0xFE00);
                break;
            }
            if (v == AC_BAD) {
                static int warned;
                if (!warned++)
                    fprintf(stderr, "[mdec] DecDCTvlc: bad Huffman code\n");
                PUT(0xFE00);
                goto done;
            }
            if (v == AC_ESC) {
                bits_skip(&b, 6);
                v = bits_get(&b, 16);
            } else
                bits_skip(&b, s_ac_len[w]);
            PUT(v);
            k += (v >> 10) + 1;
            if (k > 63) {                       /* block full: the EOB is still in the stream */
                ;
            }
        }
        blk++;
    }
done:
    while (nhw & 63)                            /* pad to the 80h-byte DMA block */
        PUT(0xFE00);
#undef PUT
    if (nhw > maxhw)
        nhw = maxhw;
    buf[0] = 0x38000000u | (nhw / 2);
    return 0;
}

void PC_DecDCTvlcSize(int size)
{
    s_vlc_size = size;
}

/* ---------------- the MDEC ---------------- */
static const u16 *s_in, *s_in_end;
static u32 s_cmd;
static int s_cur_blocks[6][64];

static int sext10(int v)
{
    return (v & 0x200) ? (v | ~0x3FF) : (v & 0x3FF);
}

static void idct(int *blk)
{
    int tmp[64], pass, x, y, z;
    int *src = blk, *dst = tmp;
    for (pass = 0; pass < 2; pass++) {
        for (x = 0; x < 8; x++)
            for (y = 0; y < 8; y++) {
                int sum = 0;
                for (z = 0; z < 8; z++)
                    sum += src[y + z * 8] * s_st[x + z * 8];
                dst[x + y * 8] = (sum + 0xFFF) >> 13;
            }
        src = tmp;
        dst = blk;
    }
}

/* rl_decode_block; returns 0 when the input ran out */
static int rl_block(int *blk)
{
    int k, q, n, val;
    memset(blk, 0, 64 * sizeof(int));
    do {
        if (s_in >= s_in_end)
            return 0;
        n = *s_in++;
    } while (n == 0xFE00);
    q = (n >> 10) & 0x3F;
    k = 0;
    val = sext10(n & 0x3FF) * s_iq[0];
    for (;;) {
        if (q == 0)
            val = sext10(n & 0x3FF) * 2;
        if (val < -0x400)
            val = -0x400;
        if (val > 0x3FF)
            val = 0x3FF;
        if (q > 0)
            blk[s_zagzig[k]] = val;
        else
            blk[k] = val;
        if (s_in >= s_in_end)
            break;
        n = *s_in++;
        k += ((n >> 10) & 0x3F) + 1;
        if (k > 63)
            break;
        val = (sext10(n & 0x3FF) * s_iq[k] * q + 4) / 8;
    }
    idct(blk);
    return 1;
}

static int clamp9s8(int v)          /* psx-spx y_to_mono: keep 9 bits (signed), saturate to -128..127 */
{
    v &= 0x1FF;
    if (v & 0x100)
        v -= 0x200;
    if (v < -128)
        v = -128;
    if (v > 127)
        v = 127;
    return v;
}

/* one colour macroblock into 16x16 pixels at out (row stride 16 pixels) */
static int decode_mb(u8 *out)
{
    int *cr = s_cur_blocks[0], *cb = s_cur_blocks[1], *yb = s_cur_blocks[2];
    int depth = (s_cmd >> 27) & 3, sgn = (s_cmd >> 26) & 1, stp = (s_cmd >> 25) & 1;
    int i, x, y;
    if (!rl_block(cr) || !rl_block(cb))
        return 0;
    for (i = 0; i < 4; i++) {
        int xx = (i & 1) * 8, yy = (i >> 1) * 8;
        if (!rl_block(yb))
            return 0;
        for (y = 0; y < 8; y++)
            for (x = 0; x < 8; x++) {
                int c = ((x + xx) >> 1) + ((y + yy) >> 1) * 8;
                int R = cr[c], B = cb[c], Y = yb[x + y * 8], r, g, bb;
                int px = (x + xx) + (y + yy) * 16;
                /* 1.402, -0.3437/-0.7143, 1.772 in 8.8 fixed point (PC-PORT: unverified rounding) */
                r = clamp9s8(Y + ((359 * R + 128) >> 8));
                g = clamp9s8(Y + ((-88 * B - 183 * R + 128) >> 8));
                bb = clamp9s8(Y + ((454 * B + 128) >> 8));
                if (!sgn) {
                    r ^= 0x80;
                    g ^= 0x80;
                    bb ^= 0x80;
                }
                r &= 0xFF;
                g &= 0xFF;
                bb &= 0xFF;
                if (depth == 2) {
                    out[px * 3 + 0] = r;
                    out[px * 3 + 1] = g;
                    out[px * 3 + 2] = bb;
                } else {
                    /* 15-bit: 8 -> 5 bits with rounding (as mednafen; PC-PORT: unverified vs hardware) */
                    int r5 = (r + 4) >> 3, g5 = (g + 4) >> 3, b5 = (bb + 4) >> 3;
                    if (r5 > 31) r5 = 31;
                    if (g5 > 31) g5 = 31;
                    if (b5 > 31) b5 = 31;
                    ((u16 *)out)[px] = r5 | g5 << 5 | b5 << 10 | stp << 15;
                }
            }
    }
    return 1;
}

/* ---------------- libpress API ---------------- */
static void (*s_out_cb)(void);
static int s_out_done, s_cb_busy;

void PC_DecDCTReset(int mode)
{
    init_tables();
    s_in = s_in_end = NULL;
    s_out_done = 0;
    (void)mode;
}

void PC_DecDCTin(const void *buf, int mode)
{
    const u32 *w = (const u32 *)buf;
    init_tables();
    s_cmd = w[0];
    if (mode & 1)
        s_cmd &= ~(1u << 27);           /* 24-bit */
    else
        s_cmd |= 1u << 27;              /* 15-bit */
    if (mode & 2)
        s_cmd |= 1u << 25;
    else
        s_cmd &= ~(1u << 25);
    s_in = (const u16 *)(w + 1);
    s_in_end = s_in + (s_cmd & 0xFFFF) * 2;
}

void PC_DecDCTout(void *buf, int size)
{
    int depth = (s_cmd >> 27) & 3;
    int mbwords = depth == 2 ? 192 : 128;
    u8 *out = (u8 *)buf;
    int n = size / mbwords, i;
    if (depth != 2 && depth != 3) {
        ONCE("DecDCTout: monochrome MDEC output not supported");
        n = 0;
    }
    for (i = 0; i < n; i++) {
        if (!decode_mb(out)) {
            memset(out, 0, (n - i) * mbwords * 4);   /* input exhausted: the rest stays black */
            break;
        }
        out += mbwords * 4;
    }
    s_out_done = 1;                      /* DMA1 done: the callback runs at the next service point */
}

void (*PC_DecDCToutCallback(void (*func)(void)))(void)
{
    void (*old)(void) = s_out_cb;
    s_out_cb = func;
    return old;
}

/* the DMA1-done interrupt: runs the out callback for each finished transfer (the callback usually starts the next
 * one with DecDCTout, which this loop then completes too) */
void PC_DctService(void)
{
    int guard = 0;
    if (s_cb_busy)
        return;
    s_cb_busy = 1;
    while (s_out_done && guard++ < 256) {
        s_out_done = 0;
        if (s_out_cb)
            s_out_cb();
    }
    s_cb_busy = 0;
}

/* the PsyQ names (NFMV calls libpress by name; the main exe by address, see pc_psyq.c) */
void DecDCTReset(int mode) { PC_DecDCTReset(mode); }
int DecDCTvlc(unsigned int *bs, unsigned int *buf) { return PC_DecDCTvlc(bs, buf); }
void DecDCTvlcSize(int size) { PC_DecDCTvlcSize(size); }
void DecDCTin(unsigned int *buf, int mode) { PC_DecDCTin(buf, mode); }
void DecDCTout(unsigned int *buf, int size) { PC_DecDCTout(buf, size); }
int DecDCToutCallback(void (*func)(void)) { return (int)PC_DecDCToutCallback(func); }
