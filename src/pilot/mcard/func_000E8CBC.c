extern void VSync();
extern int func_00078E4C();
extern void func_00077ED8();
extern void func_000E8DD4();

int func_000E8CBC(int port, int flag)
{
    int a[2];
    int b[2];
    int n;
    int arg;
    int r;
    int p16 = port << 4;

    b[0] = 0;
    b[1] = 0;
    if (flag) {
        a[0] = 0;
        a[1] = 0;
        n = 999;
        do {
            VSync(0);
            if (func_00078E4C(0, &a[0], &a[1]))
                break;
        } while (--n != -1);
        func_00077ED8(p16);
        arg = 0;
    } else {
        arg = 1;
    }
    r = func_00078E4C(arg, &b[0], &b[1]);
    switch (r) {
    case 0:
        return 0;
    case -1:
        func_00077ED8(p16);
        return 0;
    case 1:
        if (b[0] == 1 && b[1]) {
            func_000E8DD4(port);
            return 1;
        }
        return 0;
    default:
        return 0;
    }
}
