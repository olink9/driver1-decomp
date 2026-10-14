extern short D_00086F30[];
extern short D_00087030[];
extern int D_0009757C;
extern void func_00035B5C();
extern void func_00074404();
extern short func_0007A7F4();
extern short GetTPage();

int func_000516E4(short *a0, short *a1, int a2, int *a3)
{
    int n;
    int i;
    short r[4];
    short *q;

    n = *a3;
    a3++;
    i = 0;
    if (n > 0) {
        q = D_00087030 + a2 * 32;
        do {
            func_00074404(a1, a3);
            a3 += 8;
            *q = func_0007A7F4(a1[0], a1[1]);
            a1[0] += 16;
            if (a1[0] == 0x400) {
                a1[0] = 0x3C0;
                a1[1] += 1;
            }
            i++;
            q++;
        } while (i < n);
    }
    r[0] = a0[0];
    r[1] = a0[1];
    r[2] = a0[2];
    r[3] = 0x100;
    func_00035B5C(0xD0200, a3);
    func_00074404(r, 0xD0200);
    D_00086F30[a2] = GetTPage(0, 0, a0[0], a0[1]);
    a0[0] += 0x40;
    if (a0[0] == 0x400) {
        a0[0] = 0x200;
        a0[1] += 0x100;
        if (a0[1] == 0x200) {
            D_0009757C = 100;
            a0[1] = 0x100;
        }
    }
    if (D_0009757C) {
        a0[0] = 0x380;
        a0[1] = 0x100;
    }
    return 1;
}
