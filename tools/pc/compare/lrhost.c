/* lrhost - a minimal headless libretro frontend that runs the PS1 Driver disc in an emulator core (SwanStation,
 * the DuckStation libretro fork) for the lockstep comparison (port/README.md "Lockstep comparison").
 *
 * No window, no audio: the core uses its software renderer and the frames/samples go nowhere (or to a PPM on
 * request). One retro_run() = one emulated video frame (a vblank). Things it can do:
 *   --pad SCRIPT      pad 1 script, the same syntax as the PC build's D1_PAD (FROM[-TO]=BTN[+BTN..], vblanks)
 *   --frames N        stop after N vblanks
 *   --shot n1,n2..    write shot_<n>.ppm (the core's output frame) at those vblanks
 *   --trace SPEC OUT  install the per-game-frame snapshot hook (see below) and write every snapshot to OUT
 *   --ramdump n1,..   write ram_<n>.bin (2 MB) at those vblanks
 *   --zerocheck A-B   report the words in [A,B) that were ever non-zero (finding RAM nobody uses, for the hook)
 *   --ramdump-frame F write ram_f<F>.bin (2 MB) at the hook of game frame F (state exactly at the trace point)
 *   --padf SCRIPT     in-game pad input by sim step (CameraCnt), the PC's D1_PADF (see PADCALL_ADDR)
 *   --skip-garage     the PC mod skip_garage_test on the PS1 (see install_skip)
 *   --pursuit-r R     Pursuit/Getaway: use setup R (0-14) instead of the vblank count's pick (see PURSUITR_ADDR)
 *   --until-frame N   stop once a snapshot has game frame >= N, or 3000 vblanks after the last one (with --trace)
 *   --poke VBL:ADDR=V,.. a 32-bit store into PS1 RAM at that vblank, ADDR hex (the PC's D1_POKE takes symbols)
 *   --save N FILE     savestate before vblank N; --load FILE starts from one (exploration only: patches made
 *                     before the save are in the state, so --padf/--trace must match what the saving run used)
 *
 * The game frame hook. A trace record must be the game state at one fixed point of each game frame, but a
 * libretro core only returns between vblanks, and the in-game loop (main.c func_0002360C) takes >= 2 vblanks per
 * game frame, so between two retro_run()s the game is usually half-way through a frame. So the host patches the
 * running game (in emulated RAM, once the BIOS has loaded SLUS_008.42; the disc image is not modified):
 *   0x00024240  lui v0,%hi(FastForward)   ->  jal STUB
 *   0x00024244  lw  v0,%lo(FastForward)(v0) -> nop
 * i.e. right after ControlReplay, before the frame's VSync(-1) wait - the point where the PC build's pc_trace.c
 * takes its snapshot (the first VSync(-1) poll from func_0002360C in a frame). The stub bumps a sequence word and
 * spins until the host writes that number back: the game's main thread stands still at the hook while the host,
 * after that retro_run, reads the spec's regions straight from emulated RAM (and hands over the next frame's pad
 * for --padf), then releases it. Interrupt handlers keep running meanwhile (VSync callback: the XM player; CD
 * callbacks: spooling) - only state the trace masks anyway. The stall costs the game at most one vblank per frame,
 * which only moves timing that is not part of the deterministic model (frame pacing, CD, audio). Stubs and
 * mailbox live in kernel RAM nothing uses (0xC000-0xC220, in the 0xBE34-0xDF80 gap that lrhost --zerocheck finds
 * untouched over boot, menus, demos and play); the host checks the stub code every vblank. (The first version
 * copied the regions into a buffer at 0x1E5100 instead - New York's level data lives there.)
 *
 * Spec lines (tools/pc/compare/trace.spec): "label ps1addr pcsymbol size" (the emulator only needs the address).
 *
 * Build (in the build distrobox): gcc -O2 -o lrhost lrhost.c -ldl
 * Run:    lrhost CORE.so SYSTEMDIR GAME.cue [options]
 */
#include <dlfcn.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---- the libretro API subset we need (libretro.h, MIT) ---- */
#define RETRO_DEVICE_JOYPAD 1
#define RETRO_MEMORY_SYSTEM_RAM 2
#define RETRO_ENVIRONMENT_GET_CAN_DUPE 3
#define RETRO_ENVIRONMENT_SET_PERFORMANCE_LEVEL 8
#define RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY 9
#define RETRO_ENVIRONMENT_SET_PIXEL_FORMAT 10
#define RETRO_ENVIRONMENT_SET_HW_RENDER 14
#define RETRO_ENVIRONMENT_GET_VARIABLE 15
#define RETRO_ENVIRONMENT_SET_VARIABLES 16
#define RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE 17
#define RETRO_ENVIRONMENT_GET_LOG_INTERFACE 27
#define RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY 31
#define RETRO_ENVIRONMENT_GET_CORE_OPTIONS_VERSION 52
#define RETRO_ENVIRONMENT_GET_PREFERRED_HW_RENDER 56
#define RETRO_ENVIRONMENT_EXPERIMENTAL 0x10000

