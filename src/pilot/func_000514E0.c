/* pad read: previous / current / newly pressed buttons */
extern void func_000357F0(void);
extern unsigned short D_0009752C, D_00097538, D_00097544;
extern unsigned short D_000A8018[];

void func_000514E0(void) {
    func_000357F0();
    if (D_0009752C & 0x2000)
        if (D_0009752C & 0x8000)
            D_0009752C &= 0x5FFF;
    D_00097544 = D_0009752C;
    D_0009752C = D_000A8018[0];
    D_00097538 = D_0009752C & (D_00097544 ^ D_0009752C);
}
