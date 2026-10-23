typedef struct { short x, y, w, h; } RECT16;
typedef struct { int tim, clut; } CREDIMG;
extern RECT16 D_001E1F80;
extern char *D_001F3818;
extern char *D_001E27F0;
extern char D_001E27F8[];
extern int D_001F37F8;
extern int D_001F3868;
extern int D_001F3870;
extern int D_001E1AC0;
extern int D_001E1AB8;
extern int D_001E1F44;
extern void func_00074404(RECT16 *rect, void *p); /* LoadImage */
extern void func_00073B74();
extern void func_00073B94();
extern void func_000768E4();
extern void func_00073AF8();
extern void func_001E088C(void);

int func_001E0974(int n, int m)
{
    char *base;
    int tim, clut;
    char *data;
    char *pal;
    char *pix;
    int r;
    int t;

    base = D_001F3818;
    tim = ((CREDIMG *)(base + n * 8))->tim;
    clut = ((CREDIMG *)(base + n * 8))->clut;
    D_001E1F80.x = 0x3B0;
    D_001E1F80.y = m + 0x1C0;
    D_001E1F80.w = 16;
    D_001E1F80.h = 1;
    pal = base + clut;
    data = base + tim;
    pix = pal + 0x40;
    func_00074404(&D_001E1F80, pal + 0x14);
    if (m == 0) {
        D_001E1F80.x = 0x3A0;
        D_001E1F80.y = 0x180;
    } else {
        D_001E1F80.x = 0x380;
        D_001E1F80.y = 0x1C0;
    }
    D_001E1F80.w = 0x14;
    D_001E1F80.h = 0x40;
    func_00074404(&D_001E1F80, pix);
    D_001F37F8 = 0;
    t = m * 5;
    D_001E1F80.x = ((t & 3) << 4) + 0x380;
    D_001E1F80.y = ((t >> 2) << 6) + 0x100;
    D_001E1F80.w = 16;
    D_001E1F80.h = 64;
    D_001F3868 = 5;
    func_00073B94(func_001E088C);
    func_000768E4(data, D_001F3870, D_001E27F8);
    func_00073AF8(D_001F3870, 2);
    func_00073B74(D_001E27F0 + (D_001F37F8 << 11), D_001E1AC0);
    r = D_001E1F44;
    D_001E1AB8 = 1;
    return r;
}