struct retro_game_info { const char *path; const void *data; size_t size; const char *meta; };
struct retro_variable { const char *key; const char *value; };
typedef void (*retro_log_printf_t)(int level, const char *fmt, ...);
struct retro_log_callback { retro_log_printf_t log; };
typedef bool (*env_t)(unsigned, void *);

static void (*p_init)(void);
static bool (*p_load_game)(const struct retro_game_info *);
static void (*p_run)(void);
static void *(*p_mem_data)(unsigned);
static size_t (*p_mem_size)(unsigned);
static void (*p_set_env)(env_t);
static void (*p_set_video)(void (*)(const void *, unsigned, unsigned, size_t));
static void (*p_set_audio)(void (*)(int16_t, int16_t));
static void (*p_set_audio_batch)(size_t (*)(const int16_t *, size_t));
static void (*p_set_poll)(void (*)(void));
static void (*p_set_state)(int16_t (*)(unsigned, unsigned, unsigned, unsigned));
static void (*p_set_controller)(unsigned, unsigned);
static size_t (*p_ser_size)(void);
static bool (*p_serialize)(void *, size_t);
static bool (*p_unserialize)(const void *, size_t);

static const char *g_sysdir;
static unsigned g_pixfmt;   /* 0 = 0RGB1555, 1 = XRGB8888, 2 = RGB565 */
static int g_verbose;

static const struct { const char *k, *v; } g_opts[] = {
    { "swanstation_GPU_Renderer", "Software" },
    { "swanstation_CPU_ExecutionMode", "Interpreter" },   /* exact and no code cache: the host patches code */
    { "swanstation_BIOS_PathNTSCU", "scph5501.bin" },
    { "swanstation_Console_Region", "NTSC-U" },
    { "swanstation_BIOS_PatchFastBoot", "false" },
    { "swanstation_CDROM_ReadThread", "false" },
    { "swanstation_CDROM_LoadImageToRAM", "true" },
    { "swanstation_CDROM_ReadSpeedup", "1" },
    { "swanstation_CDROM_SeekSpeedup", "1" },
    { "swanstation_CPU_Overclock", "100" },
    { "swanstation_MemoryCards_Card1Type", "None" },      /* never writes a card */
    { "swanstation_MemoryCards_Card2Type", "None" },
    { "swanstation_GPU_UseThread", "false" },
    { "swanstation_Display_ShowOSDMessages", "false" },
    { "swanstation_Logging_LogLevel", "Warning" },
    { NULL, NULL }
};

static void log_cb(int level, const char *fmt, ...)
{
    va_list ap;
    if (level < 2 && !g_verbose)
        return;
    va_start(ap, fmt);
    fprintf(stderr, "[core] ");
    vfprintf(stderr, fmt, ap);
    va_end(ap);
}

static bool env_cb(unsigned cmd, void *data)
{
    cmd &= ~RETRO_ENVIRONMENT_EXPERIMENTAL;
    switch (cmd) {
    case RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY:
    case RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY:
        *(const char **)data = g_sysdir;
        return true;
    case RETRO_ENVIRONMENT_SET_PIXEL_FORMAT:
        g_pixfmt = *(unsigned *)data;
        return true;
    case RETRO_ENVIRONMENT_GET_VARIABLE: {
        struct retro_variable *v = data;
        for (int i = 0; g_opts[i].k; i++)
            if (!strcmp(v->key, g_opts[i].k)) {
                v->value = g_opts[i].v;
                return true;
            }
        v->value = NULL;
        return false;
    }
    case RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE:
        *(bool *)data = false;
        return true;
    case RETRO_ENVIRONMENT_GET_LOG_INTERFACE:
        ((struct retro_log_callback *)data)->log = log_cb;
        return true;
    case RETRO_ENVIRONMENT_GET_CORE_OPTIONS_VERSION:
        *(unsigned *)data = 0;   /* -> the core uses SET_VARIABLES / GET_VARIABLE */
        return true;
    case RETRO_ENVIRONMENT_GET_PREFERRED_HW_RENDER:
        *(unsigned *)data = 0;   /* RETRO_HW_CONTEXT_NONE */
        return true;
    case RETRO_ENVIRONMENT_GET_CAN_DUPE:
        *(bool *)data = true;
        return true;
    case RETRO_ENVIRONMENT_SET_HW_RENDER:
        return false;
    case RETRO_ENVIRONMENT_SET_VARIABLES:
    case RETRO_ENVIRONMENT_SET_PERFORMANCE_LEVEL:
        return true;
    default:
        return false;
    }
}

/* ---- video: keep the last frame for --shot ---- */
static const void *g_fb;
static unsigned g_fbw, g_fbh;
static size_t g_fbpitch;
static void video_cb(const void *d, unsigned w, unsigned h, size_t pitch)
{
    if (d) {
        g_fb = d;
        g_fbw = w;
        g_fbh = h;
        g_fbpitch = pitch;
    }
}
static void audio_cb(int16_t l, int16_t r) { (void)l; (void)r; }
static size_t audio_batch_cb(const int16_t *d, size_t n) { (void)d; return n; }
static void poll_cb(void) {}

