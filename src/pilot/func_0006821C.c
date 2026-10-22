/* FAKEMATCH: empty __asm__ used as a compiler hint (no instruction emitted) */
struct O { char pad[0x11C]; unsigned int f11c; };
extern volatile long long CameraCnt;
extern int D_00097784;
extern int D_00097788;
extern char D_0009B16C[];
extern int func_00068054();

void func_0006821C(char *a0)
{
    if (*(unsigned int *)(a0 + 0x11C) & 0x10) {
        if (*(volatile int *)&CameraCnt != D_00097784) {
            D_00097784 = *(volatile int *)&CameraCnt;
            D_00097788 = func_00068054(a0, D_0009B16C) & 0xFF;
        }
        if (D_00097788 & 0xFF) {
            __asm__ volatile("");
            a0[0x2E2] = 60;
            *(int *)(a0 + 0x2A0) += 0x4000;
            a0[0x2E1]++;
        }
    }
}
