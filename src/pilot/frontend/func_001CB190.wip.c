/* WIP 86.2: logic identical; register/schedule order of the prologue (lui D_0008531C first, table index in v0) and 0x10 hoist differ. char pad[16] sizes the frame */
typedef struct { int a, b, c, d; } E;
extern E D_001CF290[];
extern int D_0008531C[];
extern int D_00096834, D_00096804, gNight, D_001CF098;
extern short D_000976A4[];
extern void func_001CB290();
extern void func_001C9848();
extern void func_001C98F4();
extern void LoadFont();
extern void SetTextColour();
extern void func_0005C68C();
extern void func_0006F57C();
extern void func_001CAE64();

void func_001CB190(void)
{
    short r[4];
    char pad[16];
    int *d = D_0008531C;
    E *e = (E *)((D_00096834 << 4) + (int)D_001CF290);
    d[16] = e->a;
    D_00096804 = 1;
    d[17] = e->b;
    gNight = 0;
    d[18] = e->c;
    func_001CB290();
    func_001C9848(D_00096834);
    func_001C98F4();
    r[0] = 0x3C0;
    r[1] = 0x100;
    D_000976A4[0] = 0x3D0;
    r[2] = 16;
    r[3] = 1;
    D_000976A4[1] = 0x103;
    D_000976A4[2] = 16;
    D_000976A4[3] = 1;
    LoadFont(r, 16);
    SetTextColour(0, 0, 0);
    func_0005C68C(0, 0);
    func_0006F57C();
    D_001CF098 = 0;
    func_001CAE64();
}
