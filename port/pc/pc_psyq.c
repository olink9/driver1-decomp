/* Driver 1 PC port - PsyQ library functions PsyCross does not provide (or that need PC behaviour).
 *
 * The game calls most of its PsyQ library by address (func_0007xxxx: the library was identified, not named);
 * port/pc/pc_gendata.py maps the ones PsyCross implements onto the PsyQ names (PSYQ_ALIAS). These are the rest,
 * defined under the game's func_ names with the PsyQ name in the comment; how each was identified is in
 * port/README.md. Stubs log once. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"      /* the game's PsyQ view (types, libgte, libgpu) */

#define ONCE(msg) do { static int done_; if (!done_++) printf("[psyq] %s\n", msg); } while (0)

/* ---- BIOS / libapi ---- */
static unsigned int s_rand_seed;   /* PC-PORT: unverified - the BIOS's initial seed; Driver calls srand first? */

/* rand: BIOS A(2Fh) - x = x * 0x41C64E6D + 0x3039, returns (x >> 16) & 0x7FFF (psx-spx) */
int func_000776E4(void)
{
    s_rand_seed = s_rand_seed * 0x41C64E6D + 0x3039;
    return (s_rand_seed >> 16) & 0x7FFF;
}

/* srand: BIOS A(30h) */
void func_00077D7C(unsigned int seed)
{
    s_rand_seed = seed;
}

/* BIOS A0 libc calls: same semantics as glibc */
void *func_00079DB0(void *d, const void *s, int n) { return memcpy(d, s, n); }      /* A(2Ah) memcpy */
int func_0007B218(const char *a, const char *b, int n) { return strncmp(a, b, n); } /* A(18h) strncmp */
int func_0007B228(const char *a, const char *b) { return strcmp(a, b); }          /* A(17h) strcmp */
char *func_0007E8EC(char *d, const char *s) { return strcat(d, s); }              /* A(15h) strcat */
int func_0007EC88(const char *s) { return strlen(s); }                            /* A(1Bh) strlen */
char *func_0007EC98(char *d, const char *s, int n) { return strncpy(d, s, n); }   /* A(1Ah) strncpy */

/* GetSp / SetSp (libapi): Driver points the stack at the scratchpad around hot code (cars.c, main.c, ladder.c).
 * On PC the stack stays where it is: GetSp returns a token, SetSp ignores it. */
int func_00079AE0(void)
{
    return 0;
}

int func_00079AF0(int sp)
{
    (void)sp;
    return 0;
}

/* EnterCriticalSection / ExitCriticalSection (syscall 1 / 2): interrupts are PsyCross threads; nothing to mask */
int func_0007EC78(void)
{
    return 1;
}

void func_0007EB34(void)
{
}

/* FlushCache: BIOS A(44h) */
void func_0007EBE8(void)
{
}

/* Exec: BIOS A(43h) - only fmvplay.c uses it, to run NFMV.EXE (the FMV player, not ported yet) */
int func_0007EB24(void *exec, int argc, char **argv)
{
    ONCE("Exec(NFMV.EXE): FMV player not ported yet, movie skipped");
    return 1;
}

/* B(38h) per the PsyQ signature (exit) - sound.c calls it when SpuMalloc fails */
void func_0007C744(int code)
{
    fprintf(stderr, "[psyq] exit(%d) from the game\n", code);
    abort();
}

/* ---- libgpu ---- */
/* SetDrawStp(DR_STP *p, int pbw): 2-word packet, GP0(E6h) mask-bit setting (asm: len 2, E6000000|pbw, 0) */
void func_00074D2C(u_int *p, int pbw)
{
    ((u_char *)p)[3] = 2;
    p[1] = pbw ? 0xE6000001 : 0xE6000000;
    p[2] = 0;
}

void SetLineF2(LINE_F2 *p)
{
    setLineF2(p);
}

void SetLineF4(LINE_F4 *p)
{
    setLineF4(p);
}

/* ---- libsnd ---- */
extern void PC_XaSetVolume(int left, int right); /* pc_xa.c */
void SsSetSerialVol(short s_num, short voll, short volr)
{
    if (s_num == 0)
        PC_XaSetVolume(voll, volr);
}

/* ---- libpress (MDEC) - used by CRED.BIN for the credits portraits. PC-PORT: no MDEC decoder yet: the
 * calls keep their control flow (the out-callback chain runs, so nothing waits forever), the image stays
 * whatever was in the buffer. ---- */
static void (*s_dct_cb)(void);
static int s_dct_busy, s_dct_pending;

/* DecDCTReset(int mode) */
void func_00073AC4(int mode)
{
}

/* DecDCTvlcBuild(DECDCTTAB table) */
void DecDCTvlcBuild(void *table)
{
}

/* DecDCTvlc(u_long *bs, u_long *buf, DECDCTTAB table) */
int func_000768E4(void *bs, void *buf, void *table)
{
    ONCE("DecDCTvlc: MDEC not emulated (credits portraits stay blank)");
    return 0;
}

/* DecDCTin(u_long *buf, int mode) */
void func_00073AF8(void *buf, int mode)
{
}

/* DecDCTout(u_long *buf, int size): the "DMA" completes at the next VSync (PC_DctService), where the callback
 * runs and may queue the next slice - as on PS1 the caller sets its busy flag after this returns */
void func_00073B74(void *buf, int size)
{
    s_dct_pending = 1;
}

void PC_DctService(void)
{
    int guard = 0;
    if (s_dct_busy)
        return;
    s_dct_busy = 1;
    while (s_dct_pending && guard++ < 64) {
        s_dct_pending = 0;
        if (s_dct_cb)
            s_dct_cb();
    }
    s_dct_busy = 0;
}

/* DecDCToutCallback(void (*func)()) */
void func_00073B94(void (*func)(void))
{
    s_dct_cb = func;
}

/* ---- libmcrd: port/pc/pc_mcard.c (a raw .mcd card image) ---- */