static void write_ppm(const char *path)
{
    FILE *f;
    if (!g_fb || !(f = fopen(path, "wb")))
        return;
    fprintf(f, "P6\n%u %u\n255\n", g_fbw, g_fbh);
    for (unsigned y = 0; y < g_fbh; y++)
        for (unsigned x = 0; x < g_fbw; x++) {
            const uint8_t *row = (const uint8_t *)g_fb + y * g_fbpitch;
            uint8_t rgb[3];
            if (g_pixfmt == 1) {
                uint32_t p = ((const uint32_t *)row)[x];
                rgb[0] = p >> 16; rgb[1] = p >> 8; rgb[2] = p;
            } else if (g_pixfmt == 2) {
                uint16_t p = ((const uint16_t *)row)[x];
                rgb[0] = (p >> 11) << 3; rgb[1] = ((p >> 5) & 63) << 2; rgb[2] = (p & 31) << 3;
            } else {
                uint16_t p = ((const uint16_t *)row)[x];
                rgb[0] = ((p >> 10) & 31) << 3; rgb[1] = ((p >> 5) & 31) << 3; rgb[2] = (p & 31) << 3;
            }
            fwrite(rgb, 1, 3, f);
        }
    fclose(f);
}

/* ---- pad script: the D1_PAD syntax of port/pc/pc_main.c ---- */
struct padev { unsigned from, to, bits; };
static struct padev g_pad[512];
static int g_npad;
static unsigned g_vbl;   /* vblanks run so far */

static unsigned pad_bit(const char *b, int n)
{
    /* -> libretro joypad ids: B=0 (Cross) Y=1 (Square) SELECT=2 START=3 UP=4 DOWN=5 LEFT=6 RIGHT=7 A=8 (Circle)
     *    X=9 (Triangle) L=10 R=11 L2=12 R2=13 */
    static const struct { const char *name; int id; } t[] = {
        { "x", 0 }, { "sq", 1 }, { "select", 2 }, { "start", 3 }, { "up", 4 }, { "down", 5 }, { "left", 6 },
        { "right", 7 }, { "o", 8 }, { "tri", 9 }, { "l1", 10 }, { "r1", 11 }, { "l2", 12 }, { "r2", 13 },
    };
    for (unsigned i = 0; i < sizeof t / sizeof t[0]; i++)
        if ((int)strlen(t[i].name) == n && !strncmp(t[i].name, b, n))
            return 1u << t[i].id;
    fprintf(stderr, "lrhost: unknown pad button '%.*s'\n", n, b);
    return 0;
}

static void parse_pad(const char *s)
{
    while (*s && g_npad < 512) {
        char *e;
        struct padev ev;
        ev.from = strtoul(s, &e, 10);
        ev.to = ev.from + 5;
        if (*e == '-')
            ev.to = strtoul(e + 1, &e, 10);
        ev.bits = 0;
        if (*e == '=') {
            const char *b = e + 1;
            for (;;) {
                const char *q = b;
                while (*q && *q != '+' && *q != ',')
                    q++;
                ev.bits |= pad_bit(b, q - b);
                if (*q != '+') {
                    e = (char *)q;
                    break;
                }
                b = q + 1;
            }
        }
        g_pad[g_npad++] = ev;
        s = *e == ',' ? e + 1 : e;
        if (*e && *e != ',')
            break;
    }
}

static int16_t state_cb(unsigned port, unsigned device, unsigned index, unsigned id)
{
    (void)index;
    if (port != 0 || device != RETRO_DEVICE_JOYPAD || id > 15)
        return 0;
    for (int i = 0; i < g_npad; i++)
        if (g_vbl >= g_pad[i].from && g_vbl <= g_pad[i].to && (g_pad[i].bits >> id & 1))
            return 1;
    return 0;
}

static int in_list(const char *list, unsigned n)
{
    const char *p = list;
    while (p && *p) {
        char *e;
        unsigned v = strtoul(p, &e, 10);
        if (v == n)
            return 1;
        if (*e != ',')
            break;
        p = e + 1;
    }
    return 0;
}

/* ---- the game frame hook (see the top of the file) ---- */
#define HOOK_ADDR     0x00024240u
#define HOOK_INSN0    0x3C020009u   /* lui v0, 0x9            (%hi(FastForward)) */
#define HOOK_INSN1    0x8C426DC4u   /* lw  v0, 0x6DC4(v0)     (%lo(FastForward)) */
#define STUB_BASE     0x0000C000u   /* the stall stub */
#define PADSTUB       0x0000C080u   /* --padf */
#define SKIPSTUB      0x0000C140u   /* --skip-garage */
#define CODE_END      0x0000C160u
#define MAIL          0x0000C200u   /* +0 seq (stub), +4 release (host), +8 next frame, +C its pad, +10 frame 0's pad */
#define MAIL_END      0x0000C220u
#define FRAME_ADDR    0x00097534u   /* D_00097534, the game frame counter */
#define CAMCNT_ADDR   0x000970D8u   /* CameraCnt, the sim step counter: what --padf is keyed on */
/* --padf: main.c func_0002360C reads the pad once per game frame through func_000514E0 (call at 0x237B4). That call
 * is redirected to PADSTUB, which calls func_000514E0 and then, as the PC's D1_PADF does, replaces the result with
 * the scripted buttons of the current sim step (CameraCnt - +1 per sim step, still in pause and cut-scenes; nothing
 * pressed where the script has no entry): D_000A8018 (decoded pad), D_0009752C (held) and D_00097538 (newly pressed
 * = held & (previous ^ held), func_000514E0's own formula). The buttons come from the mailbox: at each stall the host
 * writes the entry for the step the next read will see (CameraCnt does not change between the hook and the next
 * frame's pad read), and step 0's is always there (a new game starts at 0 before any stall). Not while a replay
 * plays (D_00096899 != 0), as on the PC. Menus and the pause menu read the pad elsewhere and keep the libretro pad
 * (--pad, by vblank). */
