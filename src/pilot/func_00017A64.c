extern unsigned char D_00096FC4;
extern unsigned char D_00099AF0[];
int func_00017A64(void) {
    unsigned char i = D_00096FC4;
    if (i == 0x16)
        return -1;
    D_00096FC4 = i + 1;
    return D_00099AF0[i];
}
