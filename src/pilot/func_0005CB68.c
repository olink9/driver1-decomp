/* r4-01 (run3-r4): matched. Fix: the call argument is a single-set pointer b = buf, and the loop pointer p = b is a second pseudo. A single-set pseudo gets sched1 birthing boost (p, set twice, did not), so buf is computed after &rect as in the target; global alloc ties p to b (s0). */
typedef struct { short x, y, w, h; } RECT;
extern void func_000766A8(), func_0007D608();
void func_0005CB68(unsigned int pos, unsigned short mask, unsigned char idx)
{
    int y, x;
    unsigned short buf[16];
    RECT rect;
    unsigned short *p;
    unsigned short *b = buf;
    RECT *r = &rect;
    int i;
    x = (pos & 0x3F) * 16;
    y = (unsigned short)pos >> 6;
    rect.x = x;
    rect.y = y;
    r->w = 16;
    r->h = 1;
    func_000766A8(r, b);
    for (p = b, i = 1; p < buf + 16; p++, i++) {
        if (((unsigned short)mask >> i) & 1)
            *p |= 0x8000;
        else
            *p &= 0x7FFF;
    }
    buf[idx & 0xFF] = 0;
    func_0007D608(buf, x, y);
}
