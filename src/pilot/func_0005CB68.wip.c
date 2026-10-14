typedef struct { short x, y, w, h; } RECT;
extern void func_000766A8(), func_0007D608();
void func_0005CB68(unsigned int pos, unsigned int mask, unsigned int idx)
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
