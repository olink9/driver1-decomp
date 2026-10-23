struct V { int pad0; int f4; char p1[0x48 - 8]; int f48; char p2[0x5C - 0x4C]; int f5C; char p3[0x70 - 0x60]; int f70; char p4[0x84 - 0x74]; };
extern int D_00096C74;
extern struct V D_000BD498[];
extern struct V D_000BD50C[];
extern void func_0004E5E4();
extern void SpuSetVoiceAttr();
extern void func_0004DBF8();
extern void SetChannelPitch();

void func_0004DDA8(unsigned idx, int a1, int a2, int a3, int e) {
    struct V *v;
    if (idx < 0x10 && D_00096C74 == 0) {
        if (a2 == 0) a2 = (int)&D_000BD50C[idx];
        D_000BD498[idx].f5C = a1;
        D_000BD498[idx].f70 = a2;
        D_000BD498[idx].f48 = a3;
        func_0004E5E4(idx);
        D_000BD498[idx].f4 = 0xF;
        SpuSetVoiceAttr(&D_000BD498[idx]);
        func_0004DBF8(&D_000BD498[idx]);
        SetChannelPitch(idx, e);
    }
}