#define PADCALL_ADDR  0x000237B4u
#define PADCALL_INSN  0x0C014538u   /* jal func_000514E0 */
/* --skip-garage: the PC mod "skip_garage_test" (port/pc/pc_mods.c, main.c func_00024754 case 0) on the PS1: the
 * `jal func_00024F1C` (play the interview) at 0x24C28 becomes a call to SKIPSTUB, which does what the PC does
 * instead: D_000970A8 = 1, D_00096818 = 1 ($gp = 0x80096770). */
#define SKIPCALL_ADDR 0x00024C28u
#define SKIPCALL_INSN 0x0C0093C7u   /* jal func_00024F1C */

/* --pursuit-r R: Pursuit/Getaway (D_00096898 == 3) pick one of 15 setups at level init with VSync(-1) % 15 (main.c
 * func_00022818) - the vblank count, so it differs per run on a real PS1 too. To compare a session the emulator
 * takes the PC's choice (its D_00096914 at frame 0): the first `jal VSync` there becomes `addiu v0, zero, R`. */
#define PURSUITR_ADDR 0x00022A84u
#define PURSUITR_INSN 0x0C01E6C0u   /* jal VSync */

#define MAXREG 63
struct region { char label[64]; uint32_t addr, size; };
static struct region g_reg[MAXREG];
static int g_nreg;
static uint32_t g_bufsize;

static int load_spec(const char *path)
{
    FILE *f = fopen(path, "r");
    char line[256];
    if (!f) {
        perror(path);
        return -1;
    }
    while (fgets(line, sizeof line, f)) {
        char label[64], pcname[128];
        unsigned addr, size;
        char *h = strchr(line, '#');
        if (h)
            *h = 0;
        /* "label ps1addr pcsymbol size" */
        if (sscanf(line, "%63s %x %127s %u", label, &addr, pcname, &size) != 4 || !strcmp(label, "mask") ||
            !strcmp(label, "pausemask"))
            continue;   /* mask lines are for lockstep.py ("mask car_data .." would scan "ca" as hex) */
        if (g_nreg == MAXREG || addr + size > 0x200000) {
            fprintf(stderr, "lrhost: bad spec entry %s\n", label);
            return -1;
        }
        strcpy(g_reg[g_nreg].label, label);
        g_reg[g_nreg].addr = addr;
        g_reg[g_nreg].size = size;
        g_bufsize += size;
        g_nreg++;
    }
    fclose(f);
    return 0;
}

static void w32(uint8_t *ram, uint32_t a, uint32_t v) { memcpy(ram + a, &v, 4); }
static uint32_t r32(const uint8_t *ram, uint32_t a) { uint32_t v; memcpy(&v, ram + a, 4); return v; }

/* MIPS encoders */
#define R(op, rs, rt, rd, sh, fn) (((op) << 26) | ((rs) << 21) | ((rt) << 16) | ((rd) << 11) | ((sh) << 6) | (fn))
#define I(op, rs, rt, imm) (((op) << 26) | ((rs) << 21) | ((rt) << 16) | ((imm) & 0xFFFF))
enum { ZERO = 0, V0 = 2, T0 = 8, T1, T2, T3, T4, T5, T6, T7, GP = 28, SP = 29, RA = 31 };
#define LUI(rt, imm) I(0x0F, 0, rt, imm)
#define ORI(rt, rs, imm) I(0x0D, rs, rt, imm)
#define LW(rt, off, rs) I(0x23, rs, rt, off)
#define SW(rt, off, rs) I(0x2B, rs, rt, off)
#define LHU(rt, off, rs) I(0x25, rs, rt, off)
#define LBU(rt, off, rs) I(0x24, rs, rt, off)
#define SH(rt, off, rs) I(0x29, rs, rt, off)
#define ADDIU(rt, rs, imm) I(0x09, rs, rt, imm)
#define BEQ(rs, rt, off) I(0x04, rs, rt, off)
#define BNE(rs, rt, off) I(0x05, rs, rt, off)
#define ADDU(rd, rs, rt) R(0, rs, rt, rd, 0, 0x21)
#define XOR(rd, rs, rt) R(0, rs, rt, rd, 0, 0x26)
#define AND(rd, rs, rt) R(0, rs, rt, rd, 0, 0x24)
#define JR(rs) R(0, rs, 0, 0, 0, 8)
#define NOP 0u
#define JAL(a) ((3u << 26) | (((a) >> 2) & 0x03FFFFFF))
#define BR(from, to) (((to) - ((from) + 1)) & 0xFFFF)

