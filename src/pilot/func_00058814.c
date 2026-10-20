extern long long D_00096898, D_00096899, D_000BEED8;
#define B98 (*(unsigned char *)&D_00096898)
#define B99 (*(unsigned char *)&D_00096899)
extern int D_0009761C, D_00097670, D_00097614, D_00097664, D_0009766C;
extern char D_00096DA0, D_00097660;
extern int D_00096DC8, D_00097668, D_0009765C, D_00097608, D_00096DCC, D_00097618;
extern int D_00096DE0, D_00096DDC, D_00096DD8, D_00096DA8, D_0009762C, D_00097648, D_00097624;
extern short D_00097634;
extern int D_00097620, D_00096DB4;
extern char D_00097654[3];
extern int D_000BEED4[];
extern int D_000BEEE0[];
extern char D_000BEEBC[];

void func_00058814(void)
{
    int base = 0x1FABBC;
    int i;
    int m = B98;
    unsigned char c;
    int e8;

    D_0009761C = 1;
    D_00096DA0 = 1;
    D_00097670 = base;
    D_00097614 = 0;
    D_000BEED4[0] = base;
    if ((unsigned)(m - 6) < 2)
        D_00097664 = 1000;
    if ((m & 0xff) == 3 || (m & 0xff) == 8)
        D_00097664 = 2000;
    else
        D_00097664 = 3000;
    D_000BEED4[1] = base + 0x1770;
    D_0009766C = base + 0x1770;
    D_000BEED4[2] = base + 0xBB8;
    for (i = 0; i < 3; i++) {
        D_000BEEE0[i] = D_000BEED4[i] + D_00097664 * 3;
        D_00097654[i] = 0;
    }
    e8 = *(int *)&D_000BEED8;
    c = B99;
    base += 0x2328;
    D_00096DC8 = base;
    base += 0x640;
    D_00097668 = base;
    base += 0x14;
    D_0009765C = base;
    base += 0x258;
    D_00097608 = base;
    D_00096DCC = 0;
    D_00097618 = 0;
    D_00096DE0 = base - e8;
    base += 0x870;
    D_00096DDC = base - D_00097670;
    if (c)
        D_00097660 = 1;
    else
        D_00097660 = 0;
    D_00096DD8 = 0;
    D_00096DA8 = 0;
    D_0009762C = 150;
    D_00097634 = 0;
    D_00097648 = 0;
    D_00097624 = 0;
    if ((unsigned)(B98 - 7) < 2) {
        D_00097620 = 150;
    } else {
        D_00097620 = 100;
    }
    i = 23;
    {
        char *p = D_000BEEBC;
        p += 23;
        for (i = 23; i >= 0; i--)
            *p-- = 0;
    }
    D_00096DB4 = 0;
}
