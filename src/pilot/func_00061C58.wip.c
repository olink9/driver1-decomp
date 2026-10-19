extern long long D_00097374;
extern int *D_0009D8F4[];
extern int D_000C039C[];

static inline void ConvRow(int *dst, int *src, int n)
{
    int v;
    while (n--) {
        v = *src++;
        *dst++ = ((v & 1) | ((v >> 25) & 0x1e)) << 24 | 0xffff;
    }
}

void func_00061C58(int x, int y)
{
    int ty, tx;
    int sel, row, i, w, n;
    int *base, *src;

    x -= 32;
    tx = x / 60;
    y -= 32;
    ty = y / 60;
    y -= ty * 60;
    sel = ((*(int *)&D_00097374 - ty) << 1) & 2;
    row = y * 60;
    x -= tx * 60;
    base = D_000C039C;

    if (x + 64 >= 60) {
        w = 60 - x;
        for (i = 0; i < 64; i++) {
            sel = (sel & 2) | (tx & 1);
            src = D_0009D8F4[sel] + row + x;
            if (w) ConvRow(base, src, w);
            base += 60 - x;
            src = D_0009D8F4[sel ^ 1] + row;
            n = x + 4;
            if (n) ConvRow(base, src, n);
            base += n;
            row += 60;
            if (i + y == 59) {
                row = 0;
                sel ^= 2;
            }
        }
    } else {
        for (i = 0; i < 64; i++) {
            sel = (sel & 2) | (tx & 1);
            ConvRow(base, D_0009D8F4[sel] + row + x, 64);
            base += 64;
            if (i + y == 59) {
                row = 0;
                sel ^= 2;
            } else {
                row += 60;
            }
        }
    }
}
