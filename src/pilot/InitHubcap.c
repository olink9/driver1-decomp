extern int D_000973DC;
extern int D_000B700C[];
extern int func_0004FFE8();
void InitHubcap(void) {
    int *p;
    D_000973DC = func_0004FFE8(1) & 0x7FF;
    p = D_000B700C;
    p[3] = 1;
    p[2] = 1;
    p[1] = 1;
    D_000B700C[0] = 1;
    p[4] = 0xCD;
    p[5] = 20;
    p[6] = 0x122;
    p[8] = 0xCD;
    p[9] = 20;
    p[10] = -0x113;
    p[12] = -0xCD;
    p[13] = 20;
    p[14] = 0x122;
    p[16] = -0xCD;
    p[17] = 20;
    p[18] = -0x113;
}
