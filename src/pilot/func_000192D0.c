int func_000192D0(unsigned char *p, int c) {
    unsigned char *q = p + 0x29;
    unsigned char *e = p + (p[0x26] + 0x29);
    while (q < e) {
        if (*q == c) return 1;
        q++;
    }
    return 0;
}
