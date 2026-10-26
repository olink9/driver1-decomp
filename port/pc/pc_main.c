/* Driver 1 PC port - process entry, emulated PS1 memory, small platform services.
 *
 * PS1 memory model (see port/README.md):
 *   pc_psxram   the 2 MB PS1 main RAM. It is an absolute linker symbol at address 0 (pc_syms.ld) and the
 *               pages 0x10000-0x1FFFFF are mapped here at startup, so PC_RAM(a) == physical PS1 address:
 *               hard-coded buffers (0xD0200, 0x1C0000 ...), the .bss symbols the linker places at their PS1
 *               addresses, and the 24-bit OT links (the prim layout is the PS1 one) all agree with no
 *               translation. 0x0-0xFFFF (PS1 kernel area) stays unmapped (vm.mmap_min_addr).
 *   pc_scratchpad  the 1 KB scratchpad (0x1F800000), a plain array (PC_SCRATCH).
 * The exe itself is linked at 4 MB (port/CMakeLists.txt), so its globals are below 16 MB too, and the game
 * runs on a stack inside the exe's .bss: primitives in game globals or on the stack can be linked into an OT.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <ucontext.h>
#include <limits.h>
#include <signal.h>
#include <execinfo.h>

#include "common.h"            /* PsyQ types (DISPENV, RECT16) */
#include "PsyX/PsyX_public.h"
#include <SDL.h>

extern unsigned char pc_psxram[];          /* = 0, port/pc/pc_syms.ld */
unsigned char pc_scratchpad[4096] __attribute__((aligned(16)));   /* PS1: 1 KB; the rest catches overruns */

extern void d1_main(void);                 /* src/main.c: the game's main() */
extern int PC_CdFileSize(const char *name);

static char s_datadir[PATH_MAX];
static unsigned char s_game_stack[1 << 20] __attribute__((aligned(16)));   /* below 16 MB, see above */
static ucontext_t s_main_ctx, s_game_ctx;

const char *PC_DataDir(void)
{
    return s_datadir;
}

void PC_Break(const char *file, int line)
{
    /* PS1: `break 1024` (a debugger trap) in an endless loop - the game's own assert */
    fprintf(stderr, "[driver1] game assert (PS1 break) at %s:%d\n", file, line);
    fflush(stdout);
    abort();
}

static void map_psxram(void)
{
    void *want = pc_psxram + 0x10000;
    void *p = mmap(want, 0x200000 - 0x10000, PROT_READ | PROT_WRITE,
                   MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED_NOREPLACE, -1, 0);
    if (p != want) {
        fprintf(stderr, "[driver1] can't map PS1 RAM at %p (got %p): is vm.mmap_min_addr above 0x10000?\n", want, p);
        exit(1);
    }
}

static int is_dir(const char *p)
{
    struct stat st;
    char idx[PATH_MAX];
    snprintf(idx, sizeof(idx), "%s/DISC.LBA", p);
    return stat(p, &st) == 0 && S_ISDIR(st.st_mode) && access(idx, R_OK) == 0;
}

static void find_datadir(int argc, char **argv)
{
    char exe[PATH_MAX];
    ssize_t n;
    const char *env = getenv("D1_DATA");
    if (env && is_dir(env)) {
        snprintf(s_datadir, sizeof(s_datadir), "%s", env);
        return;
    }
    if (argc > 1 && is_dir(argv[1])) {
        snprintf(s_datadir, sizeof(s_datadir), "%s", argv[1]);
        return;
    }
    if (is_dir("pcdata")) {
        snprintf(s_datadir, sizeof(s_datadir), "pcdata");
        return;
    }
    n = readlink("/proc/self/exe", exe, sizeof(exe) - 1);
    if (n > 0) {
        exe[n] = 0;
        *strrchr(exe, '/') = 0;
        snprintf(s_datadir, sizeof(s_datadir), "%s/pcdata", exe);
        if (is_dir(s_datadir))
            return;
    }
    fprintf(stderr, "[driver1] no game data: run port/pc/pc_extract.py, then start with D1_DATA=<dir>, "
                    "the directory as argument, or ./pcdata (port/README.md)\n");
    exit(1);
}

/* ---- PS1 images in PS1 RAM ----
 * The PS1 data the C files don't own (yet) is used where the PS1 had it: SLUS_008.42 is loaded at 0x10000 at
 * startup and an overlay at its address when the game loads it (its code bytes are dead weight: all code is
 * linked in). The words splat identified as pointers are then rewritten to PC symbols (pc_fixups.s from
 * pc_gendata.py): function pointers in data tables and pointers to C-owned objects. */
typedef struct { unsigned int addr, value; } Fixup;
extern const unsigned int pc_fixups_main[], pc_fixups_frontend[], pc_fixups_mcard[], pc_fixups_cred[],
    pc_fixups_iview[];

