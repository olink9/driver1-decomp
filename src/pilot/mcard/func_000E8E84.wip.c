/* WIP 91.1: logic right; gcc hoists the 0x82/0x81 constants (t4/t3) out of the loop and copies c through a 2nd reg
   (target: in-place `andi v1,v1,0xff`, li in the bne delay slots). if-chain, switch, int/uchar/uint c all tried */
typedef struct { unsigned char k; char p; unsigned char v; char q; } KV;
extern KV D_000ED7E8[];

/* SJIS (0x82 / 0x81 lead) -> ASCII name copy, max 32 chars */
void func_000E8E84(unsigned char *src, unsigned char *dst)
{
    unsigned char i;
    unsigned char j;
    unsigned char c;
    int o;
    int k;

    for (i = 0; i < 32; i++) {
        o = 0x3F;
        c = *src++;
        if (c == 0)
            break;
        if (c == 0x82) {
            c = *src++;
            if ((unsigned char)(c - 0x4F) < 10 || (unsigned char)(c - 0x60) < 26)
                o = c - 0x1F;
            else if ((unsigned char)(c + 0x7F) < 26)
                o = c - 0x20;
        } else if (c == 0x81) {
            k = *src++;
            for (j = 0; j < 33; j++) {
                if (k == D_000ED7E8[j].k)
                    o = D_000ED7E8[j].v;
            }
        }
        *dst++ = o;
    }
    *dst = 0;
}
