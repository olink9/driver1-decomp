/* Driver 1 PC port - lockstep trace (port/README.md "Lockstep comparison", tools/pc/compare/).
 *
 * Once per game frame, at the same point the emulator side (tools/pc/compare/lrhost.c) patches into the PS1 game
 * (main.c func_0002360C, right after ControlReplay, before the frame's VSync(-1) wait), copy a list of game
 * variables to a trace file. The same spec file drives both sides, so the records compare byte for byte.
 *
 *   D1_TRACE=<spec>:<out>   tools/pc/compare/trace.spec : file to write (off when unset)
 *
 * The point: __wrap_VSync (pc_main.c) calls PC_TraceVSync with its return address; the first call per game frame
 * that comes from func_0002360C is the first poll of that wait (nothing between ControlReplay and the poll
 * changes state). "Per game frame" = D_00097534, the loop's frame counter, has changed since the last record.
 *
 * Symbols: every spec entry names a symbol, looked up in this executable's own ELF symbol table (so C-owned
 * objects, the PS1-image names pc_gendata PROVIDEs at their PS1 addresses, and file-static variables such as
 * pc_psyq.c's rand seed all work without declarations here).
 *
 * File format (shared with lrhost): "DLC1", record size (8 + payload), region count, then per region
 * label[32] ps1addr size; then records: seq, vblank, payload (the regions back to back).
 */
#define _GNU_SOURCE
#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int __real_VSync(int mode);
extern int D_00097534;

#define MAXREG 63
static struct { char label[32]; unsigned ps1addr, size; const unsigned char *pc; } s_reg[MAXREG];
/* "*" entries (a PS1 address range): per-byte source, PS1-image bytes from PC_RAM, C-owned objects from the PC */
static const unsigned char **s_rangesrc[MAXREG];
static int s_nreg, s_state;            /* s_state: 0 = not set up, 1 = tracing, -1 = off */
static unsigned s_payload, s_seq;
static int s_lastframe = -1;
static unsigned long s_loop_lo, s_loop_hi;   /* func_0002360C */
static FILE *s_out;

/* this executable's .symtab: name -> (value, size) */
static char *s_elf;
static Elf32_Sym *s_syms;
static int s_nsyms;
static const char *s_strtab;

static int elf_load(void)
{
    FILE *f = fopen("/proc/self/exe", "rb");
    long n;
    Elf32_Ehdr *eh;
    Elf32_Shdr *sh;
    int i;
    if (!f)
        return -1;
    fseek(f, 0, SEEK_END);
    n = ftell(f);
    fseek(f, 0, SEEK_SET);
    s_elf = malloc(n);
    if (fread(s_elf, 1, n, f) != (size_t)n) {
        fclose(f);
        return -1;
    }
    fclose(f);
    eh = (Elf32_Ehdr *)s_elf;
    sh = (Elf32_Shdr *)(s_elf + eh->e_shoff);
    for (i = 0; i < eh->e_shnum; i++)
        if (sh[i].sh_type == SHT_SYMTAB) {
            s_syms = (Elf32_Sym *)(s_elf + sh[i].sh_offset);
            s_nsyms = sh[i].sh_size / sizeof(Elf32_Sym);
            s_strtab = s_elf + sh[sh[i].sh_link].sh_offset;
            return 0;
        }
    return -1;
}

static Elf32_Sym *elf_find(const char *name)
{
    Elf32_Sym *found = NULL;
    int i, n = 0;
    for (i = 0; i < s_nsyms; i++) {
        int t = ELF32_ST_TYPE(s_syms[i].st_info);
        if ((t == STT_OBJECT || t == STT_FUNC || t == STT_NOTYPE) && s_syms[i].st_name &&
            !strcmp(s_strtab + s_syms[i].st_name, name)) {
            if (!found)
                found = &s_syms[i];
            n++;
        }
    }
    if (n > 1)
        fprintf(stderr, "[trace] %s: %d symbols of that name, using the first\n", name, n);
    return found;
}

