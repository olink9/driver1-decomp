struct X { char pad[0xF4]; int out; int ang; char pad2[0x18C - 0xFC]; };
extern struct X D_0009B230[];

int func_00018100(unsigned char *p, int target) {
    int cur, diff, a3;
    a3 = 0;
    cur = D_0009B230[*p].ang & 0xFFF;
    diff = ((target - cur + 0x800) & 0xFFF) - 0x800;
    if (cur != target) {
        if (abs(diff) < 0x80) a3 = diff / 2;
        else if (diff >= 0) a3 = 0x200;
        else a3 = -0x200;
    }
    D_0009B230[*p].out = a3;
    return abs(diff);
}
