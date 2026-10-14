extern long long D_00097050, D_00097054;
extern void func_0004343C();
void func_00053764(unsigned short a, int b, int c)
{
    int t;
    if ((unsigned short)(a + 9) < 0x13 || (unsigned short)(a - 0x7F7) < 0x13)
        t = *(unsigned char *)&D_00097054;
    else
        t = *(unsigned char *)&D_00097050;
    switch (t) {
    case 1: case 2:
        func_0004343C(b, c - 0x2D0, -0x5B4, -0x46, 0x200);
        break;
    case 4:
        func_0004343C(b, c - 0x2D0, -0x546, -0x46, 0x400);
        break;
    case 3:
        func_0004343C(b, c - 0x2D0, -0x4D8, -0x46, 0x800);
        break;
    }
}
