int func_0006E4CC(unsigned int a, unsigned int *b) {
    unsigned int *end = b + 15;
    int n = 0;
    while (b < end) {
        unsigned int v = b[2];
        int r;
        if ((v & 0xFE000000) < (a & 0xFE000000)) r = -1;
        else if ((a & 0xFE000000) < (v & 0xFE000000)) r = 1;
        else {
            unsigned int al = a & 0x1FFFFFF;
            unsigned int vl = v & 0x1FFFFFF;
            if (vl < al) r = -1;
            else if (al < vl) r = 1;
            else r = 0;
        }
        if (r == -1) break;
        b += 3;
        n++;
    }
    if (b == end) return -1;
    return n;
}
