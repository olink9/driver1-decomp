/* WIP 97.17: block 2 (C98/CE8) matches; block 1 only a register swap: target gives the D_001D2CC0 base a1 and the scale's u1-u0 chain a3/v0, mine a2 and a1/a3 (local-alloc in the first block). Tried: pointer locals (worse), scale as temp/inline helper (either param order), w before setXYWH, 2-D array, decl order. */
/* Diagnosis (ov1-wip pass): the root is a sched1 LOAD ORDER in block 1 - target loads u1,u1,u0,u0 (both u1-u0 chains
   interleaved, as block 2 does), ours u1,u0,u1,u0; the regs follow. Block 2 gets it because its loads go through a
   cse'd base (a2 - 0x28) distinct from the store base; block 1 has one base for both. Tried here (no change): index via
   const local / i = 0 (const-propagated), (D + 0)[0], extra parens. Block 1's source probably differs from block 2. */
#include "common.h"

extern int D_00096C90, D_00096C94, D_00096C98, D_00096C9C;
extern unsigned char D_001CE9E0;
extern POLY_FT4 D_001D2C70[2], D_001D2CC0[2];

void func_001C6990(void)
{
    int w;

    if (D_00096C9C != 0 || D_001CE9E0 != 0) {
        setXYWH(&D_001D2C70[0], 0xE2, 0xE1, D_001D2C70[0].u1 - D_001D2C70[0].u0, D_001D2C70[0].v2 - D_001D2C70[0].v0);
        w = ((D_00096C94 + 10000) * (((D_001D2C70[0].u1 - D_001D2C70[0].u0) << 12) / 10000)) >> 12;
        setXYWH(&D_001D2CC0[0], w + 0xE3, 0xE1, D_001D2CC0[0].u1 - D_001D2CC0[0].u0, D_001D2CC0[0].v2 - D_001D2CC0[0].v0);
    }
    if (D_00096C98 != 0 || D_001CE9E0 != 0) {
        setXYWH(&D_001D2C70[1], 0xE2, 0xC8, D_001D2C70[1].u1 - D_001D2C70[1].u0, D_001D2C70[1].v2 - D_001D2C70[1].v0);
        w = ((D_00096C90 + 10000) * (((D_001D2C70[1].u1 - D_001D2C70[1].u0) << 12) / 10000)) >> 12;
        setXYWH(&D_001D2CC0[1], w + 0xE3, 0xC8, D_001D2CC0[1].u1 - D_001D2CC0[1].u0, D_001D2CC0[1].v2 - D_001D2CC0[1].v0);
    }
}
