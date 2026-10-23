extern char D_001E35A0[];

void func_001E22CC(void)
{
    int i;
    char *p;

    i = 8;
    p = D_001E35A0;
    p += 8;
    for (; i >= 0; i--)
        *p-- = 0;
}
