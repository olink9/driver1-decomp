typedef struct { int voice; int mask; char pad[0x1C]; int a24; int a28; int a2c; short s30, s32, s34, s36, s38; char pad2[0x84-0x3A]; } SVA;
extern SVA D_000C79D0;
extern void SpuSetVoiceAttr(SVA *);
void func_000731A0(int v)
{
    D_000C79D0.mask = 0xFF80;
    D_000C79D0.voice = 1 << v;
    D_000C79D0.a24 = 1;
    D_000C79D0.a28 = 1;
    D_000C79D0.a2c = 3;
    D_000C79D0.s30 = 0x10;
    D_000C79D0.s32 = 0;
    D_000C79D0.s34 = 0;
    D_000C79D0.s36 = 0;
    D_000C79D0.s38 = 15;
    SpuSetVoiceAttr(&D_000C79D0);
}
