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
#include <sys/stat.h>
#include <limits.h>
#include <signal.h>
#include <pthread.h>
#ifndef _WIN32
#include <sys/mman.h>
#include <ucontext.h>
#include <execinfo.h>
#endif

#include "common.h"            /* PsyQ types (DISPENV, RECT16) */
#include "PsyX/PsyX_public.h"
#include <SDL.h>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>        /* after the PsyQ headers: it #defines OpenEvent */
#endif

extern unsigned char pc_psxram[];          /* = 0, port/pc/pc_syms.ld */
unsigned char pc_scratchpad[4096] __attribute__((aligned(16)));   /* PS1: 1 KB; the rest catches overruns */
/* stands in for PS1 kernel RAM 0x0-0xFFFF where the game dereferences NULL (the PS1 just reads/writes the kernel's
 * garbage area there; on PC page 0 can't be mapped): used explicitly at those sites (iview.c) */
unsigned char pc_lowram[0x10000] __attribute__((aligned(16)));

extern void d1_main(void);                 /* src/main.c: the game's main() */
extern int PC_CdFileSize(const char *name);

static char s_datadir[PATH_MAX];
static unsigned char s_game_stack[1 << 20] __attribute__((aligned(16)));   /* below 16 MB, see above */
#ifdef _WIN32
/* Windows: the kernel fills the low address space with its own mappings before main() runs, so PS1 RAM is part of
 * the exe image instead (port/CMakeLists.txt): image base 0x10000, then this uninitialised section from 0x11000 up
 * to 0x200000, the program above. The first 0x400 bytes at 0x10000 stay the PE headers (the loader reads them
 * whenever a DLL loads); the PS1 bytes of 0x10000-0x103FF live in pc_hdrshadow (pc_gendata.py --pe resolves names
 * there), the rest of that page is made writable. */
extern unsigned char pc_psxram_section[];
unsigned char pc_hdrshadow[0x400] __attribute__((aligned(16)));
#define PC_HDR_END 0x10400
#else
static ucontext_t s_main_ctx, s_game_ctx;
#endif

/* where a PS1 image byte lives (only differs on Windows, for the PE header bytes) */
static unsigned char *img_byte(unsigned int addr)
{
#ifdef _WIN32
    if (addr - 0x10000u < PC_HDR_END - 0x10000u)
        return pc_hdrshadow + (addr - 0x10000);
#endif
    return pc_psxram + addr;
}

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
#ifdef _WIN32
    DWORD old;
    if (pc_psxram_section != pc_psxram + 0x11000 ||
        !VirtualProtect(pc_psxram + 0x10000, 0x1000, PAGE_READWRITE, &old)) {
        fprintf(stderr, "[driver1] PS1 RAM is not where the build put it (%p, expected %p): built without "
                        "port/CMakeLists.txt's Windows link options?\n", (void *)pc_psxram_section,
                (void *)(pc_psxram + 0x11000));
        exit(1);
    }
#else
    void *want = pc_psxram + 0x10000;
    void *p = mmap(want, 0x200000 - 0x10000, PROT_READ | PROT_WRITE,
                   MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED_NOREPLACE, -1, 0);
    if (p != want) {
        fprintf(stderr, "[driver1] can't map PS1 RAM at %p (got %p): is vm.mmap_min_addr above 0x10000?\n", want, p);
        exit(1);
    }
