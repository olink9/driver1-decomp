extern int D_000A8024[];
extern short D_0009728C[];
extern unsigned char *D_000AA7E8[];
extern int D_000973A8;
extern void Getlong();
void func_0003A270(int idx, int val, int off, unsigned char *out)
{
    int hdr;
    unsigned char *base, *p, *q;
    int i, n, k;
    unsigned char c, b;
    if (D_000A8024[idx] != val || D_0009728C[idx] != -1) {
        for (i = 0; i < D_000973A8; i++)
            *out++ = 0;
    } else {
        base = D_000AA7E8[idx];
        Getlong(&hdr, base + off * 4);
        p = base + (hdr & 0x3FFFFFFF);
        if (p != base) {
            n = D_000973A8;
            if (n & 7)
                n++;
            while (n != 0) {
                c = *p++;
                n -= c * 8;
                while (c != 0) {
                    *out++ = 1; *out++ = 1; *out++ = 1; *out++ = 1;
                    *out++ = 1; *out++ = 1; *out++ = 1; *out++ = 1;
                    c--;
                }
                if (n == 0)
                    break;
                c = *p++;
                if (c == 0)
                    continue;
                do {
                    b = *p++;
                    for (k = 0; k < 8; k++) {
                        *out++ = b & 1;
                        if (--n == 0)
                            break;
                        b >>= 1;
                    }
                    c--;
                } while (c != 0);
            }
        } else {
            q = out;
            for (i = 0; i < D_000973A8; i++)
                *q++ = 1;
        }
    }
}
