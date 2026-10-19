unsigned char *func_00035B5C(unsigned char *dest, unsigned char *src)
{
    unsigned char *d = dest + 0x7FFF;
    int c, t;
    unsigned char v;

    do {
        c = *(signed char *)src++;
        if (c & 0x80) {
            v = *src++;
            *d-- = v;
            do { *d-- = v; t = c; c++; } while (t);
        } else {
            do { *d-- = *src++; t = c; c--; } while (t);
        }
    } while (d >= dest);
    return src;
}
