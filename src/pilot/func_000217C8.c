extern unsigned int func_00021690(void);
int func_000217C8(int a, int b, short *out) {
    unsigned int v = func_00021690();
    if (v == 0) return 0;
    out[0] = (v >> 16) & 0x3FF;
    out[1] = v;
    out[2] = (v >> 30) << 10;
    return 1;
}
