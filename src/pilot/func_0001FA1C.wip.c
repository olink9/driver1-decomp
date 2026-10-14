extern char D_0009B0A4[];
extern unsigned char D_00096898[];
extern int D_0009700C, D_00097010, D_00097014, D_00097018, D_00097034;
extern void func_000774C4();
extern int func_000776E4();
extern void func_0001B4A4();

void func_0001FA1C(void) {
    int buf[4];
    int i;
    char *p = D_0009B0A4;
    func_000774C4(buf, 0, 16);
    D_00097010 = 1;
    if (D_00096898[0] == 0) {
        D_00097018 = -((func_000776E4() & 0x3ff) + 0x2ee);
        D_00097014 = (func_000776E4() & 0x7ff) + 0x5dc;
    } else {
        D_00097018 = 0;
        D_00097014 = 0;
    }
    D_0009700C = 0;
    D_00097034 = 0;
    for (i = 0; i < 25; i++) {
        *p = i;
        if (i > 0 && i != 23) {
            p[2] = 1;
            func_0001B4A4(p, buf);
        }
        p += 0x18c;
    }
}
