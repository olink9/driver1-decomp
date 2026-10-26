/* Driver 1 PC port - runtime for the translated hand-written asm modules (port/pc/asm_*.c).
 *
 * The original src/<module>_asm.s routines are not C-ABI code: they pass values in t/s registers, fall through into each
 * other, jump into the middle of one another and dispatch through tables of code addresses that DrawMapPSX
 * writes into the scratchpad. The PC versions are therefore a mechanical translation that keeps that model:
 *  - one global MIPS register file R shared by every module (so register hand-offs between routines, and between
 *    consecutive C-called routines such as func_0002C5F0 -> func_0002C6DC (t3-t5), keep working);
 *  - each asm function is `static void F_<addr>(int entry)` running on R; `entry` selects an alternative entry
 *    point (an offset into the function) where the original is entered mid-way;
 *  - GTE access goes through PsyCross' software GTE (MTC2/MFC2/CTC2/CFC2/doCOP2), i.e. the same GTE state the
 *    game's C code uses through inline_c.h;
 *  - the C-callable symbols (func_XXXXXXXX) are thin wrappers that load a0-a3 (and stack args), run the body and
 *    return v0.
 * 32-bit build only: registers hold host pointers (see port/CMakeLists.txt, -m32).
 * Generator: ~/dev/driver1-work/pcbuild/B-scratch/mips2c.py (asm -> C bodies); the wrappers are hand-written. */
#ifndef ASM_PC_H
#define ASM_PC_H

#include <stdint.h>

typedef struct MipsRegs {
    uint32_t zero, at, v0, v1, a0, a1, a2, a3;
    uint32_t t0, t1, t2, t3, t4, t5, t6, t7;
    uint32_t s0, s1, s2, s3, s4, s5, s6, s7;
    uint32_t t8, t9, k0, k1, gp, sp, fp, ra;
    uint32_t hi, lo;
} MipsRegs;

extern MipsRegs R;

/* PsyCross software GTE (port/psycross/src/psx/INLINE_C.C) */
extern void MTC2(unsigned int value, int reg);
extern unsigned int MFC2(int reg);
extern void CTC2(unsigned int value, int reg);
extern unsigned int CFC2(int reg);
extern int doCOP2(int op);

/* the PS1 scratchpad (0x1F800000) on PC: section A's buffer (src/tile.c), shared with the game's C code */
extern unsigned char pc_scratchpad[];
#define PC_SCRATCHPAD_BASE ((uint32_t)(uintptr_t)pc_scratchpad)

/* The tpage/clut lookups (func_000268FC, func_00026BB8, func_0002C6B0, func_00030474, func_00030BF0) read
 * texture_cluts[page][clut] as texture_pages + 0x100 + ..., i.e. they rely on texture_cluts directly following
 * texture_pages (0x86F30 / 0x87030 on PS1). On PC the two arrays need not be adjacent, so those loads go through
 * TEXCLUT (same address when they are adjacent). */
extern char texture_pages[];            /* unsigned short[128] */
extern char texture_cluts[];            /* unsigned short[128][32] */
#define TEXCLUT(a) ((uint32_t)(a) - (uint32_t)(uintptr_t)texture_pages + (uint32_t)(uintptr_t)texture_cluts)

/* memory access on host addresses held in 32-bit registers */
#define MEMP(T, a) ((T *)(uintptr_t)(uint32_t)(a))
#define LW(a)  (*MEMP(uint32_t, a))
#define LH(a)  ((uint32_t)(int32_t)*MEMP(int16_t, a))
#define LHU(a) ((uint32_t)*MEMP(uint16_t, a))
#define LB(a)  ((uint32_t)(int32_t)*MEMP(int8_t, a))
#define LBU(a) ((uint32_t)*MEMP(uint8_t, a))
#define SW(a, v) (*MEMP(uint32_t, a) = (uint32_t)(v))
#define SH(a, v) (*MEMP(uint16_t, a) = (uint16_t)(v))
#define SB(a, v) (*MEMP(uint8_t, a) = (uint8_t)(v))

/* MIPS div rs, rt -> lo/hi (R3000 results for a zero divisor and 0x80000000 / -1, no trap) */
void pc_div(uint32_t rs, uint32_t rt);
/* `break` in the original (a hang/trap on PS1): log and abort */
void pc_asm_break(uint32_t addr, int code);
/* jalr through a code address (scratchpad dispatch tables written by DrawMapPSX): runs the routine at tgt */
void pc_asm_dispatch(uint32_t tgt);
/* per-module lookups used by pc_asm_dispatch: return 1 when tgt is one of the module's entry points */
int pc_asm_tile_dispatch(uint32_t tgt);
int pc_asm_mdraw_dispatch(uint32_t tgt);
int pc_asm_gte_dispatch(uint32_t tgt);
/* a fresh fake MIPS stack for a C -> asm call (only carpoly/unpack touch $sp) */
void pc_asm_enter(void);

/* ---- C-callable entry points (original names; args a0-a3 then stack, result v0) ---- */
/* unpack_asm.s */
int func_00026140(void *packed, void *dst);
void func_00026194(void);
/* tile_asm.s */
int func_00026794(void *cop, void *matrix);
int func_0002680C(void *cop);
void func_000268FC(void *verts, void *polys, void *prim, int otofs);
char *func_000269D4(void *model, char *prim, int otofs);
char *func_00026FF0(void *model, char *prim);
char *func_00027098(void *model, char *prim);
/* gte_asm.s */
void func_0002C53C(void);
void func_0002C564(void *v);
void func_0002C598(void *dst, void *src, int bytes);
int func_0002C5F0(void *pos, int radius);
int func_0002C6DC(void *model, void *prim, void *ot, int yang);
/* mdraw_asm.s */
int func_00030ED0(void *list, int n, void *db);
/* rle_asm.s */
void *func_00035B5C(void *dst, void *src);
/* carpoly_asm.s */
void func_0003DF94(int n, void *polys, void *verts, void *plot, int a4, int a5);
void func_0003E244(int n, void *polys, void *verts, void *plot, int a4, int a5);

/* register-convention fragments (no C arguments; they run on R). Exported so that their addresses can be put in
 * the scratchpad dispatch tables exactly as the PS1 code does. */
void func_0002686C(void); void func_00026878(void); void func_00026894(void); void func_000268B8(void);
void func_000268D8(void); void func_00026B94(void); void func_00026BA4(void); void func_00026BB8(void);
void func_00026BE8(void); void func_00026C18(void); void func_00026C38(void); void func_00026DD0(void);
void func_00026E54(void); void func_00026F28(void);
void func_0002C688(void); void func_0002C694(void); void func_0002C6B0(void);
void func_00030380(void); void func_000303E8(void); void func_00030464(void); void func_00030474(void);
void L00030520(void);    /* = .L00030520, the second entry of func_00030474 (DrawMapPSX takes its address) */
void func_0003056C(void); void func_000305D0(void); void func_00030718(void); void func_000307E8(void);
void func_000307FC(void); void func_00030BF0(void); void func_00030C20(void); void func_00030F60(void);
void func_00031048(void); void func_00031050(void); void func_00031074(void); void func_00031094(void);
void func_000310B8(void); void func_0003110C(void); void func_00031260(void); void func_000312C4(void);
void func_0003134C(void); void func_00031550(void); void func_000316C8(void); void func_00031734(void);

#endif
