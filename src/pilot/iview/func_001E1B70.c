extern unsigned char D_001E35A0[];
extern unsigned char D_001E3398[];

int func_001E1B70(int i)
{
    if (D_001E35A0[i] < D_001E3398[i]) {
        D_001E35A0[i] += 0x13;
        if (D_001E3398[i] < D_001E35A0[i])
            D_001E35A0[i] = D_001E3398[i];
        return 1;
    }
    return 0;
}
