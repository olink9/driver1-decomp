extern int D_0009D8DC[];
extern long long D_0009D8E4;
extern short D_0009AF48[];
extern short car_cosmetics[];

void func_0001BA14(void) {
    int a, b, c;
    if ((D_0009D8DC[0] == 0 && D_0009AF48[0] < 0) || (D_0009D8DC[0] != 0 && D_0009AF48[0] > 0)) {
        a = car_cosmetics[0x4C / 2];
        b = car_cosmetics[0x44 / 2];
        car_cosmetics[0x44 / 2] = a;
        car_cosmetics[0x4C / 2] = b;
        car_cosmetics[0x54 / 2] = a;
        car_cosmetics[0x5C / 2] = b;
    }
    if (*(int *)&D_0009D8E4 != 0) {
        car_cosmetics[0x42 / 2] = -0xF5;
        car_cosmetics[0x4A / 2] = -0xF5;
        car_cosmetics[0x52 / 2] = -0xF5;
        car_cosmetics[0x5A / 2] = -0xF5;
    } else if (car_cosmetics[0x42 / 2] < -0xC8) {
        car_cosmetics[0x42 / 2] = -0xA5;
        car_cosmetics[0x4A / 2] = -0xA5;
        car_cosmetics[0x52 / 2] = -0xA5;
        car_cosmetics[0x5A / 2] = -0xA5;
    }
}