/* A "*" entry: the PS1 range [addr, addr+size) as the PS1 sees it. Every byte comes from PC_RAM(a) (pc_psxram is at
 * address 0, so that is just the address) unless the PS1 map puts a symbol there that the PC build defines elsewhere
 * (a C-owned object, ELF address != PS1 address): those bytes come from the PC object (its ELF size; a size-0 alias
 * only up to the next name).
 * The map: D1_TRACE_MAP, else <spec dir>/../../../build/slus_008.42.map. */
static int range_setup(int r, unsigned addr, unsigned size, const char *spec)
{
    char path[600], line[512];
    const char *m = getenv("D1_TRACE_MAP");
    unsigned j, *ps1 = NULL, nps1 = 0, cap = 0;
    char (*names)[64] = NULL;
    unsigned char *claimed = calloc(1, size);
    int pass;
    FILE *f;
    if (!m) {
        const char *sl = strrchr(spec, '/');
        snprintf(path, sizeof path, "%.*s/../../../build/slus_008.42.map", sl ? (int)(sl - spec) : 1, sl ? spec : ".");
        m = path;
    }
    if (!(f = fopen(m, "r"))) {
        perror(m);
        return -1;
    }
    while (fgets(line, sizeof line, f)) {
        unsigned a;
        char nm[64];
        if (sscanf(line, " 0x%x %63s", &a, nm) != 2 || a < addr || a >= addr + size)
            continue;
        if (!((nm[0] >= 'A' && nm[0] <= 'Z') || (nm[0] >= 'a' && nm[0] <= 'z') || nm[0] == '_'))
            continue;
        if (nps1 == cap) {
            cap = cap ? cap * 2 : 256;
            ps1 = realloc(ps1, cap * sizeof *ps1);
            names = realloc(names, cap * sizeof *names);
        }
        ps1[nps1] = a;
        strcpy(names[nps1], nm);
        nps1++;
    }
    fclose(f);
    /* plus the PC's own D_xxxxxxxx objects in the range the PS1 map has no label for */
    for (j = 0; j < (unsigned)s_nsyms; j++) {
        const char *nm = s_strtab + s_syms[j].st_name;
        unsigned a;
        char *e;
        if (strncmp(nm, "D_", 2) || strlen(nm) != 10)
            continue;
        a = strtoul(nm + 2, &e, 16);
        if (*e || a < addr || a >= addr + size)
            continue;
        if (nps1 == cap) {
            cap = cap ? cap * 2 : 256;
            ps1 = realloc(ps1, cap * sizeof *ps1);
            names = realloc(names, cap * sizeof *names);
        }
        ps1[nps1] = a;
        strcpy(names[nps1], nm);
        nps1++;
    }
    s_rangesrc[r] = malloc(size * sizeof *s_rangesrc[r]);
    for (j = 0; j < size; j++)
        s_rangesrc[r][j] = (const unsigned char *)(unsigned long)(addr + j);
    /* sized PC objects (C definitions) cover their whole size; size-0 names (owner+offset aliases, PS1-only labels
     * inside an object) only fill bytes no sized object claimed, up to the next name */
    for (pass = 0; pass < 2; pass++)
        for (j = 0; j < nps1; j++) {
            Elf32_Sym *s = elf_find(names[j]);
            unsigned end = addr + size, k, a;
            if (!s || s->st_value == ps1[j] || (pass == 0) != (s->st_size != 0))
                continue;
            if (s->st_size) {
                if (ps1[j] + s->st_size < end)
                    end = ps1[j] + s->st_size;
            } else {
                for (k = 0; k < nps1; k++)
                    if (ps1[k] > ps1[j] && ps1[k] < end)
                        end = ps1[k];
            }
            for (a = ps1[j]; a < end; a++) {
                if (pass == 1 && claimed[a - addr])
                    break;
                s_rangesrc[r][a - addr] = (const unsigned char *)(unsigned long)(s->st_value + (a - ps1[j]));
                claimed[a - addr] = 1;
            }
        }
    free(claimed);
    free(ps1);
    free(names);
    return 0;
}