static void apply_fixups(const unsigned int *t)
{
    const Fixup *f = (const Fixup *)(t + 1);
    unsigned int i;
    for (i = 0; i < t[0]; i++)
        *(unsigned int *)(pc_psxram + f[i].addr) = f[i].value;
}

static void load_main_image(void)
{
    char path[PATH_MAX];
    unsigned int hdr[8];
    FILE *f;
    snprintf(path, sizeof(path), "%s/SLUS_008.42", s_datadir);
    f = fopen(path, "rb");
    if (!f || fread(hdr, 1, sizeof(hdr), f) != sizeof(hdr) || memcmp(hdr, "PS-X EXE", 8)) {
        fprintf(stderr, "[driver1] %s missing or not a PS-X EXE\n", path);
        exit(1);
    }
    /* header: +0x18 text address, +0x1C text size; the image starts at file offset 0x800 */
    fseek(f, 0x800, SEEK_SET);
    if (hdr[6] != 0x10000 || fread(pc_psxram + hdr[6], 1, hdr[7], f) != hdr[7]) {
        fprintf(stderr, "[driver1] %s: unexpected layout (text 0x%x size 0x%x)\n", path, hdr[6], hdr[7]);
        exit(1);
    }
    fclose(f);
    apply_fixups(pc_fixups_main);
}

/* Loadfile() hook. The overlays load for real (their data is used in place, see above), then get their
 * pointer fixups; NFMV.EXE (the FMV player, 0x800FF800) is not loaded - the FMV is skipped (pc_psyq.c Exec).
 * Returns 1 when it handled the file (*size = bytes), 0 for an ordinary file. */
int PC_OverlayLoad(const char *name, int addr, int *size)
{
    static const struct { const char *name; const unsigned int *fix; } ov[] = {
        { "FRONTEND.BIN", pc_fixups_frontend }, { "MCARD.BIN", pc_fixups_mcard },
        { "CRED.BIN", pc_fixups_cred }, { "IVIEW.BIN", pc_fixups_iview }, { "NFMV\\NFMV.EXE", NULL },
    };
    extern int CdReadFile(char *file, unsigned long *addr, int nbyte);
    char path[64];
    unsigned i;
    for (i = 0; i < sizeof(ov) / sizeof(ov[0]); i++) {
        if (strcasecmp(name, ov[i].name))
            continue;
        snprintf(path, sizeof(path), "\\DRIVER\\%s;1", name);
        if (!ov[i].fix) {
            *size = PC_CdFileSize(path);
            printf("[driver1] %s not loaded (FMV player not ported)\n", name);
            return 1;
        }
        *size = CdReadFile(path, (unsigned long *)(pc_psxram + (addr & 0x1FFFFF)), 0);
        apply_fixups(ov[i].fix);
        return 1;
    }
    return 0;
}

/* VSync() is linked with --wrap (port/CMakeLists.txt): the vblank is where the PS1 game would have taken the
 * CD and MDEC interrupts its waits depend on, so pending streams/transfers complete here. */
extern int __real_VSync(int mode);
extern void PC_CdService(void);
extern void PC_DctService(void);

/* The PS1 shows a frame by itself; PsyCross needs PsyX_EndScene() to present what was drawn (REDRIVER2 calls it
 * from its game code). Driver's frames all end in VSync(), so a drawn scene is presented there. */
extern char begin_scene_flag;                  /* PsyX_main.cpp */
extern DISPENV activeDispEnv;                  /* LIBGPU.C */
extern void PsyX_EndScene(void);
extern int StoreImage(RECT16 *rect, u_long *p);

/* debugging aids (environment): D1_SHOT=n1,n2,.. writes the displayed VRAM area at those vblank counts to
 * shot_<n>.ppm; D1_QUIT=n exits after n vblanks */