#endif
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
#ifdef _WIN32
    n = GetModuleFileNameA(NULL, exe, sizeof(exe) - 1);
    if (n > 0 && strrchr(exe, '\\')) {
        exe[n] = 0;
        *strrchr(exe, '\\') = 0;
#else
    n = readlink("/proc/self/exe", exe, sizeof(exe) - 1);
    if (n > 0) {
        exe[n] = 0;
        *strrchr(exe, '/') = 0;
#endif
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
        *(unsigned int *)img_byte(f[i].addr) = f[i].value;
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
#ifdef _WIN32
    if (hdr[6] != 0x10000 || fread(pc_hdrshadow, 1, sizeof(pc_hdrshadow), f) != sizeof(pc_hdrshadow) ||
        fread(pc_psxram + PC_HDR_END, 1, hdr[7] - sizeof(pc_hdrshadow), f) != hdr[7] - sizeof(pc_hdrshadow)) {
#else
    if (hdr[6] != 0x10000 || fread(pc_psxram + hdr[6], 1, hdr[7], f) != hdr[7]) {
#endif
        fprintf(stderr, "[driver1] %s: unexpected layout (text 0x%x size 0x%x)\n", path, hdr[6], hdr[7]);
        exit(1);
    }
    fclose(f);
    apply_fixups(pc_fixups_main);
}

/* Loadfile() hook. The overlays load for real (their data is used in place, see above), then get their
 * pointer fixups; NFMV.EXE (the FMV player, 0x800FF800) is not loaded - pc_psyq.c's Exec runs NFMV in-process
 * (pc_fmv.c).
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
            printf("[driver1] %s not loaded (NFMV runs in-process, port/pc/pc_fmv.c)\n", name);
            return 1;
        }
        *size = CdReadFile(path, (unsigned long *)(pc_psxram + (addr & 0x1FFFFF)), 0);
        apply_fixups(ov[i].fix);
        if (ov[i].fix == pc_fixups_frontend) {
            /* FRONTEND.BIN .data 0x1CF308-0x1CF327: ladview.c's eight pointers into the scratchpad (D_001CF308
             * plot table end .. D_001CF324, values 0x1F800010-0x1F800264, the only scratchpad addresses in any
             * loaded image's data): point them at pc_scratchpad (ladder/apartment 3D view, func_001CC894). */
            unsigned int *w = (unsigned int *)(pc_psxram + 0x1CF308);
            int k;
            for (k = 0; k < 8; k++)
                if (w[k] - 0x1F800000u < 0x400u)
                    w[k] = (unsigned int)(pc_scratchpad + (w[k] - 0x1F800000u));
        }
        return 1;
    }
    return 0;
}

/* VSync() is linked with --wrap (port/CMakeLists.txt): the vblank is where the PS1 game would have taken the
 * CD and MDEC interrupts its waits depend on, so pending streams/transfers complete here. */
extern int __real_VSync(int mode);
extern void PC_CdService(void);
extern void PC_DctService(void);
extern void PC_TraceVSync(void *caller);

/* The PS1 shows a frame by itself; PsyCross needs PsyX_EndScene() to present what was drawn (REDRIVER2 calls it
 * from its game code). Driver's frames all end in VSync(), so a drawn scene is presented there. */
extern char begin_scene_flag;                  /* PsyX_main.cpp */
extern DISPENV activeDispEnv;                  /* LIBGPU.C */
extern void PsyX_EndScene(void);
extern int StoreImage(RECT16 *rect, u_long *p);

/* debugging aids (environment): D1_SHOT=n1,n2,.. writes the displayed VRAM area (shot_<n>.ppm) and the window
 * (gl_<n>.ppm, grabbed just before a frame is presented) at those vblank counts; D1_QUIT=n exits after n vblanks.
 * Counts are real vblanks (PsyCross's 60 Hz counter), so they mean the same in the frontend (VSync(0) per frame)
 * and in game (the loop polls VSync(-1)). */
static const char *s_shots;
static int s_quit = -1, s_shot_last;   /* s_shot_last: shots up to this count are done */

static int shot_due(int vbl)           /* the first listed count in (s_shot_last, vbl], or 0 */
{
    const char *p = s_shots;
    int best = 0;
    while (p && *p) {
        int n = atoi(p);
        if (n > s_shot_last && n <= vbl && (!best || n < best))
            best = n;
        p = strchr(p, ',');
        p = p ? p + 1 : NULL;
    }
    return best;
}

static void shot_vram(int n)
{
    RECT16 r = activeDispEnv.disp;
    int rgb24 = activeDispEnv.isrgb24;
    unsigned short *px;
    char name[64];
    FILE *f;
    int i;
    if (rgb24)
        r.w = (r.w * 3 / 2 + 1) & ~1;   /* 24-bit display: w pixels take w*3/2 VRAM halfwords */
    px = malloc(r.w * r.h * 2);
    StoreImage(&r, (u_long *)px);
    snprintf(name, sizeof(name), "shot_%d.ppm", n);
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
    fclose(f);
    free(px);
    printf("[driver1] vblank %d: %s (%dx%d at %d,%d)\n", n, name, r.w, r.h, r.x, r.y);
}

