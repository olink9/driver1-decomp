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
 *
 * The snapshot hook. A trace record must be the game state at one fixed point of each game frame, but a libretro
 * core only returns between vblanks, and the in-game loop (main.c func_0002360C) takes >= 2 vblanks per game frame,
 * so between two retro_run()s the game is usually half-way through a frame. So the host patches the running game
 * (in emulated RAM, after the BIOS has loaded SLUS_008.42; the disc image is not modified):
 *   0x00024240  lui v0,%hi(FastForward)   ->  jal STUB
 *   0x00024244  lw  v0,%lo(FastForward)(v0) -> nop
 * i.e. right after ControlReplay, before the frame's VSync(-1) wait - the point where the PC build's
 * pc_trace.c takes its snapshot (the first VSync(-1) poll from func_0002360C in a frame). The stub copies the
 * regions listed in a table (written by the host from the spec) into a buffer, bumps a sequence word, redoes the
 * two replaced instructions and returns ($ra is free there: func_0002360C saved it in its prologue, and no
 * t-register is live at that label). One game frame takes >= 2 vblanks, so one buffer is enough: after every
 * retro_run the host checks the sequence word and copies the buffer out. Stub, table and buffer live in kernel
 * RAM that stays zero for the whole run (checked with --zerocheck; see STUB_BASE).
 *
 * Spec lines (tools/pc/compare/trace.spec): "label ps1addr size" (the emulator only needs the PS1 address).
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

/* ---- the snapshot hook ---- */
#define HOOK_ADDR   0x00024240u
#define HOOK_INSN0  0x3C020009u   /* lui v0, 0x9            (%hi(FastForward)) */
#define HOOK_INSN1  0x8C426DC4u   /* lw  v0, 0x6DC4(v0)     (%lo(FastForward)) */
/* RAM nothing touches during boot, the frontend and the attract demos (lrhost --zerocheck 0-0x200000 over 30000
 * vblanks): 0x1E4D64-0x1EDA6C, between IVIEW.BIN's end (0x1E0000 + code/data/bss) and the stack's deepest use. Only
 * CRED.BIN (the credits, 80 KB from 0x1E0000) covers it - the hook is reinstalled when the game's code comes back,
 * and the host warns whenever the buffer changes without the sequence word moving (someone else writes there).
 * Layout: stub (<= 0x100), table (count + 63 entries), buffer (seq word, then the regions). */
#define STUB_BASE   0x001E4E00u
#define TABLE_BASE  0x001E4F00u
#define TABLE_MAX   ((0x200 - 4) / 8)
#define BUF_BASE    0x001E5100u
#define BUF_END     0x001ED800u

struct region { char label[64]; uint32_t addr, size; };
static struct region g_reg[TABLE_MAX];
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
        if (sscanf(line, "%63s %x %127s %u", label, &addr, pcname, &size) != 4)
            continue;
        if (g_nreg == TABLE_MAX || (size & 3) || (addr & 3)) {
            fprintf(stderr, "lrhost: bad spec entry %s (table full or not word-sized)\n", label);
            return -1;
        }
        strcpy(g_reg[g_nreg].label, label);
        g_reg[g_nreg].addr = addr;
        g_reg[g_nreg].size = size;
        g_bufsize += size;
        g_nreg++;
    }
    fclose(f);
    if (BUF_BASE + 4 + g_bufsize > BUF_END) {
        fprintf(stderr, "lrhost: spec needs %u bytes, the buffer has %u\n", g_bufsize, BUF_END - BUF_BASE - 4);
        return -1;
    }
    return 0;
}

static void w32(uint8_t *ram, uint32_t a, uint32_t v) { memcpy(ram + a, &v, 4); }
static uint32_t r32(const uint8_t *ram, uint32_t a) { uint32_t v; memcpy(&v, ram + a, 4); return v; }

/* MIPS encoders */
#define R(op, rs, rt, rd, sh, fn) (((op) << 26) | ((rs) << 21) | ((rt) << 16) | ((rd) << 11) | ((sh) << 6) | (fn))
#define I(op, rs, rt, imm) (((op) << 26) | ((rs) << 21) | ((rt) << 16) | ((imm) & 0xFFFF))
enum { ZERO = 0, V0 = 2, T0 = 8, T1, T2, T3, T4, T5, T6, RA = 31 };
#define LUI(rt, imm) I(0x0F, 0, rt, imm)
#define ORI(rt, rs, imm) I(0x0D, rs, rt, imm)
#define LW(rt, off, rs) I(0x23, rs, rt, off)
#define SW(rt, off, rs) I(0x2B, rs, rt, off)
#define ADDIU(rt, rs, imm) I(0x09, rs, rt, imm)
#define BEQ(rs, rt, off) I(0x04, rs, rt, off)
#define BNE(rs, rt, off) I(0x05, rs, rt, off)
#define JR(rs) R(0, rs, 0, 0, 0, 8)
#define NOP 0u
#define JAL(a) ((3u << 26) | (((a) >> 2) & 0x03FFFFFF))

