typedef struct { int pad0; int f4; char pad[0x84 - 8]; } SV;
extern int D_00096C74;
extern SV D_000BD498[];
extern void SpuSetVoiceAttr(SV *);
void func_0004E024(void)
{
    int i;
    if (D_00096C74 != 0) {
        for (i = 0; i < 16; i++) {
            D_000BD498[i].f4 = 15;
            SpuSetVoiceAttr(&D_000BD498[i]);
        }
        D_00096C74 = 0;
    }
}
