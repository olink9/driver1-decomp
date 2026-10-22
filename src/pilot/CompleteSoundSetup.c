typedef struct { int voice; int mask; short vl; short vr; int pc, p10; short pitch; short p16; int p18; int addr; char pad[0x20]; char c40; char c41; short s42; int i44; char pad2[0x3C]; } VA;
typedef struct { int a; int b; char c[4]; int d; } ENTRY;
typedef struct { ENTRY e[32]; } ROW;
extern char D_000BCA74[];
extern VA D_000BD498[];
extern int D_00096C74, D_00096C80;
extern void func_0004E5E4(), SpuSetVoiceAttr(), SpuSetKey();

int CompleteSoundSetup(int v, int a, int b, int c)
{
    int p, t;
    char *base = D_000BCA74;
    ENTRY *e = (ENTRY *)(base + ((b << 4) + (a << 9)));
    unsigned int n;
    VA *va;

    p = (e->d * c) / 44100;
    if (p >= 0x4000) p = 0x3FFF;
    t = ((e->d * c) >> 12) / 60;
    if (t == 0) return -1;
    n = (unsigned int)e->b / t;
    func_0004E5E4(v);
    { VA *vb = D_000BD498; va = vb + v; }
    va->mask = 0x9F;
    D_00096C80 = 1;
    va->addr = e->a;
    va->pitch = p;
    va->c40 = e->c[0];
    va->s42 = n << 1;
    va->i44 = e->d;
    if (D_00096C74 != 0) {
        va->vl = 0;
        va->vr = 0;
    }
    SpuSetVoiceAttr(va);
    SpuSetKey(1, va->voice);
    D_00096C80 = 0;
    return v;
}
