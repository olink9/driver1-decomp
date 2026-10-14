extern int func_0004A9F4(void *p);
int func_0006BCD8(char *a0) {
    int r;
    if (*(int *)(a0 + 0x108) >= 0x5B && func_0004A9F4(a0 + 0xC8) == 0)
        r = 1;
    else
        r = 0;
    return r;
}
