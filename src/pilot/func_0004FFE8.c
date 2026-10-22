extern long long CameraCnt;
extern unsigned char D_000BDD40[];
int func_0004FFE8(int a0) {
    int x = *(int *)&CameraCnt;
    return D_000BDD40[(x + a0) & 0xff] | (D_000BDD40[(x - a0) & 0xff] << 8);
}