static void grab_window(int n)
{
    typedef void (PC_GLAPI *ReadPixels)(int, int, int, int, unsigned, unsigned, void *);
    static ReadPixels rp;
    int w = 640, h = 480, y;
    unsigned char *px = malloc(w * h * 3);
    char name[64];
    FILE *f;
    if (!rp)
        rp = (ReadPixels)SDL_GL_GetProcAddress("glReadPixels");
    rp(0, 0, w, h, 0x1907 /* GL_RGB */, 0x1401 /* GL_UNSIGNED_BYTE */, px);
    snprintf(name, sizeof(name), "gl_%d.ppm", n);
    f = fopen(name, "wb");
    fprintf(f, "P6 %d %d 255\n", w, h);
    for (y = h - 1; y >= 0; y--)
        fwrite(px + y * w * 3, 1, w * 3, f);
    fclose(f);
    free(px);
}

static void debug_hooks(int vbl, int presenting)
{
    static int init;
    int n;
    if (!init) {
        const char *q = getenv("D1_QUIT");
        init = 1;
        s_shots = getenv("D1_SHOT");
        s_quit = q ? atoi(q) : -1;
    }
    /* wait up to 4 vblanks for a frame to be presented, so the window grab shows that count's frame */
    if (s_shots && (n = shot_due(vbl)) != 0 && (presenting || vbl >= n + 4)) {
        if (presenting)
            grab_window(n);
        shot_vram(n);
        s_shot_last = n;
    }
    if (s_quit >= 0 && vbl >= s_quit) {
        printf("[driver1] D1_QUIT: exit at vblank %d\n", vbl);
        fflush(NULL);
        exit(0);            /* PsyX_Shutdown (atexit) is safe from the game context now */
    }
}

/* The game thread is the process's main thread (the game runs on a ucontext there). PsyCross runs the PS1
 * VSyncCallback on its own interrupt thread, and Driver's callback calls VSync(1) itself (xmplay.c timing):
 * that must stay a plain counter read - presenting, CD/MDEC service or the debug hooks there race the game. */
static pthread_t s_game_thread;

int __wrap_VSync(int mode)
{
    static int last_poll = -1;
    int r, now, presenting;
    if (!pthread_equal(pthread_self(), s_game_thread))
        return __real_VSync(mode);
    PC_TraceVSync(__builtin_return_address(0));   /* lockstep trace (pc_trace.c, D1_TRACE) */
    PC_CdService();
    PC_DctService();
    /* present what was drawn since the last frame: the frontend ends each frame with VSync(0), the in-game loop
     * (main.c func_0002360C) with DrawOTag then a VSync(-1) poll until 2 vblanks have passed */
    presenting = begin_scene_flag != 0;
    if (presenting)
        PsyX_WaitPresentVBlank();   /* present on the vblank after the frame was drawn, as a PS1 flip shows */
    now = __real_VSync(-1);
    if (presenting) {
        debug_hooks(now, 1);
        PsyX_EndScene();
    }
    r = __real_VSync(mode);
    if (mode < 0) {
        /* a busy-wait poll: sleep until the next vblank (at most 4 ms) while the vblank count stands still */
        if (r == last_poll)
            PsyX_Sys_WaitVBlankChange(r, 4);
        last_poll = r;
    }
    debug_hooks(r, 0);
    return r;
}

/* The career ladder's 3D apartment screens (frontend ladconfirm.c x3, ladoptions.c: "Get A New Job", the job
 * confirm, options) loop on DrawSync + func_000513D4 (buffer flip) with no VSync at all: on the PS1 the flip's
 * PutDispEnv shows the frame and the loop runs GPU-bound. Here nothing would be presented (the screen froze, and
 * D1_QUIT/D1_SHOT never fired). PC: the same pacing as ladder.c's own loop (func_001C9048: poll VSync(-1) until 2
 * vblanks have passed), whose polls present the frame. */
void PC_LadderFrame(void)
{
    static int last;
    while ((unsigned)(VSync(-1) - last) < 2)
        ;
    last = VSync(-1);
}

/* pc_compat.h: a game busy-wait on an interrupt-set flag (spool.c) - the CD/MDEC work it waits for runs here */
void PC_WaitService(void)
{
    PC_CdService();
    PC_DctService();
}

/* ---- pads ----
 * PsyCross fills the PadInitDirect buffers in PsyX_UpdateInput(), which its own code only calls in _DEBUG builds
 * (REDRIVER2 calls it from its pad reader), so the game's pad read (pad.c func_000357F0) calls PC_PadRead first.
 * D1_PAD=<script> replaces pad 1 with scripted presses (headless testing; real input is ignored then):
 *   entries "FROM[-TO]=BTN[+BTN..]" separated by commas, FROM/TO in vblanks (the D1_SHOT clock), a lone FROM
 *   holds for 6 vblanks; BTN: up down left right x o tri sq l1 l2 r1 r2 start select.
 *   e.g. D1_PAD=4100=start,4300=x,5000-9000=x  */