static void install_stub(uint8_t *ram)
{
    uint32_t c[64];
    int n = 0, loop, inner, next, done;
    /* table: [count] then {src, words} per region; dst = BUF_BASE + 4 */
    w32(ram, TABLE_BASE, g_nreg);
    for (int i = 0; i < g_nreg; i++) {
        w32(ram, TABLE_BASE + 4 + i * 8, 0x80000000u | g_reg[i].addr);
        w32(ram, TABLE_BASE + 8 + i * 8, g_reg[i].size / 4);
    }
    c[n++] = LUI(T0, 0x8000 | (TABLE_BASE >> 16));
    c[n++] = ORI(T0, T0, TABLE_BASE & 0xFFFF);
    c[n++] = LW(T1, 0, T0);                       /* t1 = count */
    c[n++] = ADDIU(T0, T0, 4);
    c[n++] = LUI(T2, 0x8000 | ((BUF_BASE + 4) >> 16));
    c[n++] = ORI(T2, T2, (BUF_BASE + 4) & 0xFFFF);
    loop = n;
    c[n++] = 0;                                   /* beq t1, zero, done (patched) */
    c[n++] = NOP;
    c[n++] = LW(T3, 0, T0);                       /* t3 = src */
    c[n++] = LW(T4, 4, T0);                       /* t4 = words */
    c[n++] = ADDIU(T0, T0, 8);
    inner = n;
    c[n++] = 0;                                   /* beq t4, zero, next (patched) */
    c[n++] = NOP;
    c[n++] = LW(T5, 0, T3);
    c[n++] = ADDIU(T3, T3, 4);
    c[n++] = SW(T5, 0, T2);
    c[n++] = ADDIU(T4, T4, -1);
    c[n] = BEQ(ZERO, ZERO, (inner - (n + 1)) & 0xFFFF);
    n++;
    c[n++] = ADDIU(T2, T2, 4);                    /* delay slot */
    next = n;
    c[n] = BEQ(ZERO, ZERO, (loop - (n + 1)) & 0xFFFF);
    n++;
    c[n++] = ADDIU(T1, T1, -1);                   /* delay slot */
    done = n;
    c[n++] = LUI(T6, 0x8000 | (BUF_BASE >> 16));
    c[n++] = ORI(T6, T6, BUF_BASE & 0xFFFF);
    c[n++] = LW(T5, 0, T6);
    c[n++] = NOP;
    c[n++] = ADDIU(T5, T5, 1);
    c[n++] = SW(T5, 0, T6);                       /* seq++ last: the buffer is complete */
    c[n++] = HOOK_INSN0;                          /* the two replaced instructions */
    c[n++] = HOOK_INSN1;
    c[n++] = JR(RA);
    c[n++] = NOP;
    c[loop] = BEQ(T1, ZERO, (done - (loop + 1)) & 0xFFFF);
    c[inner] = BEQ(T4, ZERO, (next - (inner + 1)) & 0xFFFF);
    for (int i = 0; i < n; i++)
        w32(ram, STUB_BASE + i * 4, c[i]);
    w32(ram, BUF_BASE, 0);
    /* the hook last: jal STUB; nop */
    w32(ram, HOOK_ADDR + 4, NOP);
    w32(ram, HOOK_ADDR, JAL(0x80000000u | STUB_BASE));
}

