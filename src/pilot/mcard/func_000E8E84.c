/* FAKEMATCH: the no-op `c &= 0xff;` after the byte load only reproduces the in-place andi. */
typedef struct { unsigned char k; char p; unsigned char v; char q; } KV;
extern KV D_000ED7E8[];

/* SJIS (0x82 / 0x81 lead) -> ASCII name copy, max 32 chars. The outer loop is a goto loop (a for loop lets loop.c
   hoist the 0x82/0x81 compare constants); `c &= 0xff` is a no-op on the byte but is what leaves the in-place andi. */
void func_000E8E84(unsigned char *src, unsigned char *dst)
{
    unsigned char i;
    unsigned char j;
    int c;
    int o;
    int k;
    KV *t;

    i = 0;
    t = D_000ED7E8;
loop:
    o = 0x3F;
    c = *src++;
    c &= 0xff;
    if (c == 0)
        goto end;
    if (c == 0x82) {
        c = *src++;
        if ((unsigned char)(c - 0x4F) < 10 || (unsigned char)(c - 0x60) < 26)
            o = c - 0x1F;
        else if ((unsigned char)(c + 0x7F) < 26)
            o = c - 0x20;
    } else if (c == 0x81) {
        c = *src++;
        k = c;
        for (j = 0; j < 33; j++) {
            if (k == t[j].k)
                o = t[j].v;
        }
    }
    *dst++ = o;
    i++;
    if (i < 32)
        goto loop;
end:
    *dst = 0;
}
