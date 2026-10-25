/* Driver 1 decomp - FRONTEND overlay shared records (Step 3 for the overlays).
 *
 * Types only (no globals, no prototypes), shared by the 23 src/frontend/ TUs. The records the main exe
 * also uses come from structs.h (MENU_ITEM, DB, VECTOR/SVECTOR/MATRIX/CVECTOR); this adds the menu
 * label record, the full draw buffer, the texture-page table and the libgpu primitive layouts.
 *
 *  - Include after any "common.h". Without common.h, the libgpu subset below is defined here with the
 *    real PSX layout. With common.h (PsyCross) those types come from libgpu.h - but PsyCross's DRAWENV is
 *    0x60 bytes (an extra `drt` byte), not the real 0x5C, so DB_ENV is only defined without common.h.
 *  - Plain char is unsigned (-funsigned-char).
 */
#ifndef D1_FRONTEND_H
#define D1_FRONTEND_H

#include "structs.h"

/* structs.h's check, own name prefix (its names use __LINE__, which collides across headers) */
#define FE_SIZE_ASSERT(T, n) typedef char D1_CAT(fe_size_assert_, __LINE__)[(sizeof(T) == (n)) ? 1 : -1]

/* ---- libgpu subset (PsyQ libgpu.h layout, 32-bit tag) ---- */
#ifndef COMMON_H
typedef struct { short x, y, w, h; } RECT;
typedef RECT RECT16;
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0, x1, y1, x2, y2, x3, y3;
} POLY_F4;
typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short x1, y1;
    unsigned char u1, v1;
    unsigned short tpage;
    short x2, y2;
    unsigned char u2, v2;
    unsigned short pad1;
} POLY_FT3;
typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short x1, y1;
    unsigned char u1, v1;
    unsigned short tpage;
    short x2, y2;
    unsigned char u2, v2;
    unsigned short pad1;
    short x3, y3;
    unsigned char u3, v3;
    unsigned short pad2;
} POLY_FT4;
typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char r1, g1, b1, p1;
    short x1, y1;
    unsigned char r2, g2, b2, p2;
    short x2, y2;
    unsigned char r3, g3, b3, p3;
    short x3, y3;
} POLY_G4;
typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    unsigned char r1, g1, b1, p1;
    short x1, y1;
    unsigned char u1, v1;
    unsigned short tpage;
    unsigned char r2, g2, b2, p2;
    short x2, y2;
    unsigned char u2, v2;
    unsigned short pad2;
    unsigned char r3, g3, b3, p3;
    short x3, y3;
    unsigned char u3, v3;
    unsigned short pad3;
} POLY_GT4;
typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short w, h;
} SPRT;
typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    short x1, y1;
} LINE_F2;
typedef struct {
    RECT clip;
    short ofs[2];
    RECT tw;
    unsigned short tpage;
    unsigned char dtd, dfe, isbg, r0, g0, b0;
    unsigned long dr_env[16];
} DRAWENV;
FE_SIZE_ASSERT(DRAWENV, 0x5C);
typedef struct {
    RECT disp;
    RECT screen;
    unsigned char isinter, isrgb24, pad0, pad1;
} DISPENV;
FE_SIZE_ASSERT(DISPENV, 0x14);

/* ---- draw buffer with its environments (D2 DB in full, 0x80 bytes; D_000BDFA4[2]). `current` and
 * D_00097524 point at one; TUs keep them as structs.h DB * and cast where the envs are touched. ---- */
typedef struct {
    char *primptr;                  /* 0x00 */
    unsigned long *ot;              /* 0x04 */
    char *primtab;                  /* 0x08 */
    int id;                         /* 0x0C */
    DRAWENV draw;                   /* 0x10 */
    DISPENV disp;                   /* 0x6C */
} DB_ENV;
FE_SIZE_ASSERT(DB_ENV, 0x80);
#endif

/* ---- a word seen as its P_TAG bit fields plus the primitive's colour/code word ---- */
typedef struct { unsigned addr:24; unsigned len:8; unsigned char r0, g0, b0, code; } P_TAG2;

/* ---- menu label (the table func_001C2114/func_001C1BD4 draws, 16 bytes; parallel to a MENU_ITEM table).
 * The list ends at a NULL text. `font` is the highlight: a screen's enter/leave handlers set it to 1/0. ---- */
typedef struct {
    short x, y;                     /* 0x0 */
    unsigned char font;             /* 0x4 func_000E6F14 arg 3 */
    unsigned char justify;          /* 0x5 func_000E6F14 arg 4 */
    short f6;                       /* 0x6 */
    char *text;                     /* 0x8 */
    CVECTOR col;                    /* 0xC */
} MENU_TEXT;
FE_SIZE_ASSERT(MENU_TEXT, 16);

/* ---- a screen title (func_001C2114's first argument; the screens' enter handlers set text/x/y) ---- */
typedef struct {
    short x, y;                     /* 0x0 */
    char *text;                     /* 0x4 */
} MENU_TITLE;

/* ---- a texture found by name (func_001C334C): D2 TEXTURE_DETAILS + the sheet's entry/slot ---- */
typedef struct {
    unsigned char u0, v0, u1, v1, u2, v2, u3, v3; /* 0x0 */
    unsigned short tpage;           /* 0x8 */
    unsigned short clut;            /* 0xA */
} TEX;
typedef struct {
    unsigned char u0, v0, u1, v1, u2, v2, u3, v3; /* 0x0 */
    unsigned short tpage;           /* 0x8 */
    unsigned short clut;            /* 0xA */
    unsigned char idx, slot;        /* 0xC */
} TEXINFO;

/* ---- frnt\tpdata.dat: 32 texture sheets of up to 10 named images (D_001CFDF0) ---- */
typedef struct { char name[16]; short clut; short pad; int x, y, w, h; } TP_ENTRY;
FE_SIZE_ASSERT(TP_ENTRY, 0x24);
typedef struct {
    int used;                       /* 0x00 */
    unsigned char slot;             /* 0x04 */
    char pad;                       /* 0x05 */
    short tpage;                    /* 0x06 */
    int count;                      /* 0x08 */
    TP_ENTRY e[10];                 /* 0x0C */
} TP_SHEET;
FE_SIZE_ASSERT(TP_SHEET, 0x174);

#endif
