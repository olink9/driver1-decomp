extern int D_00097248;
extern int D_000972D4;
extern unsigned short D_00089118[];
extern long long D_00097524;
extern long long D_00097530;
extern char D_000A7B3C[];
extern void func_00033E54();
extern int func_00032F98();
extern void func_000359BC();
extern void func_0004DE58();
extern void func_0004E024();
extern void func_00074404();
extern void DrawSync();
extern void PutDrawEnv();
extern void DrawPrim();
extern void VSync();

void func_000334B4(void)
{
    char pad[8];
    short rect[4];
    short *rp;
    unsigned short *q;
    int i;
    unsigned short t;

    if (D_00097248) func_00033E54();
    if (D_000972D4 && func_00032F98()) {
        func_000359BC();
        func_0004DE58();
        DrawSync(0);
        PutDrawEnv(*(int *)&D_00097524 + 0x10);
        while (D_000972D4) {
            rp = rect;
            t = D_00089118[1];
            i = 1;
            q = D_00089118 + 1;
            for (; i < 13; i++, q++)
                *q = q[1];
            D_00089118[i] = t;
            rp[0] = 0x3c0;
            rp[1] = 0x1aa;
            rp[2] = 0x10;
            rp[3] = 1;
            func_00074404(rect, D_00089118);
            DrawPrim(D_000A7B3C);
            DrawSync(0);
            VSync(0);
        }
        PutDrawEnv(*(int *)&D_00097530 + 0x10);
        func_0004E024();
        PutDrawEnv(*(int *)&D_00097530 + 0x10);
        func_0004E024();
    }
}
