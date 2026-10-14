void func_00068020(char *p, int a, unsigned char c)
{
    *(int *)(p + 0x2A0) += a << 14;
    if (c == 0) {
        p[0x2E2] = 60;
        p[0x2E1] += a;
    }
}
