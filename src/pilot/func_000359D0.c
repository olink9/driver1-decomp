typedef struct { char pad[12]; unsigned char b0; unsigned char b1; } T14;
extern int D_00096F80[2];
extern unsigned char D_00096899[8];
extern int D_00096A04;
extern char D_00097028[8];
typedef struct { char p0[0x50]; int f50; char p1[0xB4]; int f108; } S9;
extern S9 D_0009B0A4;
extern unsigned char D_000972DC;
extern unsigned char D_000972E0;
extern unsigned char D_000972E8;
extern unsigned char *D_000972EC;
extern unsigned char *D_00089F0C[];
extern T14 D_000A8014[];

void func_000359D0(void) {
    int a = 0;
    int r;
    if (D_00096F80[0] == 3 || D_00096F80[0] == 1 || D_00096899[0] != 0 || D_00096A04 == 0) {
        D_000A8014[0].b1 = 0;
        D_000A8014[0].b0 = 0;
        return;
    }
    if (D_00097028[0] != 0) {
        int v;
        a = D_0009B0A4.f50 >> 15;
        if (a < 0) a = -a;
        a = a + D_0009B0A4.f108;
        v = 0x3C;
        if (a < 0x3D) v = a;
        a = v;
    }
    if (D_000972DC != 0) {
        if (D_000972EC == 0) {
            D_000972E0 = D_000972DC;
            D_000972EC = D_00089F0C[D_000972DC];
        }
    }
    if (D_000972EC != 0) {
        a += *D_000972EC++;
        if (*D_000972EC == 0) D_000972EC = 0;
    }
    if (a >= 0x100) a = 0xFF;
    r = a * (D_00096A04 + 0x100);
    D_000972DC = 0;
    D_000A8014[0].b1 = r >> 9;
    if (D_000972E8 != 0) {
        D_000972E8 = D_000972E8 - 1;
        if (D_000972E8 != 0) D_000A8014[0].b0 = 1;
        else D_000A8014[0].b0 = 0;
    }
}
