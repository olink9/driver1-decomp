/* Driver 1 decomp - NFMV (the separate FMV player program) shared records (Step 3 for the overlays).
 *
 * Types only. Include after "common.h" (PsyCross: RECT16, DISPENV, DR_ENV, u_long...).
 * PsyCross's DRAWENV is 0x60 bytes (an extra `drt` byte); the real PSX one is 0x5C, so the draw buffer
 * uses DRAWENV_PSX with the real layout.
 */
#ifndef D1_NFMV_H
#define D1_NFMV_H

typedef struct {
    RECT16 clip;
    short ofs[2];
    RECT16 tw;
    u_short tpage;
    u_char dtd, dfe, isbg, r0, g0, b0;
    DR_ENV dr_env;
} DRAWENV_PSX;

/* double buffer D_8012DB48[2] */
typedef struct {
    DRAWENV_PSX draw;
    DISPENV disp;
} DB;

/* one movie of the play list (D_80112868) */
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

/* movie decoder state D_801268A8 (PsyQ DECENV-like). Scalars are volatile (the DCT/CD callbacks change them),
 * the RECT16 members are not. */
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

#endif
