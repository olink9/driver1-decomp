extern char D_000ED530;
extern char D_000ED531;
extern short D_000ED532;
extern short D_000ED534;
extern short D_000ED536;
extern char D_000ED538[];

void func_000E6F14(short a, short b, char c, char d, unsigned char *rgb)
{
    D_000ED536 = a;
    D_000ED532 = a;
    D_000ED534 = b;
    D_000ED531 = c;
    D_000ED530 = d;
    if (rgb) {
        D_000ED538[0] = rgb[0];
        D_000ED538[1] = rgb[1];
        D_000ED538[2] = rgb[2];
        D_000ED538[3] = rgb[3];
    } else {
        D_000ED538[0] = 255;
        D_000ED538[1] = 255;
        D_000ED538[2] = 255;
        D_000ED538[3] = 255;
    }
}
