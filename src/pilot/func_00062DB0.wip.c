/* run4 (no improvement; 99.12 kept). (b) sched1 analysis (-dS): x's quotient subu (334) sets the multi-set x, so it
   gets no birthing boost and ties 348 at priority 13; luid sends 348 first, then the boosted z sra 345 lands after
   334, so 248/255 never overlap and both get a0. Tried: all 20 interleavings of the six x/z statements, 106 x/z
   merge forms x orders (x1/x2/x2b/x3 against z1/z2/z2b/z3), single-set quotient temps qx/qz (r1-r4), in-place
   `x = pos[0] + K; x /= 750` forms, switch/idx expression orders: none above 99.12. A single-set x makes x win the
   global race but then the x chain is boosted, the z mult is issued first and a0 conflicts with x (x pref a1). */
/* run3 r4-11c: 98.41 -> 99.12, REAL pass loop (no goto) - this is now the long-term base.
   Fixes from the r4-11b real-loop form (94.10): (1) `dx = c * 180; pos[0] = px + FIX(ns * -900 - dx);` - the c*180
   product as its own statement (permuter find; 94.10 -> 97.17) fixes car->x load position and the a1/a2 product regs;
   (2) pre-loop order from a def-use-safe sweep: ns, stepx, z0, c, len, px, stepz, dx, dz, pos0, pos1 - z0 computed
   EARLY lengthens its life so the hoisted D_000C039C base (REG_EQUIV, doubled length) outranks it in global-alloc
   (z0 t3 / base t0 as target) and its cam.z/car->z loads land before the pos[0] store (97.17 -> 99.12).
   Left (both pure allocation, schedule identical):
   (a) i-loop top: andi a1 vs v1. local-alloc (block 1): chain {i<<2, *5, <<4} = 3*12/8 = 4.5 beats the la
       D_00098950 qty (2*4/2 = 4), so it takes v0 and the andi pseudo ends in v1; target needs la first. Lengths/refs
       of both are fixed by the target's own insn order - unreachable with a block-local andi pseudo. Making it a
       global pseudo by reusing x (`x = (unsigned char)i; car = &D[x]; s = &S[x];`) gives 99.26 (fixes andi a1, not the
       v0/v1 la pair) - variable reuse for steering (r4-04), not kept; file build/r4-11c/r4-11c_99.26_xreuse.c.
   (b) inner loop: x/z a1/a0 swapped (target x a0, z a1; mfhi a2/a1). In local-alloc (block 8) 248 (x sra) and 255
       (z sra) both get a0 (non-overlapping lives); z (32 refs/17) is global-allocated before x (32/23) and takes a0
       by preference. The target has 255 in a1, i.e. a0 busy over z's sra life = a different sched1 order in that
       block. 246/253 (the hi pseudos) are local-alloc'd to HI, then reload spills HI (s7 saved = spill reg, as in
       target) and re-allocates them. 8 inner-loop form variants (split/merged -= m->x*2, -= 9936) all worse.
   Tried: z0 forms ((cam.z - car->z) + 750 reassociates; add750() keeps the target addiu-then-subu), 5! pre-loop
   orders x 3 z0 forms, slot/car index forms (unsigned char k, i & 0xFF, s before car, SLOT D_00098950[] = hoisted).
   Earlier history: */
/* run3 r4-11: 83.35 -> 98.41. Fixes: (1) the pass loop is goto-built (`again: ... if (++pass < 2) goto again;`) so
   loop.c leaves the pass-2 chain inside `if (pass)` and the D_000C039C lo_sum in the pass region, as in the target;
   (2) the pre-loop x is its own variable `px` (reusing the inner-loop x made it a global pseudo); (3) the inner loop
   does `x = (pos+K)/750 - m->x*2; ... x -= 9936;` (tied quotient reg); (4) pre-loop statement order from a
   permute_dep2 sweep (px, z0, ns, stepx, c, len, stepz, x1, pos0, pos1) - stops the rcossin_tbl+2 spill (s5 = s4+2);
   (5) `unsigned char bits` gives `or bits,cell` operand order.
   Left: pure global-alloc priority order of the long-lived t/s regs: target is len t5 > magic t6 > K t7 > i t8 >
   stepz t9 > x1 s0 > stepx s1; ours ranks i (9 refs/187 insns, .144) above len/stepz/magic/K, and stepx above x1.
   The goto loop gives the d loop depth 2 (a real pass loop, depth 3, would weight magic/K/len/stepz above i but
   loop.c then hoists the chain; jump-into-loop variant breaks layout). Permuter (2k iters) only found a do{}while(0)
   switch wrapper (98.55, FAKEMATCH, not kept). Earlier history: */
