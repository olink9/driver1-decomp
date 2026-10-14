extern long long D_000970D8;
extern unsigned char D_000BDD40[];
int func_0004FFE8(int a0) {
    int x = *(int *)&D_000970D8;
    return D_000BDD40[(x + a0) & 0xff] | (D_000BDD40[(x - a0) & 0xff] << 8);
}
