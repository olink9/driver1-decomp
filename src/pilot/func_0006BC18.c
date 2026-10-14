extern char D_00099080[];
extern char D_0009B230[];
extern int func_000612FC();
int func_0006BC18(char *a)
{
    char *p;
    char *q;
    char *e;
    int r = 0;
    e = D_00099080;
    if (*(int *)(a + 0x108) >= 0x5B) {
        for (p = e - 0x730, q = D_0009B230; p < e; p += 0x50, q += 0x18C) {
            if (p[1] != 0 && q != a) {
                if (func_000612FC(a + 0xC8, q + 0xC8) < 500) { r = 1; break; }
            }
        }
    }
    return r;
}
