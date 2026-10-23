extern int D_00096858;
extern char D_000967BF;
extern long long ReplayParameterPtr;
extern int D_00096884;
extern int D_00096888;
extern long long D_00096780;
extern long long D_0009677C;
extern int D_000BE9DC[];
extern void func_0001AF2C();
extern void func_0001AECC();

void func_000244B8(void) {
    if (D_00096858 != 0) {
        D_000967BF = 0;
    }
    func_0001AF2C(0x17, *(int *)&ReplayParameterPtr + 4, *(short *)(*(int *)&ReplayParameterPtr + 0x10), (unsigned char)D_000967BF, 4);
    D_00096884 = 0x17;
    D_00096888 = 0;
    *(int *)&D_00096780 = 1;
    *(int *)&D_0009677C = 0;
    D_000BE9DC[0] = 1;
    func_0001AECC();
}
