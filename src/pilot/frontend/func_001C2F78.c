/* FAKEMATCH: unused RECT r2 only sizes the frame (counts at sp+0x20). Only fnscore diff: target reloc %hi/%lo(D_000D0200) vs literal 0xD0200 (byte-identical; buf is the 0xD0200 load address, as in mcard func_000EC280). */
typedef struct { short x, y, w, h; } RECT;
typedef struct { char name[16]; short s10; short pad; int x, y, w, h; } ENT;
typedef struct { int used; unsigned char slot; char pad; short tpage; int count; ENT e[10]; } TEX;
typedef struct { int n[3]; } N3;

#define getTPage(tp, abr, x, y) \
    ((((tp) & 0x3) << 7) | (((abr) & 0x3) << 5) | (((y) & 0x100) >> 4) | (((x) & 0x3ff) >> 6) | (((y) & 0x200) << 2))

extern N3 D_001C0188;
extern TEX D_001CFDF0[];
extern RECT D_000976A4;
extern int D_000967C0, D_000D0200;
extern char D_001C0194[], D_001C01A4[], D_001C01B4[], D_001C01C4[], D_001C01D4[], D_001C01E0[];
extern char D_001C01F0[], D_001C0200[], D_001C0208[], D_001C0210[], D_001C0220[], D_001C0228[];
extern void func_00073F88(), func_00079800(), SetDispMask(), ShowLoadingScreen(), func_000505E0();
extern void Loadfile(), LoadFont(), func_00074404(), DrawSync(), func_0007D608(), func_001C1270();
extern int sprintf();
extern void func_000E6D60(char *file, char *name, unsigned char idx, short val);

void func_001C2F78(void)
{
    RECT r;
    RECT r2; /* FAKEMATCH: frame size */
    N3 counts;
    char name[16];
    unsigned char *p;
    short *e;
    int *buf;
    int i, j, k, n, x, y;

    counts = D_001C0188;
    func_00073F88(1);
    func_00079800(0);
    SetDispMask(0);
    if (D_000967C0 != 0) {
        ShowLoadingScreen(D_001C0194, 1, 7);
        func_000505E0();
        D_000967C0 = 0;
    } else {
        ShowLoadingScreen(D_001C01A4, 1, 7);
        func_000505E0();
    }
    Loadfile(D_001C01B4, D_001CFDF0);
    for (j = 31; j >= 0; j--)
        D_001CFDF0[j].used = 0;
    func_000505E0();
    r.x = 0x3C0;
    r.y = 0x100;
    r.w = 16;
    r.h = 1;
    D_000976A4.x = 0x3D0;
    D_000976A4.y = 0x103;
    D_000976A4.w = 16;
    D_000976A4.h = 1;
    LoadFont(&r);
    k = 0;
    r.w = 64;
    r.h = 256;
    for (j = 0; j < 3; j++) {
        sprintf(name, D_001C01C4, j + 1);
        Loadfile(name, 0xD8600);
        p = (unsigned char *)0xD8600;
        p += *p;
        for (i = 0; i < counts.n[j]; i++) {
            r.x = (k % 11) * 64 + 320;
            r.y = (k / 11) << 8;
            D_001CFDF0[k].tpage = getTPage(0, 0, r.x, r.y);
            D_001CFDF0[k].slot = k;
            D_001CFDF0[k].used = 1;
            func_00074404(&r, p);
            DrawSync(0);
            p += 0x8000;
            k++;
        }
        func_000505E0();
        k--;
    }
    Loadfile(D_001C01D4, 0xE6600);
    func_000505E0();
    buf = (int *)0xD0200;
    Loadfile(D_001C01E0, buf);
    e = (short *)buf;
    n = *(int *)e;
    e += 2;
    x = 0x3C0;
    y = 0x100;
    for (k = 0; k < n; k++) {
        if (*e == 0x80)
            *e = -1;
        func_0007D608(e, x, y);
        DrawSync(0);
        e += 16;
        y++;
    }
    func_000E6D60(D_001C01F0, D_001C0200, 0, -2);
    func_000505E0();
    func_000E6D60(D_001C01F0, D_001C0208, 1, -2);
    func_000505E0();
    func_000E6D60(D_001C0210, D_001C0220, 2, -2);
    func_000505E0();
    func_000E6D60(D_001C0210, D_001C0228, 3, -2);
    func_001C1270();
}