static void put(uint8_t *ram, uint32_t base, const uint32_t *c, int n, uint32_t limit)
{
    if (base + n * 4 > limit) {
        fprintf(stderr, "lrhost: stub at %X too long\n", base);
        exit(1);
    }
    for (int i = 0; i < n; i++)
        w32(ram, base + i * 4, c[i]);
}

static void install_hook(uint8_t *ram)
{
    uint32_t c[32];
    int n = 0, spin;
    c[n++] = LUI(T0, 0x8000 | (MAIL >> 16));
    c[n++] = ORI(T0, T0, MAIL & 0xFFFF);
    c[n++] = LW(T1, 0, T0);
    c[n++] = NOP;
    c[n++] = ADDIU(T1, T1, 1);
    c[n++] = SW(T1, 0, T0);                       /* seq++ */
    spin = n;
    c[n++] = LW(T2, 4, T0);                       /* until release == seq */
    c[n++] = NOP;
    c[n] = BNE(T2, T1, BR(n, spin));
    n++;
    c[n++] = NOP;
    c[n++] = HOOK_INSN0;                          /* the two replaced instructions */
    c[n++] = HOOK_INSN1;
    c[n++] = JR(RA);
    c[n++] = NOP;
    put(ram, STUB_BASE, c, n, PADSTUB);
    w32(ram, HOOK_ADDR + 4, NOP);
    w32(ram, HOOK_ADDR, JAL(0x80000000u | STUB_BASE));
}

static void install_padf(uint8_t *ram)
{
    uint32_t c[40];
    int n = 0, br0, b1, b2, b3, apply;
    c[n++] = ADDIU(SP, SP, -24);
    c[n++] = SW(RA, 16, SP);
    c[n++] = JAL(0x800514E0u);                    /* func_000514E0 as before */
    c[n++] = NOP;
    c[n++] = LW(RA, 16, SP);
    c[n++] = ADDIU(SP, SP, 24);
    c[n++] = LUI(T0, 0x8009);
    c[n++] = LBU(T1, 0x6899, T0);                 /* D_00096899: a replay plays - leave the pad alone */
    c[n++] = NOP;
    br0 = n;
    c[n++] = 0;                                   /* bne t1, zero, out */
    c[n++] = NOP;
    c[n++] = LW(T1, CAMCNT_ADDR & 0xFFFF, T0);    /* step = CameraCnt */
    c[n++] = LUI(T2, 0x8000 | (MAIL >> 16));
    c[n++] = ORI(T2, T2, MAIL & 0xFFFF);
    c[n++] = LW(T3, 8, T2);                       /* the mailbox's frame */
    c[n++] = NOP;
    b1 = n;
    c[n++] = 0;                                   /* bne t3, t1, notnext */
    c[n++] = ADDU(T5, ZERO, ZERO);                /* delay: v = 0 */
    c[n++] = LHU(T5, 0xC, T2);                    /* v = its pad */
    b2 = n;
    c[n++] = 0;                                   /* b apply */
    c[n++] = NOP;
    int notnext = n;
    b3 = n;
    c[n++] = 0;                                   /* bne t1, zero, apply */
    c[n++] = NOP;
    c[n++] = LHU(T5, 0x10, T2);                   /* frame 0 */
    apply = n;
    c[n++] = LUI(T7, 0x800B);
    c[n++] = SH(T5, -0x7FE8, T7);                 /* D_000A8018 = v */
    c[n++] = SH(T5, 0x752C, T0);                  /* D_0009752C = v */
    c[n++] = LHU(T6, 0x7544, T0);                 /* D_00097544 (previous) */
    c[n++] = NOP;
    c[n++] = XOR(T6, T6, T5);
    c[n++] = AND(T6, T6, T5);
    c[n++] = SH(T6, 0x7538, T0);                  /* D_00097538 = v & (prev ^ v) */
    int out = n;
    c[n++] = JR(RA);
    c[n++] = NOP;
    c[br0] = BNE(T1, ZERO, BR(br0, out));
    c[b1] = BNE(T3, T1, BR(b1, notnext));
    c[b2] = BEQ(ZERO, ZERO, BR(b2, apply));
    c[b3] = BNE(T1, ZERO, BR(b3, apply));
    put(ram, PADSTUB, c, n, SKIPSTUB);
    w32(ram, PADCALL_ADDR, JAL(0x80000000u | PADSTUB));
}

static void install_skip(uint8_t *ram)
{
    uint32_t c[] = {
        ADDIU(V0, ZERO, 1),
        SW(V0, 0x0938, GP),                       /* D_000970A8 */
        SW(V0, 0x00A8, GP),                       /* D_00096818 */
        JR(RA),
        NOP,
    };
    put(ram, SKIPSTUB, c, 5, CODE_END);
    w32(ram, SKIPCALL_ADDR, JAL(0x80000000u | SKIPSTUB));
}

