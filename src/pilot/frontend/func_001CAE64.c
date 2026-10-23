/* func_001CAE64: index every access by i (D[i].f) - the giv form; flag 8 stores 2 (the old wip stored 4 for both). */
typedef struct { unsigned short f; unsigned short g; int pad; } S;
typedef struct { int a, b, c, d; } E4;
extern S *D_001CEF58;
extern int D_001CF0F0[], D_001CF288;
extern E4 D_001D32B0[];

void func_001CAE64(void)
{
    int i;
    D_001CF288 = 0;
    for (i = 0; i < 4; i++) {
        if (D_001CEF58[i].f & 4) {
            D_001D32B0[i].a = 4;
            D_001D32B0[i].b = 0;
            D_001D32B0[i].c = 0;
            D_001D32B0[i].d = D_001CEF58[i].g;
            D_001CF288++;
        } else if (D_001CEF58[i].f & 8) {
            D_001D32B0[i].a = 2;
            D_001D32B0[i].b = 0;
            D_001D32B0[i].c = 0;
            D_001D32B0[i].d = D_001CEF58[i].g;
            D_001CF288++;
        } else if (D_001CEF58[i].f & 2) {
            D_001D32B0[i].a = 8;
            D_001D32B0[i].b = 1;
            D_001D32B0[i].c = D_001CF0F0[D_001CEF58[i].g] / 8;
            D_001D32B0[i].d = D_001CF0F0[D_001CEF58[i].g] % 8;
            D_001CF288++;
        }
    }
}
