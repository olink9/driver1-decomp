typedef struct {
    short s0;
    short s2;
    short s4;
    short s6;
    short s8;
    short sA;
    short sC;
    short sE;
    short s10;
    char pad[0x4C - 0x12];
    int v4C;
    int v50;
    int v54;
} O;

void func_00020998(O *o)
{
    int a[4];
    int b[4];

    a[0] = o->s4;
    a[1] = o->sA;
    a[2] = o->s10;
    b[0] = o->v4C >> 12;
    b[1] = o->v50 >> 12;
    b[2] = o->v54 >> 12;
    o->v4C += o->s4 * 50;
    o->v50 += o->sA * 50;
    o->v54 += o->s10 * 50;
}
