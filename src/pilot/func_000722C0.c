// JCalcPat
int func_000722C0(unsigned char *j) {
    unsigned char b;
    int ret = 1;
    b = *j;
    if (b == 0x80) return ret;
    if ((b & 0x80) != 0) {
        if ((b & 1) != 0) ret++;
        if ((b & 2) != 0) ret++;
        if ((b & 4) != 0) ret++;
        if ((b & 8) != 0) ret++;
        if ((b & 16) != 0) ret++;
    } else ret = 5;
    return ret;
}
