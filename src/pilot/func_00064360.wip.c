/* run4: not re-attacked (three independent causes: cz/cx global-alloc race, mult issue order, andi 0xffff).
   Untested idea for the andi: combine forgets nonzero_bits for a pseudo that is live at the start of the function
   or set through a SUBREG/partial store, so the original index may have come from such a value. */
/* run3-r4 (r4-cell, no improvement; 97.07 kept). Measured with cc1 -dl -dg on the shared-cz form (95.49):
   cz = 21 refs / 75 insns, cx = 21 / 94; px/pz are block-12 locals that local-alloc puts in s1/s0 first, so the
   first global allocno takes s2 - cz wins it. Tried without a flip: cz/cx uses at the end via obj->cz /
   D_0009774C reloads (81 combos, cse folds them back), `cx = a; cx += b` in top+both arms (27 refs but length
   grows to 106), cx-only or cz-only in the flag arms (cz drops to 15/42 - length shrinks with refs), do{}while(0)
   around single cx statements, sibling in-place GetCellType forms (89.4) and the 2D [z][x] index (85-89).
   andi 0xffff: multi-set `t` (t = c[1]; t &= ..; t >>= 9; t |= ..; 64 variants incl. short/int), int* + narrowing,
   and small struct/union/bitfield locals all fold it too. */
/* run3-r4 (r4-09, no improvement; 97.07 kept). Diagnosed the three remaining causes separately:
   (1) ALLOC: with one shared cz (target: top cz and tail cz both s3) cz = 21 refs/75 insns beats cx = 21/94, so cz
       takes s2. Lengths come partly from RTL order, not only sched1: writing the lookup directly on cx/cz with the
       x part first gives cz 80 / cx 84 (still loses, and adds 6 instrs). Fakematch that flips it (permuter, 96.21):
       a third identical copy `if (cz) {tail} else {tail}` inside the flag arm (cz live across the middle).
   (2) SCHED: target issues `mult cx,magic` only after zq is complete; ours hoists it right after zq's mfhi. No
       statement order (240 orders x q60() routing of either divide), 2D-index form or in-place r3-cell form changes
       it. A sched barrier (`do {} while (0);` after zq = z / 60) does move the mult to the target spot, but shifts
       the magic constant to a1 (+1 insn) - so the original probably had a real block/loop boundary there.
   (3) andi 0xffff: combine folds every zero-extend of the index (nonzero_bits); only an asm("" : "=r"(t) : "0"(t))
       on the index keeps it, i.e. the original's index came from somewhere combine could not see through. */
/* WIP 97.07 (run2-r3 r3-cell). func_00064360 - no Driver 2 counterpart.
   Found: `cx = sum; cx /= 1500;` (sum lands in the cx s-reg) and in GetCellType `xq = x / 60` placed between
   zoff and sel (95.49 with one shared cz). This file uses a separate top-level `tz` (97.07, but the target keeps the
   top cz in s3 = same pseudo as the tail cz, so tz is probably not the original). Remaining with shared cz:
   global-alloc priority - cz (21 refs / 75 insns) outranks cx (21 / 94) and takes s2; target has cx in s2, cz in s3.
   Tail statement order, decl order, extra refs (cx = a; cx += b) and obj->cz re-reads did not flip it.
   Also missing: `andi 0xffff` on the D_0008BA0C index (ushort local / ushort-returning inline / short *c all fold it). */
extern int D_0008BA0C[];
extern long long D_00097374;
extern int D_0009770C;
extern int D_00097724;
extern int D_00097738;
extern int D_0009773C;
extern int D_00097748;
extern int D_0009774C;
extern int car_data[];
extern int (*D_0009D8F4[])[60];
extern int D_0009E714[];
extern int func_000641C0();
extern void func_00061EDC();

typedef struct {
    char pad0[0x48];
    int x;
    int z;
    int cx;
    int cz;
    char pad58[0x75C - 0x58];
    int px;
    int py;
    int pz;
} OBJ;

static inline int GetCellType(int x, int z)
{
    int zq, zoff, sel, xq, xoff;
    unsigned short *c;

    zq = z / 60;
    zoff = (z % 60) * 240;
    xq = x / 60;
    sel = ((*(int *)&D_00097374 - zq) << 1) & 2;
    xoff = (x % 60) * 4;
    c = (unsigned short *)((char *)D_0009D8F4[sel | (xq & 1)] + zoff + xoff);
    return D_0008BA0C[(unsigned short)(((c[1] & 0x3C00) >> 9) | (c[2] & 1))];
}

void func_00064360(OBJ *obj, int *dx, int *dz, int force)
{
    int tx, tz;
    int cz;
    int cx;
    int px;
    int pz;
    int flag;
    int v;
    int *car;

    car = car_data;
    tz = D_0009E714[3] - (car[52] - 750);
    tz /= 1500;
    cx = car[50] + D_0009E714[2];
    cx /= 1500;
    v = GetCellType(cx, tz);
    flag = 0;
    obj->x = D_00097738;
    obj->z = D_0009773C;
    if (force == 0) {
        if (D_0009770C > 0 && (D_00097724 == 0 || v != 0x1000)) {
            D_00097724 = 1;
            D_00097738 = obj->x = car[50];
            D_0009773C = obj->z = car[52];
        } else {
            flag = 1;
        }
        if (func_000641C0(obj))
            flag = 1;
    } else {
        obj->x = car[50];
        obj->z = car[52];
    }
    if (flag) {
        cx = obj->x + D_0009E714[2];
        cz = D_0009E714[3] - (obj->z - 750);
        cx = obj->cx = cx / 1500;
        cz = obj->cz = cz / 1500;
    } else {
        cx = obj->x + D_0009E714[2];
        cz = D_0009E714[3] - (obj->z - 750);
        cx = obj->cx = cx / 1500;
        cz = obj->cz = cz / 1500;
    }
    px = (obj->px + D_0009E714[2]) / 1500;
    pz = (D_0009E714[3] - (obj->pz - 750)) / 1500;
    D_00097748 = cx;
    D_0009774C = cz;
    func_00061EDC(cx, cz, obj);
    *dx = cx - px;
    *dz = cz - pz;
}