/* --padf script -> buttons per game frame, in the decoded word func_00035818 makes of the raw pad,
 * ~((raw[2] << 8) | raw[3]); FROM[-TO]=BTN[+BTN], a lone FROM holds 6 frames (the D1_PAD rule) */
#define PADF_MAX 100000
static uint16_t *g_padf;

static unsigned padraw_bit(const char *b, int n)   /* port/pc/pc_main.c pad_bit: the PADRAW word, active high */
{
    static const struct { const char *name; unsigned bit; } t[] = {
        { "select", 0x1 }, { "l3", 0x2 }, { "r3", 0x4 }, { "start", 0x8 }, { "up", 0x10 }, { "right", 0x20 },
        { "down", 0x40 }, { "left", 0x80 }, { "l2", 0x100 }, { "r2", 0x200 }, { "l1", 0x400 }, { "r1", 0x800 },
        { "tri", 0x1000 }, { "o", 0x2000 }, { "x", 0x4000 }, { "sq", 0x8000 },
    };
    for (unsigned i = 0; i < sizeof t / sizeof t[0]; i++)
        if ((int)strlen(t[i].name) == n && !strncmp(t[i].name, b, n))
            return t[i].bit;
    fprintf(stderr, "lrhost: unknown pad button '%.*s'\n", n, b);
    return 0;
}

static int parse_padf(const char *s)
{
    g_padf = calloc(PADF_MAX, 2);
    while (s && *s) {
        char *e;
        unsigned from = strtoul(s, &e, 10), to = from + 5, m = 0;
        if (*e == '-')
            to = strtoul(e + 1, &e, 10);
        if (*e == '=') {
            const char *b = e + 1;
            for (;;) {
                int n = strcspn(b, "+,");
                m |= padraw_bit(b, n);
                b += n;
                if (*b != '+')
                    break;
                b++;
            }
            e = (char *)b;
        }
        if (to >= PADF_MAX) {
            fprintf(stderr, "lrhost: --padf frame %u beyond %u\n", to, PADF_MAX);
            return -1;
        }
        for (unsigned f = from; f <= to; f++)
            g_padf[f] |= (uint16_t)(((m & 0xFF) << 8) | (m >> 8 & 0xFF));
        s = strchr(e, ',');
        s = s ? s + 1 : NULL;
    }
    return 0;
}

static uint16_t padf_at(uint32_t f) { return g_padf && f < PADF_MAX ? g_padf[f] : 0; }

