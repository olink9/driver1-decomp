/* func_00073104: SPU voice setup (sample table D_000C78E8[bank][idx]) */
typedef struct { int voice; int mask; short vl, vr; short pad1[4]; unsigned short pitch; char pad2[6]; int addr; int loop_addr; char pad3[0x20]; } VA;
typedef struct { char pad[0x14]; int base; } B;
extern VA D_000C79D0;
extern B *D_0009781C;
extern void SpuSetVoiceAttr();

extern int D_000C78E8[][34];
void func_00073104(int a, int voice, int idx, int pitch, int l, int r) {
    D_000C79D0.mask = 0x10093;
    D_000C79D0.voice = 1 << voice;
    D_000C79D0.pitch = pitch;
    D_000C79D0.vl = l;
    D_000C79D0.vr = r;
    D_000C79D0.addr = D_000C78E8[a][idx] + D_0009781C->base;
    D_000C79D0.loop_addr = D_000C78E8[a][idx] + D_0009781C->base;
    SpuSetVoiceAttr(&D_000C79D0);
}