int main(int argc, char **argv)
{
    const char *pad = NULL, *shots = NULL, *dumps = NULL, *spec = NULL, *out = NULL, *zc = NULL;
    unsigned frames = 3600, zlo = 0, zhi = 0;
    struct retro_game_info gi = { 0 };
    void *h;
    uint8_t *ram, *zmask = NULL;
    FILE *fo = NULL;
    uint32_t lastseq = 0;
    int hooked = 0;
    unsigned nrec = 0, nclobber = 0;
    int changing = 0;
    uint8_t *stubcopy = NULL, *bufcopy = NULL;

    if (argc < 4) {
        fprintf(stderr, "usage: lrhost CORE.so SYSTEMDIR GAME.cue [--pad S] [--frames N] [--shot L] "
                        "[--ramdump L] [--trace SPEC OUT] [--zerocheck LO-HI] [-v]\n");
        return 2;
    }
    g_sysdir = argv[2];
    for (int i = 4; i < argc; i++) {
        if (!strcmp(argv[i], "--pad") && i + 1 < argc) pad = argv[++i];
        else if (!strcmp(argv[i], "--frames") && i + 1 < argc) frames = strtoul(argv[++i], NULL, 0);
        else if (!strcmp(argv[i], "--shot") && i + 1 < argc) shots = argv[++i];
        else if (!strcmp(argv[i], "--ramdump") && i + 1 < argc) dumps = argv[++i];
        else if (!strcmp(argv[i], "--trace") && i + 2 < argc) { spec = argv[++i]; out = argv[++i]; }
        else if (!strcmp(argv[i], "--zerocheck") && i + 1 < argc) zc = argv[++i];
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

    for (g_vbl = 0; g_vbl < frames; g_vbl++) {
        p_run();
        if (spec && !hooked && r32(ram, HOOK_ADDR) == HOOK_INSN0 && r32(ram, HOOK_ADDR + 4) == HOOK_INSN1 &&
            r32(ram, 0x24254) == 0x0C01E6C0u /* jal VSync: SLUS_008.42 is loaded */) {
            install_stub(ram);
            hooked = 1;
            lastseq = 0;
            free(stubcopy);
            stubcopy = malloc(BUF_BASE - STUB_BASE);
            memcpy(stubcopy, ram + STUB_BASE, BUF_BASE - STUB_BASE);
            free(bufcopy);
            bufcopy = malloc(4 + g_bufsize);
            memcpy(bufcopy, ram + BUF_BASE, 4 + g_bufsize);
            fprintf(stderr, "lrhost: hook installed at vblank %u\n", g_vbl);
        }
        if (hooked && r32(ram, HOOK_ADDR) != JAL(0x80000000u | STUB_BASE)) {
            fprintf(stderr, "lrhost: hook overwritten at vblank %u\n", g_vbl);
            hooked = 0;
        }
        if (hooked && memcmp(stubcopy, ram + STUB_BASE, BUF_BASE - STUB_BASE)) {
            fprintf(stderr, "lrhost: stub/table overwritten at vblank %u - unhooked\n", g_vbl);
            w32(ram, HOOK_ADDR, HOOK_INSN0);
            w32(ram, HOOK_ADDR + 4, HOOK_INSN1);
            hooked = 0;
        }
        /* the buffer changing with no new sequence number: normally the stub is half-way through its copy as the
         * vblank ends (the seq word is bumped one vblank later); if it is still not bumped a vblank later,
         * something else writes there */
        if (hooked && r32(ram, BUF_BASE) == lastseq && memcmp(bufcopy, ram + BUF_BASE, 4 + g_bufsize)) {
            if (changing) {
                fprintf(stderr, "lrhost: the snapshot buffer changed outside the hook at vblank %u\n", g_vbl);
                nclobber++;
            }
            changing = 1;
            memcpy(bufcopy, ram + BUF_BASE, 4 + g_bufsize);
        } else
            changing = 0;
        if (hooked && r32(ram, BUF_BASE) != lastseq) {
            uint32_t seq = r32(ram, BUF_BASE);
            if (seq != lastseq + 1)
                fprintf(stderr, "lrhost: missed %u snapshot(s) before vblank %u\n", seq - lastseq - 1, g_vbl);
            lastseq = seq;
            fwrite(&seq, 4, 1, fo);
            fwrite(&g_vbl, 4, 1, fo);
            fwrite(ram + BUF_BASE + 4, 1, g_bufsize, fo);
            memcpy(bufcopy, ram + BUF_BASE, 4 + g_bufsize);
            nrec++;
        }
        if (zmask && (zhi - zlo <= 0x10000 || g_vbl % 8 == 0))
            for (uint32_t a = zlo; a < zhi; a += 4)
                if (r32(ram, a) && !(hooked && a >= STUB_BASE && a < BUF_END))
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
    fprintf(stderr, "lrhost: %u vblanks, %u snapshots, %u buffer clobbers\n", g_vbl, nrec, nclobber);
    return 0;
}
