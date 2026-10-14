extern long long D_0009D8DC;
extern long long D_0009D8E4;
extern long long D_0009AF48;
extern short D_0009AF04[];

void func_0001BA14(void) {
    int a, b, c;
    short *q;
    if ((*(int *)&D_0009D8DC == 0 && *(short *)&D_0009AF48 < 0) || (*(int *)&D_0009D8DC != 0 && *(short *)&D_0009AF48 > 0)) {
        a = D_0009AF04[0x4C / 2];
        b = D_0009AF04[0x44 / 2];
        D_0009AF04[0x44 / 2] = a;
        D_0009AF04[0x4C / 2] = b;
        D_0009AF04[0x54 / 2] = a;
        D_0009AF04[0x5C / 2] = b;
    }
    if (*(int *)&D_0009D8E4 != 0) {
        q = D_0009AF04;
        c = -0xF5;
    } else {
        q = D_0009AF04;
        if (q[0x42 / 2] >= -0xC8) return;
        c = -0xA5;
    }
    q[0x42 / 2] = c;
    q[0x4A / 2] = c;
    q[0x52 / 2] = c;
    q[0x5A / 2] = c;
}
