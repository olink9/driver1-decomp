/* Driver 1 PC port - shared runtime of the translated asm modules (register file, div, break, jalr dispatch,
 * fake MIPS stack). See asm_pc.h. */
#include <stdio.h>
#include <stdlib.h>
#include "asm_pc.h"

MipsRegs R;

/* only carpoly_asm.s (frame + 2 stack arguments) and unpack_asm.s (register save frame) use $sp */
static uint32_t pc_asm_stack[256];

void pc_asm_enter(void)
{
    R.sp = (uint32_t)(uintptr_t)&pc_asm_stack[256 - 16];
}

void pc_div(uint32_t rs, uint32_t rt)
{
    int32_t n = (int32_t)rs, d = (int32_t)rt;

    if (d == 0) {
        /* R3000: no trap; lo = n >= 0 ? -1 : 1, hi = n */
        R.lo = n >= 0 ? 0xFFFFFFFFu : 1u;
        R.hi = (uint32_t)n;
    } else if (n == INT32_MIN && d == -1) {
        R.lo = 0x80000000u;
        R.hi = 0;
    } else {
        R.lo = (uint32_t)(n / d);
        R.hi = (uint32_t)(n % d);
    }
}

void pc_asm_break(uint32_t addr, int code)
{
    /* PC-PORT: on PS1 this is `break` inside a self-loop (a deliberate hang on corrupt model data) */
    fprintf(stderr, "driver1: asm break %d at 0x%05X (corrupt model data?)\n", code, addr);
    abort();
}

void pc_asm_dispatch(uint32_t tgt)
{
    if (pc_asm_mdraw_dispatch(tgt) || pc_asm_tile_dispatch(tgt) || pc_asm_gte_dispatch(tgt))
        return;
    fprintf(stderr, "driver1: asm jalr to unknown code address 0x%08X\n", tgt);
    abort();
}
