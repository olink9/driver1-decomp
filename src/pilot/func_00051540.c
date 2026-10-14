#include "common.h"
extern char D_000BDFB4[];
extern TILE D_000BDF94;
extern unsigned short D_0008AB9C[];
extern void func_0007D66C(), func_0007D720();

#define W(o) (*(unsigned long *)(q + (o)))
#define H(o) (*(unsigned short *)(q + (o)))
#define B(o) (*(unsigned char *)(q + (o)))

void func_00051540(void)
{
    char *p = D_000BDFB4;
    char *q;

    func_0007D66C(p, 0, 0, 512, 240);
    func_0007D66C(p + 0x80, 0, 256, 512, 240);
    func_0007D720(p + 0x5C, 0, 256, 512, 240);
    func_0007D720(p + 0xDC, 0, 0, 512, 240);
    q = p - 0x10;
    B(0x27) = 1;
    B(0xA7) = 1;
    H(0x7A) = 240;
    H(0xFA) = 240;
    W(8) = 0xD8600;
    *(unsigned long *)(p - 0x10) = 0xD8600;
    W(0x88) = 0xF6600;
    W(0x80) = 0xF6600;
    W(4) = 0xD0200;
    W(0x84) = 0xD4400;
    setTile(&D_000BDF94);
    D_000BDF94.x0 = 0;
    D_000BDF94.y0 = 0;
    D_000BDF94.w = 512;
    D_000BDF94.h = 240;
    W(0xC) = 0;
    W(0x8C) = 1;
    H(0x76) = D_0008AB9C[7];
    H(0xF6) = D_0008AB9C[7];
    H(0x74) = D_0008AB9C[6];
    H(0xF4) = D_0008AB9C[6];
}
