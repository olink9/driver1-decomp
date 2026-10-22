/* Driver 2: unknown - copy a 64x64 window of the 60x60 cell-info quadrants (D_0009D8F4) around (x, y) into
   D_000C039C, packing each entry's bits 25-28 and bit 0 into the top byte. Row/column wrap crosses into the
   neighbouring quadrant (sel ^ 1 / sel ^ 2).
   Key (run3-r4): px/py are separate remainder locals (the loops never see the params), so the params are local
   to block 0 and local-alloc gives them their a0/a1 suggestions first; `ty = D_00097374 - ty` reuses the
   quotient as in the cell-lookup siblings. */
extern long long D_00097374;
extern int *D_0009D8F4[];
extern int D_000C039C[];

static inline void ConvRow(int *src, int *dst, int n)
{
    int v, b, c;
    while (n--) {
        v = *src++;
        b = v & 1;
        v = (v >> 25) & 0x1e;
        c = v | b;
        *dst++ = c << 24 | 0xffff;
    }
}

void func_00061C58(int x, int y)
{
    int ty, tx, px, py;
    int sel, row, i, w, n;
    int *base, *src;

    x -= 32;
    tx = x / 60;
    y -= 32;
    ty = y / 60;
    py = y - ty * 60;
    ty = *(int *)&D_00097374 - ty;
    sel = (ty << 1) & 2;
    row = py * 60;
    px = x - tx * 60;
    base = D_000C039C;

    if (px + 64 >= 60) {
        w = 60 - px;
        for (i = 0; i < 64; i++) {
            sel = (sel & 2) | (tx & 1);
            src = D_0009D8F4[sel] + row + px;
            if (w) ConvRow(src, base, w);
            base += 60 - px;
            src = D_0009D8F4[sel ^ 1] + row;
            n = px + 4;
            if (n) ConvRow(src, base, n);
            base += n;
            row += 60;
            if (i + py == 59) {
                row = 0;
                sel ^= 2;
            }
        }
    } else {
        for (i = 0; i < 64; i++) {
            sel = (sel & 2) | (tx & 1);
            src = D_0009D8F4[sel] + row + px;
            ConvRow(src, base, 64);
            base += 64;
            if (i + py == 59) {
                row = 0;
                sel ^= 2;
            } else {
                row += 60;
            }
        }
    }
}
