typedef struct {
    int voice;
    int mask;
    short s18;
    short s1A;
    char pad[0x40 - 0xC];
} V;
typedef struct {
    V v;
    char pad[132 - 0x40];
} CH;
extern int D_00096C74;
extern CH D_000BD498[];
extern void SpuSetVoiceAttr();

void func_0004DE58(void)
{
    V a;
    int i;

    if (D_00096C74 == 0) {
        D_00096C74 = 1;
        a.voice = 0xFFFFFF;
        a.mask = 3;
        a.s18 = 0;
        a.s1A = 0;
        SpuSetVoiceAttr(&a);
        for (i = 0; i < 16; i++) {
            a = D_000BD498[i].v;
            a.s18 = 0;
            a.s1A = 0;
            a.mask = 15;
            SpuSetVoiceAttr(&a);
        }
    }
}