static void setup(void)
{
    const char *env = getenv("D1_TRACE");
    char spec[512], line[256];
    const char *colon;
    FILE *f;
    Elf32_Sym *s;
    unsigned hdr[3];
    int i;

    s_state = -1;
    if (!env || !(colon = strchr(env, ':')) || colon - env >= (int)sizeof(spec))
        return;
    memcpy(spec, env, colon - env);
    spec[colon - env] = 0;
    if (elf_load()) {
        fprintf(stderr, "[trace] no symbol table in /proc/self/exe\n");
        return;
    }
    if (!(s = elf_find("func_0002360C")) || !s->st_size) {
        fprintf(stderr, "[trace] func_0002360C not found\n");
        return;
    }
    s_loop_lo = s->st_value;
    s_loop_hi = s->st_value + s->st_size;
    if (!(f = fopen(spec, "r"))) {
        perror(spec);
        return;
    }
    while (fgets(line, sizeof line, f)) {
        char label[64], name[128], *h = strchr(line, '#');
        unsigned addr, size;
        if (h)
            *h = 0;
        if (sscanf(line, "%63s %x %127s %u", label, &addr, name, &size) != 4)
            continue;
        if (s_nreg == MAXREG) {
            fprintf(stderr, "[trace] too many spec entries\n");
            break;
        }
        if (!strcmp(name, "*")) {
            if (range_setup(s_nreg, addr, size, spec)) {
                fclose(f);
                return;
            }
            snprintf(s_reg[s_nreg].label, 32, "%s", label);
            s_reg[s_nreg].ps1addr = addr;
            s_reg[s_nreg].size = size;
            s_payload += size;
            s_nreg++;
            continue;
        }
        if (!(s = elf_find(name))) {
            fprintf(stderr, "[trace] %s: symbol %s not found - entry skipped (the emulator side will not match)\n",
                    label, name);
            fclose(f);
            return;
        }
        if (s->st_size && s->st_size < size)
            fprintf(stderr, "[trace] %s: %s is %u bytes on PC, the spec reads %u\n", label, name,
                    (unsigned)s->st_size, size);
        snprintf(s_reg[s_nreg].label, 32, "%s", label);
        s_reg[s_nreg].ps1addr = addr;
        s_reg[s_nreg].size = size;
        s_reg[s_nreg].pc = (const unsigned char *)(unsigned long)s->st_value;
        s_payload += size;
        s_nreg++;
    }
    fclose(f);
    if (!(s_out = fopen(colon + 1, "wb"))) {
        perror(colon + 1);
        return;
    }
    hdr[0] = 0x31434C44u;   /* "DLC1" */
    hdr[1] = 8 + s_payload;
    hdr[2] = s_nreg;
    fwrite(hdr, 4, 3, s_out);
    for (i = 0; i < s_nreg; i++) {
        char lab[32] = { 0 };
        memcpy(lab, s_reg[i].label, strnlen(s_reg[i].label, 31));
        fwrite(lab, 1, 32, s_out);
        fwrite(&s_reg[i].ps1addr, 4, 1, s_out);
        fwrite(&s_reg[i].size, 4, 1, s_out);
    }
    fflush(s_out);
    fprintf(stderr, "[trace] %d regions, %u bytes per frame -> %s\n", s_nreg, s_payload, colon + 1);
    s_state = 1;
}

/* pc_main.c __wrap_VSync, game thread only, before anything else runs there */
void PC_TraceVSync(void *caller)
{
    unsigned vbl;
    int i;
    if (s_state == 0)
        setup();
    if (s_state < 0)
        return;
    if ((unsigned long)caller < s_loop_lo || (unsigned long)caller >= s_loop_hi || D_00097534 == s_lastframe)
        return;
    s_lastframe = D_00097534;
    s_seq++;
    vbl = __real_VSync(-1);
    fwrite(&s_seq, 4, 1, s_out);
    fwrite(&vbl, 4, 1, s_out);
    for (i = 0; i < s_nreg; i++) {
        if (s_rangesrc[i]) {
            unsigned j;
            for (j = 0; j < s_reg[i].size; j++)
                fputc(*s_rangesrc[i][j], s_out);
        } else
            fwrite(s_reg[i].pc, 1, s_reg[i].size, s_out);
    }
    fflush(s_out);   /* D1_QUIT leaves with _exit */
}
