extern void VSync();
extern int func_00078E4C();
extern int func_00078798();
extern void func_000EAE3C();
extern char D_000E676C[];

int func_000E7F5C(int port, int b, int c, int d)
{
    int r[2];
    int n;
    int ret;

    r[0] = 0;
    r[1] = 0;
    n = 999;
    do {
        VSync(0);
        if (func_00078E4C(0, &r[0], &r[1]))
            break;
    } while (--n != -1);
    if (func_00078798(port << 4, b, c, 0, d) == 1) {
        n = 999;
        r[0] = 0;
        r[1] = 0;
        do {
            VSync(0);
            ret = func_00078E4C(0, &r[0], &r[1]);
            if (ret)
                break;
        } while (--n != -1);
        if (ret == 1)
            return 1;
    }
    func_000EAE3C(D_000E676C, 2);
    return 0;
}
