extern char D_000C4E38[];
extern int D_00097764;
extern long long D_00096FC8;
extern void func_00012498();
extern void func_0001548C();

static inline int find(void)
{
    int i;
    char *p = D_000C4E38;
    for (i = 0; i < 4; i++, p += 0x7A8) {
        if (*(int *)(p + 0x78C) == 0) {
            func_00012498(p, 0x7A8);
            return i;
        }
    }
    return -1;
}

char *func_00063B74(char *a0, int a1)
{
    char *r = 0;
    int k = find();
    short idx = k;

    if (idx != -1) {
        r = D_000C4E38 + idx * 0x7A8;
        a0[5] = k + 1;
        func_0001548C(a1, r);
        *(int *)&D_00096FC8 = 0;
        D_00097764 = D_00097764 + 1;
    }
    return r;
}
