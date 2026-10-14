/* FAKEMATCH: empty __asm__ used as a compiler hint (no instruction emitted) */
extern unsigned short D_0008D878[];
unsigned short func_0006F81C(unsigned int a, unsigned short b)
{
    int q, x, i, lo, d;
    a &= 0xff;
    q = a / 12;
    x = b >> 4;
    i = x + (unsigned char)(a - q * 12) * 8;
    lo = D_0008D878[i];
    d = 15;
    __asm__("" : "+r"(d));
    return (short)(lo + x * (short)(D_0008D878[i + 1] - lo) / d) >> q;
}
