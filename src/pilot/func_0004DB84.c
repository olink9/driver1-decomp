/* func_0004E5E4 takes the channel index */
typedef struct { int pad0; int f4; char pad[0x3a]; unsigned short f42; int pad3; int f48; char pad2[0x38]; } S;
extern int D_00096C74;
extern S D_000BD498[];
extern void func_0004E5E4();
extern void SpuSetVoiceAttr();


void func_0004DB84(unsigned int a0, int a1) {
    S *p;
    if (a0 < 0x10 && D_00096C74 == 0) {
        S *b = D_000BD498;
        p = b + a0;
        if (p->f42 != 0) {
            p->f48 = a1;
            func_0004E5E4(a0);
            p->f4 = 0xF; SpuSetVoiceAttr(p);
        }
    }
}
