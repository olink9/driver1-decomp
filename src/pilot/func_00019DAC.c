typedef struct {
    char pad0[0x10];
    short s10, s12, s14, s16;
    unsigned char b18;
    char pad;
    short s1A, s1C;
} O;
extern int func_00019E88();
extern int func_0004A9F4();

int func_00019DAC(O *o, int x, int y)
{
    int pt[4];

    if (o->s1A == -1)
        return 0;
    if (!func_00019E88(o, x, y, o->s1A))
        return 0;
    if (o->s1C == -1)
        return 1;
    if (o->b18) {
        pt[0] = x + o->s14;
        pt[2] = y + o->s16;
        if (!func_0004A9F4(pt))
            return 0;
    }
    return func_00019E88(o, x + o->s10, y + o->s12, o->s1C) != 0;
}
