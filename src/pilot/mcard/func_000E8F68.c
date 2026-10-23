typedef struct { unsigned short sjis; unsigned short ascii; } KV;
typedef struct { unsigned short base; unsigned short first; } RANGE;
extern KV D_000ED7E8[];
extern RANGE D_000ED7DC[];
extern void memset();

/* ASCII -> SJIS name copy (inverse of func_000E8E84) */
void func_000E8F68(unsigned char *src, unsigned char *dst)
{
    int i;
    int c;
    unsigned int t; /* offset into the punctuation table (low byte of code) */
    int k;
    unsigned int code;

    k = 0xFF;
    memset(dst, 0, 32);
    for (i = 0; i < 31; i++) {
        c = *src++;
        if (c == 0)
            break;
        code = 0;
        if (c >= 0x20 && c < 0x30)
            code = 1;
        else if (c >= '0' && c <= '9')
            k = 0;
        else if (c >= 0x3A && c < 0x41)
            code = 0xB;
        else if (c >= 'A' && c <= 'Z')
            k = 1;
        else if (c >= 0x5B && c < 0x61)
            code = 0x25;
        else if (c >= 'a' && c <= 'z')
            k = 2;
        else if (c >= 0x7B && c < 0x7F)
            code = 0x3F;
        else {
            *dst++ = 0x81;
            *dst = 0x40;
            dst++;
            continue;
        }
        t = code & 0xFF;
        if (t) {
            t += 0x1F;
            code = D_000ED7E8[c - t].sjis;
        }
        else
            code = D_000ED7DC[k].base + c - D_000ED7DC[k].first;
        *dst++ = code >> 8;
        *dst = code;
        dst++;
    }
}
