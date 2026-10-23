typedef struct { short x, y, w, h; } RECT16;
extern RECT16 D_001E1F80;
extern char *D_001E27F0;
extern int D_001F37F8;
extern int D_001F3868;
extern int D_001E1AC0;
extern int D_001E1AB8;
extern void func_00074404(RECT16 *rect, void *p); /* LoadImage */
extern void func_00073B74();
extern void func_00073B94();

void func_001E088C(void)
{
    func_00074404(&D_001E1F80, D_001E27F0 + (D_001F37F8 << 11));
    D_001F37F8 = (D_001F37F8 + 1) & 7;
    D_001E1F80.x += 16;
    if (D_001E1F80.x == 0x3C0) {
        D_001E1F80.x = 0x380;
        D_001E1F80.y += 64;
    }
    if (--D_001F3868 != 0) {
        func_00073B74(D_001E27F0 + (D_001F37F8 << 11), D_001E1AC0);
    } else {
        func_00073B94(0);
        D_001E1AB8 = 0;
    }
}