int main(int argc, char **argv)
{
    const char *pad = NULL, *shots = NULL, *dumps = NULL, *spec = NULL, *out = NULL, *zc = NULL;
    const char *save_path = NULL, *load_path = NULL, *pokes = NULL;
    unsigned frames = 3600, zlo = 0, zhi = 0, save_at = 0, vbl0 = 0, last_snap = 0, nrec = 0;
    int pursuit_r = -1, dump_frame = -1, padf = 0, skip = 0, until = -1, stop = 0, hooked = 0, extras = 0;
    struct retro_game_info gi = { 0 };
    void *h;
    uint8_t *ram, *zmask = NULL, *codecopy = NULL;
    FILE *fo = NULL;
    uint32_t lastseq = 0;

    if (argc < 4) {
        fprintf(stderr, "usage: lrhost CORE.so SYSTEMDIR GAME.cue [options, see the top of lrhost.c]\n");
        return 2;
    }
    g_sysdir = argv[2];
    for (int i = 4; i < argc; i++) {
        if (!strcmp(argv[i], "--pad") && i + 1 < argc) pad = argv[++i];
        else if (!strcmp(argv[i], "--frames") && i + 1 < argc) frames = strtoul(argv[++i], NULL, 0);
        else if (!strcmp(argv[i], "--shot") && i + 1 < argc) shots = argv[++i];
        else if (!strcmp(argv[i], "--ramdump") && i + 1 < argc) dumps = argv[++i];
        else if (!strcmp(argv[i], "--ramdump-frame") && i + 1 < argc) dump_frame = strtol(argv[++i], NULL, 0);
        else if (!strcmp(argv[i], "--trace") && i + 2 < argc) { spec = argv[++i]; out = argv[++i]; }
        else if (!strcmp(argv[i], "--zerocheck") && i + 1 < argc) zc = argv[++i];
        else if (!strcmp(argv[i], "--padf") && i + 1 < argc) { if (parse_padf(argv[++i])) return 2; padf = 1; }
        else if (!strcmp(argv[i], "--skip-garage")) skip = 1;
        else if (!strcmp(argv[i], "--pursuit-r") && i + 1 < argc) pursuit_r = strtol(argv[++i], NULL, 0);
        else if (!strcmp(argv[i], "--until-frame") && i + 1 < argc) until = strtol(argv[++i], NULL, 0);
        else if (!strcmp(argv[i], "--save") && i + 2 < argc) { save_at = strtoul(argv[++i], NULL, 0); save_path = argv[++i]; }
        else if (!strcmp(argv[i], "--load") && i + 1 < argc) load_path = argv[++i];
        else if (!strcmp(argv[i], "--poke") && i + 1 < argc) pokes = argv[++i];
        else if (!strcmp(argv[i], "-v")) g_verbose = 1;
        else { fprintf(stderr, "lrhost: unknown option %s\n", argv[i]); return 2; }
    }
    if (pad)
        parse_pad(pad);
    if (zc) {
        sscanf(zc, "%x-%x", &zlo, &zhi);
        zmask = calloc(1, 0x200000);
    }
    if (spec && load_spec(spec))
        return 1;
    if (out && !(fo = fopen(out, "wb"))) {
        perror(out);
        return 1;
    }

    h = dlopen(argv[1], RTLD_NOW | RTLD_LOCAL);
    if (!h) {
        fprintf(stderr, "lrhost: %s\n", dlerror());
        return 1;
    }
#define SYM(p, n) do { *(void **)&p = dlsym(h, n); if (!p) { fprintf(stderr, "lrhost: no %s\n", n); return 1; } } while (0)
    SYM(p_init, "retro_init");
    SYM(p_load_game, "retro_load_game");
    SYM(p_run, "retro_run");
    SYM(p_mem_data, "retro_get_memory_data");
    SYM(p_mem_size, "retro_get_memory_size");
    SYM(p_set_env, "retro_set_environment");
    SYM(p_set_video, "retro_set_video_refresh");
    SYM(p_set_audio, "retro_set_audio_sample");
    SYM(p_set_audio_batch, "retro_set_audio_sample_batch");
    SYM(p_set_poll, "retro_set_input_poll");
    SYM(p_set_state, "retro_set_input_state");
    SYM(p_set_controller, "retro_set_controller_port_device");
    SYM(p_ser_size, "retro_serialize_size");
    SYM(p_serialize, "retro_serialize");
    SYM(p_unserialize, "retro_unserialize");

    p_set_env(env_cb);
    p_init();
    p_set_video(video_cb);
    p_set_audio(audio_cb);
    p_set_audio_batch(audio_batch_cb);
    p_set_poll(poll_cb);
    p_set_state(state_cb);
    gi.path = argv[3];
    if (!p_load_game(&gi)) {
        fprintf(stderr, "lrhost: retro_load_game failed\n");
        return 1;
    }
    p_set_controller(0, RETRO_DEVICE_JOYPAD);
    if (load_path) {
        /* a savestate made with --save (exploring menus faster; make it without --trace/--padf/--skip-garage, so
         * the patches go in after loading); its first word is the vblank it was made at, so --pad / --shot /
         * --frames keep counting from boot */
        FILE *f = fopen(load_path, "rb");
        size_t n = p_ser_size();
        uint8_t *st = malloc(n);
        if (!f || fread(&vbl0, 4, 1, f) != 1 || fread(st, 1, n, f) != n || !p_unserialize(st, n)) {
            fprintf(stderr, "lrhost: cannot load state %s\n", load_path);
            return 1;
        }
        fclose(f);
        free(st);
        fprintf(stderr, "lrhost: state %s (vblank %u)\n", load_path, vbl0);
    }
    ram = p_mem_data(RETRO_MEMORY_SYSTEM_RAM);
    if (!ram || p_mem_size(RETRO_MEMORY_SYSTEM_RAM) < 0x200000) {
        fprintf(stderr, "lrhost: no system RAM\n");
        return 1;
    }
    if (fo) {
        /* header: magic, record size, region count, then per region label[32] addr size */
        uint32_t hdr[3] = { 0x31434C44u /* "DLC1" */, 8 + g_bufsize, (uint32_t)g_nreg };
        fwrite(hdr, 4, 3, fo);
        for (int i = 0; i < g_nreg; i++) {
            char lab[32] = { 0 };
            memcpy(lab, g_reg[i].label, strnlen(g_reg[i].label, 31));
            fwrite(lab, 1, 32, fo);
            fwrite(&g_reg[i].addr, 4, 1, fo);
            fwrite(&g_reg[i].size, 4, 1, fo);
        }
    }

    for (g_vbl = vbl0; g_vbl < frames && !stop; g_vbl++) {
        if (save_path && g_vbl == save_at) {
            size_t n = p_ser_size();
            uint8_t *st = malloc(n);
            FILE *f = fopen(save_path, "wb");
            if (f && p_serialize(st, n)) {
                fwrite(&g_vbl, 4, 1, f);
                fwrite(st, 1, n, f);
                fprintf(stderr, "lrhost: saved state at vblank %u -> %s\n", g_vbl, save_path);
            }
            if (f)
                fclose(f);
            free(st);
        }
        p_run();
        if (pokes) {
            /* --poke VBL:ADDR=VALUE,..: a 32-bit store into PS1 RAM at that vblank (the PC's D1_POKE) */
            const char *q = pokes;
            while (q && *q) {
                char *e;
                unsigned at = strtoul(q, &e, 0), a = 0;
                long v = 0;
                if (*e == ':')
                    a = strtoul(e + 1, &e, 16);
                if (*e == '=')
                    v = strtol(e + 1, &e, 0);
                if (at == g_vbl && a < 0x200000) {
                    w32(ram, a & ~3u, (uint32_t)v);
                    fprintf(stderr, "lrhost: vblank %u: poke %05X = %ld\n", g_vbl, a, v);
                }
                q = strchr(e, ',');
                q = q ? q + 1 : NULL;
            }
        }
        /* SLUS_008.42 is loaded and unpatched: install what was asked for */
        if (!extras && (spec || padf || skip || pursuit_r >= 0) && r32(ram, 0x24254) == 0x0C01E6C0u /* jal VSync */ &&
            r32(ram, HOOK_ADDR) == HOOK_INSN0 && r32(ram, HOOK_ADDR + 4) == HOOK_INSN1 &&
            r32(ram, PADCALL_ADDR) == PADCALL_INSN && r32(ram, SKIPCALL_ADDR) == SKIPCALL_INSN) {
            memset(ram + MAIL, 0, MAIL_END - MAIL);
            w32(ram, MAIL + 8, 0xFFFFFFFFu);
            w32(ram, MAIL + 0x10, padf_at(0));
            if (spec || padf) {
                install_hook(ram);
                hooked = 1;
            }
            if (padf)
                install_padf(ram);
            if (skip)
                install_skip(ram);
            if (pursuit_r >= 0 && r32(ram, PURSUITR_ADDR) == PURSUITR_INSN)
                w32(ram, PURSUITR_ADDR, ADDIU(V0, ZERO, pursuit_r % 15));
            codecopy = malloc(CODE_END - STUB_BASE);
            memcpy(codecopy, ram + STUB_BASE, CODE_END - STUB_BASE);
            extras = 1;
            fprintf(stderr, "lrhost: %s%s%sinstalled at vblank %u\n", hooked ? "hook " : "", padf ? "--padf " : "",
                    skip ? "--skip-garage " : "", g_vbl);
        }
        if (extras && memcmp(codecopy, ram + STUB_BASE, CODE_END - STUB_BASE)) {
            fprintf(stderr, "lrhost: kernel RAM 0x%X-0x%X (the stubs) overwritten at vblank %u - giving up\n",
                    STUB_BASE, CODE_END, g_vbl);
            return 3;
        }
        if (hooked && r32(ram, MAIL) != lastseq) {
            /* the game stands in the stub at the hook: this is the frame's state */
            uint32_t seq = r32(ram, MAIL), frame = r32(ram, FRAME_ADDR);
            if (fo) {
                fwrite(&seq, 4, 1, fo);
                fwrite(&g_vbl, 4, 1, fo);
                for (int i = 0; i < g_nreg; i++)
                    fwrite(ram + g_reg[i].addr, 1, g_reg[i].size, fo);
                nrec++;
            }
            if ((int)frame == dump_frame) {
                char nm[64];
                FILE *f;
                snprintf(nm, sizeof nm, "ram_f%u.bin", frame);
                if ((f = fopen(nm, "wb"))) {
                    fwrite(ram, 1, 0x200000, f);
                    fclose(f);
                }
                dump_frame = -1;
            }
            w32(ram, MAIL + 8, r32(ram, CAMCNT_ADDR));
            w32(ram, MAIL + 0xC, padf_at(r32(ram, CAMCNT_ADDR)));
            w32(ram, MAIL + 4, seq);              /* release */
            lastseq = seq;
            last_snap = g_vbl;
            if (until >= 0 && (int)frame >= until)
                stop = 1;
        }
        if (until >= 0 && lastseq && g_vbl - last_snap > 3000) {
            fprintf(stderr, "lrhost: no game frame for 3000 vblanks (the game ended) - stopping\n");
            stop = 1;
        }
        if (zmask && (zhi - zlo <= 0x10000 || g_vbl % 8 == 0))
            for (uint32_t a = zlo; a < zhi; a += 4)
                if (r32(ram, a) && !(extras && a >= STUB_BASE && a < MAIL_END))
                    zmask[a] = 1;
        if (shots && in_list(shots, g_vbl)) {
            char nm[64];
            snprintf(nm, sizeof nm, "shot_%u.ppm", g_vbl);
            write_ppm(nm);
        }
        if (dumps && in_list(dumps, g_vbl)) {
            char nm[64];
            FILE *f;
            snprintf(nm, sizeof nm, "ram_%u.bin", g_vbl);
            if ((f = fopen(nm, "wb"))) {
                fwrite(ram, 1, 0x200000, f);
                fclose(f);
            }
        }
    }
    if (zmask) {
        uint32_t run = 0;
        for (uint32_t a = zlo; a <= zhi; a += 4) {
            int used = a < zhi && zmask[a];
            if (used && !run)
                run = a | 1;
            if (!used && run) {
                printf("used %05X-%05X\n", run & ~1u, a);
                run = 0;
            }
        }
    }
    if (fo)
        fclose(fo);
    fprintf(stderr, "lrhost: %u vblanks, %u snapshots\n", g_vbl, nrec);
    return 0;
}
