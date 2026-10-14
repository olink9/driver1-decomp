extern int D_000971F0, D_000971F4, D_000971F8, D_00097200, D_000972D8, D_000969EC;
extern void func_0007A4FC(), func_00032DEC(), func_00034808(), func_0007A580(), func_0007A27C();
void func_000352D4(unsigned char a, unsigned char *b)
{
    char buf[8];
    if (a == 1) {
        func_0007A4FC(D_000971F0, 0x200);
        D_000971F0 += 0x800;
        D_000971F8--;
        D_000971F4++;
        if (D_000971F8 == 0) {
            D_00097200 = a;
            D_000969EC++;
            func_00032DEC();
        }
    } else {
        if (*b & 0x10) func_00034808();
        D_000972D8 = 0x3C;
        func_0007A580(D_000971F4, buf);
        func_0007A27C(0x1B, buf);
    }
}
