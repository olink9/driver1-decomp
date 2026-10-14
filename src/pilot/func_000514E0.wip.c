extern void func_000357F0(void);
extern unsigned short D_0009752C, D_00097538, D_00097544;
extern long long D_000A8018;
void func_000514E0(void) {
    unsigned short n, o, x;
    func_000357F0();
    if (D_0009752C & 0x2000)
        if (D_0009752C & 0x8000)
            D_0009752C &= 0x5FFF;
    o = D_0009752C;
    n = *(unsigned short *)&D_000A8018;
    D_00097544 = o;
    x = o ^ n;
    D_0009752C = n;
    D_00097538 = n & x;
}
