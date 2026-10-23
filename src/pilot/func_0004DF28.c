typedef struct { unsigned int voice; unsigned int mask; short a; short b; char pad[0x34]; } VA;
typedef struct { VA va; char pad[0x44]; } ENT;
extern int D_00096C74, D_00096C78, D_00096C88;
extern ENT D_000BD498[];
extern void XM_Pause(), SpuSetMute(), SpuSetVoiceAttr();
void func_0004DF28(void)
{
    VA attr;
    int i;
    if (D_00096C78 == 0) {
        XM_Pause(D_00096C88);
        D_00096C78 = 1;
        SpuSetMute(1);
    }
    if (D_00096C74 == 0) {
        D_00096C74 = 1;
        attr.voice = 0xFFFFFF;
        attr.mask = 3;
        attr.a = 0;
        attr.b = 0;
        SpuSetVoiceAttr(&attr);
        for (i = 0; i < 16; i++) {
            attr = D_000BD498[i].va;
            attr.a = 0;
            attr.b = 0;
            attr.mask = 15;
            SpuSetVoiceAttr(&attr);
        }
    }
}
