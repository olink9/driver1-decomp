/* func_001CB190: LoadFont(&r) takes one arg (sibling func_001C2F78); stores in func_001C2F78 order. FAKEMATCH pad. */
typedef struct { int a, b, c, d; } E;
extern E D_001CF290[];
extern int D_0008531C[];
extern int D_00096834, D_00096804, gNight, D_001CF098;
typedef struct { short x, y, w, h; } RECT16;
extern RECT16 D_000976A4;
extern void func_001CB290();
extern void func_001C9848();
extern void func_001C98F4();
extern void LoadFont(); /* main LoadFont is void(void); the a1=16 is reload's reg for the 16 stores */
extern void SetTextColour();
extern void func_0005C68C();
extern void func_0006F57C();
extern void func_001CAE64();
void func_001CB190(void)
{
    RECT16 r;
    char pad[16]; /* FAKEMATCH: frame size only (0x38) */
    D_0008531C[16] = D_001CF290[D_00096834].a;
    D_00096804 = 1;
    D_0008531C[17] = D_001CF290[D_00096834].b;
    gNight = 0;
    D_0008531C[18] = D_001CF290[D_00096834].c;
    func_001CB290();
    func_001C9848(D_00096834);
    func_001C98F4();
    r.x = 0x3C0;
    r.y = 0x100;
    r.w = 16;
    r.h = 1;
    D_000976A4.x = 0x3D0;
    D_000976A4.y = 0x103;
    D_000976A4.w = 16;
    D_000976A4.h = 1;
    LoadFont(&r);
    SetTextColour(0, 0, 0);
    func_0005C68C(0, 0);
    func_0006F57C();
    D_001CF098 = 0;
    func_001CAE64();
}
