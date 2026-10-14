/* FAKEMATCH: empty __asm__ used as a compiler hint (no instruction emitted) */
/* Driver 2: InitSpooling */
extern short D_0009728C[];
extern volatile int D_00097234;
extern int D_000972D4, D_00097244, D_00097268, D_000972B4, D_00097250, D_0009727C;
extern int D_000972BC, D_000972A0, D_000972A8, D_000972A4, D_00097248, D_00097260, D_000972D8, D_000972B0;
extern void func_00034204();

void func_000337D4(void) {
    int i;
    int z;
    char pad[0x88];
    for (i = 0; i < 4; i++) {
        D_0009728C[i] = -1;
        func_00034204(i);
    }
    D_000972D4 = 0;
    __asm__ volatile("");
    D_00097234 = 0;
    z = D_00097234;
    D_00097244 = 0;
    D_00097268 = 0;
    D_000972B4 = 0;
    D_00097250 = 0;
    D_0009727C = 0;
    D_000972BC = 0;
    D_000972A0 = 0;
    D_000972A8 = 0;
    D_000972A4 = 0;
    D_00097248 = 0;
    D_00097260 = 0;
    D_000972D8 = 0;
    D_000972B0 = z;
}