extern void PsyX_UpdateInput(void);

static unsigned pad_bit(const char *b, int n)
{
    static const struct { const char *name; unsigned bit; } t[] = {   /* the PADRAW buttons word, active low */
        { "select", 0x1 }, { "l3", 0x2 }, { "r3", 0x4 }, { "start", 0x8 }, { "up", 0x10 }, { "right", 0x20 },
        { "down", 0x40 }, { "left", 0x80 }, { "l2", 0x100 }, { "r2", 0x200 }, { "l1", 0x400 }, { "r1", 0x800 },
        { "tri", 0x1000 }, { "o", 0x2000 }, { "x", 0x4000 }, { "sq", 0x8000 },
    };
    unsigned i;
    for (i = 0; i < sizeof(t) / sizeof(t[0]); i++)
        if ((int)strlen(t[i].name) == n && !strncasecmp(b, t[i].name, n))
            return t[i].bit;
    fprintf(stderr, "[driver1] D1_PAD: unknown button '%.*s'\n", n, b);
    return 0;
}

static unsigned pad_script_env(const char *env, int vbl)   /* buttons held at this count (active high) */
{
    const char *p = getenv(env);
    unsigned held = 0;
    while (p && *p) {
        char *e;
        int from = strtol(p, &e, 10), to = from + 5;
        unsigned m = 0;
        if (*e == '-')
            to = strtol(e + 1, &e, 10);
        if (*e == '=') {
            const char *b = e + 1;
            for (;;) {
                int n = strcspn(b, "+,");
                m |= pad_bit(b, n);
                b += n;
                if (*b != '+')
                    break;
                b++;
            }
            e = (char *)b;
        }
        if (vbl >= from && vbl <= to)
            held |= m;
        p = strchr(e, ',');
        p = p ? p + 1 : NULL;
    }
    return held;
}

#define pad_script(vbl) pad_script_env("D1_PAD", vbl)

/* D1_PADF=<script>: the same syntax keyed on the SIMULATION step (CameraCnt: 0 at the start of every game,
 * +1 per sim step in main.c func_000230B0 - it stands still in the pause menu and mission cut-scenes, where the
 * vblank-paced frame counter D_00097534 does not) instead of the vblank, for the pad reads of the in-game loop only
 * (main.c func_0002360C -> func_000514E0 -> func_000357F0 -> here); steps with no entry read as nothing pressed.
 * Boot/FMV/loading/pause times differ between machines, sim steps don't: the lockstep comparison drives the
 * emulator with the same script (tools/pc/compare/lrhost.c --padf). Not while a replay plays (D_00096899: the cars
 * replay the recorded pads, the pad drives the replay controls). Menus (and the pause menu) still read D1_PAD /
 * the real pad. */
extern int PC_TraceInGameLoop(void *addr);   /* pc_trace.c */
extern int CameraCnt;
extern unsigned char D_00096899;           /* replay playing (main.c) */

void PC_PadRead(unsigned char *pad)
{
    PsyX_UpdateInput();
    if (getenv("D1_PADF") && !D_00096899 && PC_TraceInGameLoop(__builtin_return_address(2))) {
        unsigned held = pad_script_env("D1_PADF", CameraCnt);
        pad[0] = 0;
        pad[1] = 0x41;
        pad[2] = ~held & 0xFF;
        pad[3] = ~held >> 8 & 0xFF;
    } else if (getenv("D1_PAD")) {
        unsigned held = pad_script(__real_VSync(-1));
        pad[0] = 0;          /* status: ok */
        pad[1] = 0x41;       /* digital pad */
        pad[2] = ~held & 0xFF;
        pad[3] = ~held >> 8 & 0xFF;
    }
}

#ifdef _WIN32
/* crash report: exception code, faulting instruction and data address, and the return addresses on the stack that
 * fall into the program (no ASLR: addr2line -f -e driver1.exe <addr> resolves them). Only faults inside the
 * program itself: a DLL (a GPU driver) may fault and handle it on purpose. */
