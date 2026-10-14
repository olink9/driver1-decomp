void func_0006E39C(unsigned *o, int t, unsigned *s) {
    unsigned v = s[2];
    switch (t) {
    case 0x11:
        o[0] = v & 0x1FFFFFF;
        o[3] = v >> 25;
        break;
    case 0x12:
        o[0] = v;
        break;
    case 0x13:
        o[0] = v;
        break;
    case 0x16:
        o[0] = v;
        o[2] = s[0x11];
        break;
    }
}