static void debug_hooks(int vbl)
{
    static const char *shots = (const char *)1;
    static int quit = -1;
    if (shots == (const char *)1) {
        const char *q = getenv("D1_QUIT");
        shots = getenv("D1_SHOT");
        quit = q ? atoi(q) : -1;
    }
    if (shots) {
        const char *p = shots;
        while (p && *p) {
            if (atoi(p) == vbl) {
                RECT16 r = activeDispEnv.disp;
                int rgb24 = activeDispEnv.isrgb24, w = rgb24 ? r.w * 2 / 3 : r.w;
                unsigned short *px;
                char name[64];
                FILE *f;
                int i;
                if (rgb24)
                    r.w = (r.w * 3 / 2 + 1) & ~1;   /* 24-bit display: w pixels take w*3/2 VRAM halfwords */
                px = malloc(r.w * r.h * 2);
                StoreImage(&r, (u_long *)px);
                snprintf(name, sizeof(name), "shot_%d.ppm", vbl);
                f = fopen(name, "wb");
                fprintf(f, "P6 %d %d 255\n", rgb24 ? r.w * 2 / 3 : r.w, r.h);
                for (i = 0; i < r.h; i++) {
                    if (rgb24) {
                        fwrite((unsigned char *)(px + i * r.w), 1, r.w * 2 / 3 * 3, f);
                    } else {
                        int x;
                        for (x = 0; x < r.w; x++) {
                            unsigned short c = px[i * r.w + x];
                            unsigned char rgb[3] = { (c & 31) << 3, (c >> 5 & 31) << 3, (c >> 10 & 31) << 3 };
                            fwrite(rgb, 1, 3, f);
                        }
                    }
                }
                (void)w;
                fclose(f);
                free(px);
                printf("[driver1] vblank %d: %s (%dx%d at %d,%d)\n", vbl, name, r.w, r.h, r.x, r.y);
            }
            p = strchr(p, ',');
            p = p ? p + 1 : NULL;
        }
    }
    if (quit >= 0 && vbl >= quit) {
        printf("[driver1] D1_QUIT: exit at vblank %d\n", vbl);
        fflush(NULL);
        _exit(0);           /* skip PsyX_Shutdown (atexit): tearing SDL down from the game context crashes */
    }
}

/* D1_SHOT also grabs the window (what the player sees) just before it is presented: gl_<n>.ppm */
static void grab_window(int vbl)
{
    typedef void (*ReadPixels)(int, int, int, int, unsigned, unsigned, void *);
    static ReadPixels rp;
    const char *shots = getenv("D1_SHOT"), *p = shots;
    while (p && *p) {
        if (atoi(p) == vbl) {
            int w = 640, h = 480, y;
            unsigned char *px = malloc(w * h * 3);
            char name[64];
            FILE *f;
            if (!rp)
                rp = (ReadPixels)SDL_GL_GetProcAddress("glReadPixels");
            rp(0, 0, w, h, 0x1907 /* GL_RGB */, 0x1401 /* GL_UNSIGNED_BYTE */, px);
            snprintf(name, sizeof(name), "gl_%d.ppm", vbl);
            f = fopen(name, "wb");
            fprintf(f, "P6 %d %d 255\n", w, h);
            for (y = h - 1; y >= 0; y--)
                fwrite(px + y * w * 3, 1, w * 3, f);
            fclose(f);
            free(px);
        }
        p = strchr(p, ',');
        p = p ? p + 1 : NULL;
    }
}

int __wrap_VSync(int mode)
{
    static int frames;
    int r;
    PC_CdService();
    PC_DctService();
    if (mode >= 0 && begin_scene_flag) {
        grab_window(frames + 1);
        PsyX_EndScene();
    }
    r = __real_VSync(mode);
    if (mode >= 0)
        debug_hooks(++frames);
    return r;
}

/* crash report: the faulting address and a backtrace (addr2line -e driver1 <addr> resolves the frames) */
static void on_crash(int sig, siginfo_t *si, void *uc)
{
    void *bt[48];
    int n = backtrace(bt, 48);
    fprintf(stderr, "[driver1] signal %d, fault address %p. backtrace:\n", sig, si->si_addr);
    backtrace_symbols_fd(bt, n, 2);
    signal(sig, SIG_DFL);
    raise(sig);
}

static void install_crash_handler(void)
{
    static unsigned char altstack[65536];
    stack_t ss = { altstack, 0, sizeof(altstack) };
    struct sigaction sa;
    sigaltstack(&ss, NULL);
    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = on_crash;
    sa.sa_flags = SA_SIGINFO | SA_ONSTACK;
    sigaction(SIGSEGV, &sa, NULL);
    sigaction(SIGBUS, &sa, NULL);
    sigaction(SIGFPE, &sa, NULL);
    sigaction(SIGILL, &sa, NULL);
}

static void game_entry(void)
{
    d1_main();
    fprintf(stderr, "[driver1] game main() returned\n");
    exit(0);
}

int main(int argc, char **argv)
{
    map_psxram();
    find_datadir(argc, argv);
    setvbuf(stdout, NULL, _IOLBF, 0);
    printf("[driver1] data: %s\n", s_datadir);
    load_main_image();

    PsyX_Initialise("Driver", 640, 480, 0);
    install_crash_handler();

    getcontext(&s_game_ctx);
    s_game_ctx.uc_stack.ss_sp = s_game_stack;
    s_game_ctx.uc_stack.ss_size = sizeof(s_game_stack);
    s_game_ctx.uc_link = &s_main_ctx;
    makecontext(&s_game_ctx, game_entry, 0);
    swapcontext(&s_main_ctx, &s_game_ctx);
    return 0;
}
