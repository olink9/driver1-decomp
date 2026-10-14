typedef struct { short x, y, w, h; } RECT;
extern long long D_00097524, D_00097530;
extern unsigned short D_00089118[];
extern char D_000A7B3C[];
extern int func_0007A12C(), func_000359BC(), func_00079F70();
extern void func_0004DE58(), func_0004E024(), func_00074404(), DrawSync(), PutDrawEnv(), DrawPrim(), VSync();
void func_00034808(void)
{
    RECT rect;
    int s0;
    unsigned short first, *p, *a;
    int i;
    s0 = func_0007A12C(0);
    func_000359BC();
    func_0004DE58();
    DrawSync(0);
    PutDrawEnv(*(int *)&D_00097524 + 0x10);
    while (func_00079F70(1) != 2) {
        a = D_00089118;
        first = a[1];
        i = 1;
        p = a + 1;
        for (; i < 13; i++, p++)
            *p = p[1];
        a[i] = first;
        rect.x = 0x3C0;
        rect.y = 0x1AA;
        rect.w = 16;
        rect.h = 1;
        func_00074404(&rect, a);
        DrawPrim(D_000A7B3C);
        DrawSync(0);
        VSync(0);
    }
    PutDrawEnv(*(int *)&D_00097530 + 0x10);
    func_0004E024();
    func_0007A12C(s0);
}