/* run2 r2-041: 80.73 -> run2-r3 r3-15: 83.35. Pass loop is now a real `for (pass...)` (goto loop lost the loop-depth
   weighting): prologue now matches the target's shape (s5 = s4+2, both mult constants in t-regs, s7 saved but unused =
   the hoisted D_000C039C high part allocated s7 then spilled by reload and rematerialised as lui v0).
   Left: loop.c hoists the pass-2 `z0 + FIX(ns*180 - c*900)` chain out of the pass loop (target recomputes it inside
   `if (pass)` from ns/c), which also shifts every t-reg by one. Tried: user-var copies of ns/c (cse propagates them),
   `z = ...` user var (only the last add stays), x1 inline in the if-block (prologue regresses to the old shape), an
   invalid pass loop (jump into it: same regression). 1500 statement-order swaps (perm2.py) found nothing above 83.35. */
typedef struct {
    char pad0[0xC8];
    int x;          /* 0xC8 */
    char pad1[4];
    int z;          /* 0xD0 */
    char pad2[0xF8 - 0xD4];
    int ang;        /* 0xF8 */
    char pad3[0x108 - 0xFC];
    int len;        /* 0x108 */
    char pad4[396 - 0x10C];
} CAR;
typedef struct {
    unsigned char b0;
    unsigned char active;
    char pad[3];
    unsigned char id;
    char pad2[80 - 6];
} SLOT;
typedef struct {
    int pad[2];
    int x;
    int z;
} CAM;
typedef struct { unsigned char c[4]; } CELL;
typedef struct {
    int id;
    int pad[0x13];
    int x;      /* 0x50 */
    int z;      /* 0x54 */
} MAP;

extern long long D_00098950;
extern CAR D_0009B230[];
extern CAM D_0009E714;
extern short rcossin_tbl[];
extern CELL D_000C039C[];

static inline int add750(int v) { return v + 750; }
#define FIX(a) (((a) + 2048) >> 12)

void func_00062DB0(MAP *m)
{
    int i;
    short *tb;
    int pass;
    int d;
    int len;
    int x, z;
    int px;
    int ns, c;
    int stepx, stepz;
    int dx;
    int z0;
    int dz;
    int idx;
    unsigned char bits;
    int pos[2];
    CAR *car;
    SLOT *s;

    for (i = 0; i < 22; i++) {
        car = &D_0009B230[(unsigned char)i];
        s = &((SLOT *)&D_00098950)[(unsigned char)i];
        if (s->active == 0)
            continue;
        if (s->id == m->id + 1)
            continue;
        ns = -rcossin_tbl[(car->ang & 0xFFF) * 2];
        stepx = FIX(ns * 750);
        z0 = add750(D_0009E714.z) - car->z;
        c = rcossin_tbl[(car->ang & 0xFFF) * 2 + 1];
        len = car->len * 20 + 450;
        px = car->x + D_0009E714.x;
        stepz = FIX(c * 750);
        dx = c * 180;
        dz = c * -900 - ns * 180;
        pos[0] = px + FIX(ns * -900 - dx);
        pos[1] = z0 + FIX(dz);
        for (pass = 0; pass < 2; pass++) {
            if (pass) {
                pos[0] = px + FIX(c * 180 + ns * -900);
                pos[1] = z0 + FIX(ns * 180 - c * 900);
            }
            for (d = -900; d < len; d += 750) {
                x = (pos[0] + 0x7270E0) / 750;
                z = (pos[1] + 0x7270E0) / 750;
                x -= m->x * 2;
                x -= 9936;
                z -= m->z * 2;
                z -= 9936;
                if ((unsigned)x < 128 && (unsigned)z < 128) {
                    idx = (x >> 1) + ((z >> 1) << 6);
                    switch ((z & 1) << 1 | (x & 1)) {
                    case 0:
                    default:
                        bits = 0x22;
                        break;
                    case 1:
                        bits = 0x24;
                        break;
                    case 2:
                        bits = 0x28;
                        break;
                    case 3:
                        bits = 0x30;
                        break;
                    }
                    D_000C039C[idx].c[3] = bits | D_000C039C[idx].c[3];
                }
                pos[0] += stepx;
                pos[1] += stepz;
            }
        }
    }
}



