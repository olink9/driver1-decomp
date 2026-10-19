typedef struct {
    char p0[4];
    short s4;
    char p6[2];
    unsigned char b8, b9, bA, bB;
    char pc[4];
} D14;

extern D14 D_000A8014;
extern char D_000A801F[];
extern int D_00096A08;
extern long long D_000967EC;
extern char D_00096A0C[];
extern char D_00096A10[4];
extern int func_0007AED4();
extern int func_0007AF94();
extern void func_0007B198();
extern int func_0007B160();

void func_00035818(int arg, unsigned char *p)
{
    unsigned short type;
    int r;
    D14 *d;

    type = p[1] >> 4;
    D_000A8014.s4 = ~((p[2] << 8) | p[3]);
    if (type == 4) {
        D_000A8014.b9 = 1;
    } else {
        D_000A8014.b9 = 2;
        if (type == 7)
            D_000A8014.b8 = p[6];
        else
            type = 0x10;
    }
    r = func_0007AED4(arg);
    if (r == 0) {
        D_00096A08 = 0;
        return;
    }
    if (type == 0x10) {
        D_00096A08 = -1;
        return;
    }
    type = 1;
    D_00096A08 = type;
    if (func_0007AF94(arg, 2, 0))
        D_000A8014.b9 |= 4;
    if (r == type || r == 6)
        D_000A801F[0] = 0;
    d = &D_000A8014;
    if (d->bB == 0 && (d->b9 & 4)) {
        char *q = (char *)d;
        if (*(int *)&D_000967EC == 1)
            func_0007B198(arg, q + 0xC, 2);
        else
            func_0007B198(arg, D_00096A0C, 2);
        if (r == 2 || (r == 6 && func_0007B160(arg, D_00096A10)))
            D_000A801F[0] = 1;
    }
}
