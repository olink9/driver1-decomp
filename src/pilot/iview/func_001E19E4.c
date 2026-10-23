extern unsigned char D_001E35A0[];

int func_001E19E4(int i)
{
    if (D_001E35A0[i] == 0) {
        D_001E35A0[i] = 1;
        return 1;
    }
    return 0;
}
