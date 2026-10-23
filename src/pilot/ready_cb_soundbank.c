/* ready_cb_soundbank */
typedef struct { int a, b; void *p; int c; } SBQ;
extern int D_000971F0, D_000971F4, D_000971F8, D_000971FC, D_00097200, D_0009720C, D_00097224, D_00097268, D_000972D8;
extern volatile int D_000969EC;
extern SBQ D_000A7BA4[];
extern void func_0007A4FC(), test_changemode(), func_00034808(), func_0007A580(), func_0007A27C();

void ready_cb_soundbank(unsigned char intr, unsigned char *result)
{
    int buf[2];
    if (intr == 1) {
        int n; SBQ *q;
        func_0007A4FC(D_000971F0, 0x200);
        D_000971F0 += 0x800;
        D_000971FC--;
        n = D_000971F8 - 1;
        D_000971F8 = n;
        D_000971F4++;
        if (D_000971FC == 0) {
            D_00097268++;
            q = &D_000A7BA4[D_000969EC];
            D_00097200 = intr;
            D_0009720C++;
            if (n == 0) {
                D_00097224 = intr;
                D_000969EC++;
                test_changemode();
            } else {
                D_000971FC = 2;
                if (n < 2) D_000971FC = n;
                D_000971F0 = (int)q->p + ((D_00097268 & 1) << 12);
            }
        }
    } else {
        if (*result & 0x10) func_00034808();
        D_000972D8 = 0x3C;
        func_0007A580(D_000971F4, buf);
        func_0007A27C(0x1B, buf);
    }
}
