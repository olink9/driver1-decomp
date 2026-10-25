/* func_001C6990 - position the two volume bars (frame + fill scaled by the volume). The fill width source
   `s = u1 - u0` is its own statement after the frame's setXYWH (inline in the scale expression the loads interleave
   u1,u0,u1,u0 and the regs swap). */
#include "common.h"

extern int D_00096C90, D_00096C94, D_00096C98, D_00096C9C;
extern unsigned char D_001CE9E0;
extern POLY_FT4 D_001D2C70[2], D_001D2CC0[2];

void func_001C6990(void)
{
    int w, s;

    if (D_00096C9C != 0 || D_001CE9E0 != 0) {
        setXYWH(&D_001D2C70[0], 0xE2, 0xE1, D_001D2C70[0].u1 - D_001D2C70[0].u0, D_001D2C70[0].v2 - D_001D2C70[0].v0);
 s = D_001D2C70[0].u1 - D_001D2C70[0].u0;
 w = ((D_00096C94 + 10000) * ((s << 12) / 10000)) >> 12;
 setXYWH(&D_001D2CC0[0], w + 0xE3, 0xE1, D_001D2CC0[0].u1 - D_001D2CC0[0].u0, D_001D2CC0[0].v2 - D_001D2CC0[0].v0);
    }
    if (D_00096C98 != 0 || D_001CE9E0 != 0) {
        setXYWH(&D_001D2C70[1], 0xE2, 0xC8, D_001D2C70[1].u1 - D_001D2C70[1].u0, D_001D2C70[1].v2 - D_001D2C70[1].v0);
 s = D_001D2C70[1].u1 - D_001D2C70[1].u0;
 w = ((D_00096C90 + 10000) * ((s << 12) / 10000)) >> 12;
 setXYWH(&D_001D2CC0[1], w + 0xE3, 0xC8, D_001D2CC0[1].u1 - D_001D2CC0[1].u0, D_001D2CC0[1].v2 - D_001D2CC0[1].v0);
    }
}
