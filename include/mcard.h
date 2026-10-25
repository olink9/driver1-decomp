/* Driver 1 decomp - MCARD overlay shared records (Step 3 for the overlays).
 *
 * Types only (no globals, no prototypes) for src/mcard/mcard.c. The draw buffer, the libgpu primitives
 * (POLY_FT4, POLY_F4, P_TAG, RECT), the texture-page sheet (TP_SHEET, D_000EFC08) and the texture lookup
 * results (TEX, TEXINFO) come from frontend.h / structs.h; this adds the memory card directory, the
 * overlay's bitmap font table and the SJIS punctuation table.
 *
 *  - Include after any "common.h" (and see frontend.h for what that changes). Plain char is unsigned.
 */
#ifndef D1_MCARD_H
#define D1_MCARD_H

#include "frontend.h"

#define MC_SIZE_ASSERT(T, n) typedef char D1_CAT(mc_size_assert_, __LINE__)[(sizeof(T) == (n)) ? 1 : -1]
#define MC_OFS_ASSERT(T, f, o) typedef char D1_CAT(mc_ofs_assert_, __LINE__)[((int)&((T *)0)->f == (o)) ? 1 : -1]

/* ---- one save file on a card, as listed by func_000E7B2C (0x44 bytes) ---- */
typedef struct {
    char fname[0x18];               /* 0x00 directory name ("BESLES-..."), "Empty" when free */
    char title[0x24];               /* 0x18 ASCII title from the SJIS header (func_000E8E84) */
    int size;                       /* 0x3C bytes */
    unsigned char idx;              /* 0x40 first entry of this file */
    unsigned char n;                /* 0x41 blocks used */
    unsigned char f42;              /* 0x42 */
    unsigned char f43;              /* 0x43 slot in use */
} MC_FILE;
MC_SIZE_ASSERT(MC_FILE, 0x44);
MC_OFS_ASSERT(MC_FILE, size, 0x3C);
MC_OFS_ASSERT(MC_FILE, idx, 0x40);

/* ---- one card port's directory (D_000EE930[2], 0x404 bytes) ---- */
typedef struct {
    unsigned short status;          /* 0x00 1 = empty, 2 = ok, 4/8 = error flags */
    unsigned char free;             /* 0x02 free blocks (15 - used) */
    unsigned char changed;          /* 0x03 set by every write */
    char *msg;                      /* 0x04 status line shown under the port, NULL = none */
    MC_FILE e[15];                  /* 0x08 */
} MC_CARD;
MC_SIZE_ASSERT(MC_CARD, 0x404);
MC_OFS_ASSERT(MC_CARD, e, 0x08);

/* ---- bitmap font (D_000ED8B0[], 0x414 bytes): two glyph ranges on their own texture pages, the line height,
 * the letter spacing and one {u, v, w, h} cell per character ---- */
typedef struct {
    unsigned char lo, hi;           /* 0x0 character range */
    unsigned char u, v;             /* 0x2 */
    unsigned short tpage, clut;     /* 0x4 */
} MC_FONT_RANGE;
typedef struct {
    MC_FONT_RANGE r[2];             /* 0x000 */
    unsigned short n;               /* 0x010 line height */
    short val;                      /* 0x012 spacing */
    unsigned char col[256][4];      /* 0x014 u, v, w, h */
} MC_FONT;
MC_SIZE_ASSERT(MC_FONT, 0x414);

/* ---- palette/size block a font file starts with (loaded at 0xD0200) ---- */
typedef struct { short n; short pad; unsigned char col[256][4]; } MC_FONT_FILE;

/* ---- SJIS punctuation <-> ASCII (D_000ED7E8[33]): the second SJIS byte after 0x81 and the ASCII character.
 * func_000E8F68 reads the SJIS code as one little-endian halfword (0x81 in the high byte). ---- */
typedef struct {
    unsigned char k;                /* 0x0 SJIS second byte */
    unsigned char k_hi;             /* 0x1 0x81 */
    unsigned char v;                /* 0x2 ASCII */
    unsigned char v_hi;             /* 0x3 */
} MC_SJIS;
MC_SIZE_ASSERT(MC_SJIS, 4);

/* ---- SJIS code of the first character of '0'..'9', 'A'..'Z', 'a'..'z' (D_000ED7DC[3]) ---- */
typedef struct { unsigned short base; unsigned short first; } MC_SJIS_RANGE;

#endif
