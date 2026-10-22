/* ready_cb_regions (Driver 2) */
typedef struct { char p0[2]; unsigned short nsectors; char p1[4]; int addr; int pad; } SQ;
extern volatile int D_000969EC;
extern int D_000971F0, D_000971F4, D_000971F8, D_000971FC, D_00097200, D_00097224, D_000972D8;
extern SQ D_000A7BA4[];
extern void test_changemode(), func_00034808(), func_0007A27C(), func_0007A4FC(), func_0007A580();
void ready_cb_regions(unsigned char intr, unsigned char *result)
{
    char buf[8];
    SQ *q;
    if (intr == 1) {
        func_0007A4FC(D_000971F0, 512);
        D_000971F0 += 0x800;
        D_000971FC--;
        D_000971F8--;
        D_000971F4++;
        if (D_000971FC == 0) {
            D_000969EC++;
            D_00097200 = intr;
            if (D_000971F8 != 0) {
                q = &D_000A7BA4[D_000969EC];
                D_000971F0 = q->addr;
                D_000971FC = q->nsectors;
            } else {
                D_00097224 = intr;
                test_changemode();
            }
        }
    } else {
        if (*result & 0x10)
            func_00034808();
        D_000972D8 = 60;
        func_0007A580(D_000971F4, buf);
        func_0007A27C(27, buf);
    }
}