static LONG CALLBACK on_crash(EXCEPTION_POINTERS *ep)
{
    extern unsigned char etext[];
    const EXCEPTION_RECORD *er = ep->ExceptionRecord;
    unsigned int eip = ep->ContextRecord->Eip, *sp = (unsigned int *)ep->ContextRecord->Esp;
    int n = 0;
    switch (er->ExceptionCode) {
    case EXCEPTION_ACCESS_VIOLATION: case EXCEPTION_ILLEGAL_INSTRUCTION: case EXCEPTION_INT_DIVIDE_BY_ZERO:
    case EXCEPTION_PRIV_INSTRUCTION: case EXCEPTION_STACK_OVERFLOW: case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
        break;
    default:
        return EXCEPTION_CONTINUE_SEARCH;
    }
    if (eip < 0x200000 || eip >= (unsigned int)etext)
        return EXCEPTION_CONTINUE_SEARCH;
    fprintf(stderr, "[driver1] exception 0x%08lX at %08X, data address %08lX. backtrace:\n  %08X\n",
            er->ExceptionCode, eip, er->NumberParameters > 1 ? (unsigned long)er->ExceptionInformation[1] : 0, eip);
    if (!IsBadReadPtr(sp, 4096))
        for (; n < 1024 && !IsBadReadPtr(sp, 4); sp++, n++)
            if (*sp >= 0x200000 && *sp < (unsigned int)etext)
                fprintf(stderr, "  %08X\n", *sp);
    fflush(stderr);
    TerminateProcess(GetCurrentProcess(), 3);
    return EXCEPTION_CONTINUE_SEARCH;
}

static void install_crash_handler(void)
{
    AddVectoredExceptionHandler(1, on_crash);
}

/* Run the game on s_game_stack (below 16 MB, see above): switch the stack pointer and tell Windows (the thread's
 * stack bounds, which exception dispatch checks; an empty SEH chain, as the old one lives on the other stack).
 * game_entry() never returns. */
static void run_game(void (*entry)(void))
{
    NT_TIB *tib = (NT_TIB *)NtCurrentTeb();
    unsigned char *top = s_game_stack + sizeof(s_game_stack);
    tib->StackBase = top;
    tib->StackLimit = s_game_stack;
    tib->ExceptionList = (EXCEPTION_REGISTRATION_RECORD *)-1;
    __asm__ volatile("movl %0, %%esp\n\tcall *%1\n\tud2" : : "r"(top), "r"(entry) : "memory");
}
#else
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
#endif

static void game_entry(void)
{
    /* modelpointers (PS1 .bss 0xA0E20, up to the next object D_000A1AA0: 0x320 slots) is only filled per model
     * as a level loads/spools, never cleared. Until a model is in, its slot keeps the previous level's pointer or,
     * if no level had it since boot, NULL - and the PS1 reads the kernel's NULL "garbage area" there: the first
     * level init after boot that isn't the attract demo (a career started from the frontend: ladder apartment
     * view, city 4) hits func_0001EEB4's modelpointers[tile][1] read before InitSpooling. PC: slots that are
     * still NULL at boot point at pc_lowram, i.e. the same zero bytes. */
    {
        extern void *modelpointers[];
        int i;
        for (i = 0; i < 0x320; i++)
            if (!modelpointers[i])
                modelpointers[i] = pc_lowram;
    }
    d1_main();
    fprintf(stderr, "[driver1] game main() returned\n");
    exit(0);
}

int main(int argc, char **argv)
{
    s_game_thread = pthread_self();
    map_psxram();
    find_datadir(argc, argv);
    setvbuf(stdout, NULL, _IOLBF, 0);
    printf("[driver1] data: %s\n", s_datadir);
    load_main_image();

    /* D1_FULLSCREEN=1: borderless fullscreen (the Steam / Game Mode launcher sets it) */
    PsyX_Initialise("Driver", 640, 480, getenv("D1_FULLSCREEN") && atoi(getenv("D1_FULLSCREEN")) ? 1 : 0);
    install_crash_handler();

#ifdef _WIN32
    run_game(game_entry);
#else
    getcontext(&s_game_ctx);
    s_game_ctx.uc_stack.ss_sp = s_game_stack;
    s_game_ctx.uc_stack.ss_size = sizeof(s_game_stack);
    s_game_ctx.uc_link = &s_main_ctx;
    makecontext(&s_game_ctx, game_entry, 0);
    swapcontext(&s_main_ctx, &s_game_ctx);
#endif
    return 0;
}

#ifdef _WIN32
/* the PS1 RAM section (see the top). Last in the file: COFF gas has no .pushsection, and GCC would carry on in
 * whatever section this leaves current */
__asm__(".section .psxram,\"bw\"\n\t.globl _pc_psxram_section\n_pc_psxram_section:\n\t.space 0x1EF000");
#endif
