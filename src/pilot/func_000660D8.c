/* unknown */
typedef struct { char pad[0xC8]; int x; int pad2; int y; } S;
extern S D_0009B0A4;
extern unsigned char D_0008BBF0[];

int func_000660D8(int a, int b, int c, int d)
{
    int t, u, y;
    S *s = &D_0009B0A4;
    t = s->x + c / 256;
    u = s->y + d / 256;
    a = a - t;
    y = b - u;
    if (a < 0) a = -a;
    if (y < 0) y = -y;
    if (a < y) { t = a; a = y; y = t; }
    if (a != 0) {
        return (a * D_0008BBF0[(y << 6) / a]) / 128;
    }
    return 0;
}
