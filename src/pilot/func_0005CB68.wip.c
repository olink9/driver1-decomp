/* r3-11 (run2-r3): only diff = sched1 order of 'p = buf' (s0) vs 'r = &rect' (s1) in the prologue. -dS: at T-17 the a1-param copy (insn 8, anti-dep of 'a1 = p') carries the 0x7f000001 boost and 'p = buf' does not, so p lands first. No effect from: all 720 orders of the setup statements, decl order, mask param types (int/short/K&R/local copy), func(r, buf) / func(&rect, p), p init after call, loop bound via r, static inline call wrapper. */
typedef struct { short x, y, w, h; } RECT;
extern void func_000766A8(), func_0007D608();
void func_0005CB68(unsigned int pos, unsigned short mask, unsigned char idx)
{
    int y, x;
    unsigned short buf[16];
    RECT rect;
    unsigned short *p = buf;
    RECT *r = &rect;
    int i;
    x = (pos & 0x3F) * 16;
    y = (unsigned short)pos >> 6;
    rect.x = x;
    rect.y = y;
    r->w = 16;
    r->h = 1;
    func_000766A8(r, p);
    for (i = 1; p < buf + 16; p++, i++) {
        if (((unsigned short)mask >> i) & 1)
            *p |= 0x8000;
        else
            *p &= 0x7FFF;
    }
    buf[idx & 0xFF] = 0;
    func_0007D608(buf, x, y);
}
