extern char D_000868EC[], D_000869DC[], D_00086BBC[], D_00086D9C[];

char *func_0006E418(int a0, int a1)
{
    switch (a0) {
    case 0x11: return D_000869DC + a1 * 60;
    case 0x12: return D_00086BBC + a1 * 60;
    case 0x13: return D_000868EC + a1 * 60;
    case 0x16: return D_00086D9C + a1 * 72;
    }
    return 0;
}
