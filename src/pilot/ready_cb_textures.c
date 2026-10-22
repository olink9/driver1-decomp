/* Driver 2: ready_cb_textures */
typedef struct { int pad0, pad1, addr, pad3; } SPOOL;
extern SPOOL D_000A7BA4[];
extern volatile int D_000969EC;
extern int D_000971F0, D_000971F4, D_000971F8, D_000971FC, D_00097200;
extern int D_0009720C, D_00097220, D_00097268, D_000972D8;
extern void func_0007A4FC();
extern void func_0007A580();
extern void func_0007A27C();
extern void test_changemode();
extern void func_00034808();

void ready_cb_textures(unsigned char intr, unsigned char *result)
{
    char buf[8];
    SPOOL *cur;
    if (intr == 1) {
        func_0007A4FC(D_000971F0, 512);
        D_000971F0 += 0x800;
        D_000971FC--;
        D_000971F8--;
        D_000971F4++;
        if (D_000971FC != 0)
            return;
        cur = &D_000A7BA4[D_000969EC];
        if (D_0009720C)
            D_00097268++;
        D_00097200 = intr;
        D_0009720C++;
        if (D_000971F8 != 0) {
            D_000971FC = 4;
            D_000971F0 = cur->addr + (D_00097268 & 1) * 0x2000;
        } else {
            D_00097220--;
            D_000969EC++;
            if (D_00097220 != 0) {
                D_0009720C = 0;
                D_000971F8 = 17;
                D_000971FC = intr;
                D_000971F0 = cur->addr + 0x4000;
            } else {
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
