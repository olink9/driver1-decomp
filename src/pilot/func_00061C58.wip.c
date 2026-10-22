/* FAKEMATCH-steering (not matched): best 95.65 (run3-r4). `i = ty * 60; y -= i;` reuses the loop counter as a temp
   only to steer register allocation - real source most likely had `y -= ty * 60;` (93.51 with everything else here).
   Fixed this run: ConvRow(src, dst, n) parameter order (was dst, src) gives the target's inner-loop regs (src t0,
   n a2, dst a3) once x is in a0.
   Remaining: block-0 local-alloc. Target: magic a2, y-sign v1, ty/quotient chain v0, x in a0. Plain source: the ty
   chain is local-allocated to a0 (v0 = y-sign, v1 = magic), so global x loses a0 and goes to t0. Any change that makes
   the ty*60 product (or ty's last use) a global/extra pseudo frees a0 for x (95.65), but then y-sign gets v0 and the
   chain v1 - the chain never outranks the y-sign in local-alloc. Tried: 720+ header statement orders (with and
   without the i temp), declaration orders, %, (y-32)/60 forms, ty = D - ty forms (168 instrs), static inline
   helpers (no block split), separate px/py locals. Permuter (2 runs, ~10k iters) plateaus at the same chain/sign
   swap, only via casts like (unsigned char)ty (semantics-changing). */
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
    int ty, tx;
    int sel, row, i, w, n;
    int *base, *src;

    x -= 32;
    tx = x / 60;
    y -= 32;
    ty = y / 60;
    i = ty * 60;
    y -= i;
    sel = ((*(int *)&D_00097374 - ty) << 1) & 2;
    row = y * 60;
    x -= tx * 60;
    base = D_000C039C;

    if (x + 64 >= 60) {
        w = 60 - x;
        for (i = 0; i < 64; i++) {
            sel = (sel & 2) | (tx & 1);
            src = D_0009D8F4[sel] + row + x;
            if (w) ConvRow(src, base, w);
            base += 60 - x;
            src = D_0009D8F4[sel ^ 1] + row;
            n = x + 4;
            if (n) ConvRow(src, base, n);
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
            src = D_0009D8F4[sel] + row + x;
            ConvRow(src, base, 64);
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
