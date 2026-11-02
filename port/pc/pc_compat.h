/* Driver 1 PC port - force-included (-include) into every game file of the PC build.
 * The PS1 build never sees this file: everything PC-only in src/ sits behind #ifdef PSX_PC,
 * so `make` stays byte-identical. */
#ifndef PC_COMPAT_H
#define PC_COMPAT_H
#define PSX_PC 1
#define INCLUDE_ASM(FOLDER, NAME)
#define INCLUDE_RODATA(FOLDER, NAME)

/* common.h typedefs this too; gnu11 allows the repeat. gte.h (PsyCross inline_c.h) needs it in files without common.h */
typedef unsigned int uint;

/* OpenGL entry points fetched with SDL_GL_GetProcAddress are __stdcall on 32-bit Windows */
#ifdef _WIN32
#define PC_GLAPI __stdcall
#else
#define PC_GLAPI
#endif

/* Emulated PS1 memory (A's audit, pilot/PORT1.md): the 1 KB scratchpad at 0x1F800000 and the 2 MB main RAM
 * (offset = physical address) are plain arrays, so hard-coded PS1 addresses and the 24-bit OT links stay valid.
 * Per-file copies in src/ are #ifndef PC_SCRATCH / PC_RAM and step aside for these. */
extern unsigned char pc_scratchpad[];
extern unsigned char pc_psxram[];
#define PC_SCRATCH(a) ((int)(pc_scratchpad + ((a) - 0x1F800000)))
#define PC_RAM(a) ((int)(pc_psxram + ((unsigned int)(a) & 0x1FFFFF)))

/* A PS1 busy-wait on a flag the CD/MDEC interrupt sets (`while (flag) ;`) calls this on PC: it runs the pending
 * CD stream / MDEC callbacks there (port/pc/pc_main.c), which is where the interrupt would have come. */
extern void PC_WaitService(void);
#endif
