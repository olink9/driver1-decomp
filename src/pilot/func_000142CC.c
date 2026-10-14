typedef struct { char p0[9]; unsigned char b9; char p1[0xD]; unsigned char f17; int i18; char p2[0x20]; int i3C; } O;
extern int D_000852FC[], D_0008530C[];
extern int func_00018F44();
int func_000142CC(O *o)
{
    if (o->f17 & 8) {
        o->i3C -= 16;
        if (o->i3C < 0) o->i3C = 0;
    } else {
        if (o->i18 < 0x28A || func_00018F44(o, 1, 0) != -1) {
            o->i3C = 0;
            return 0;
        }
        if (o->i18 >= D_0008530C[o->b9 & 3])
            goto tab;
        o->i3C = o->i18 / 24;
        if (o->i3C < 5) o->i3C = 5;
    }
    return o->i3C;
tab:
    return D_000852FC[o->b9 & 3];
}
