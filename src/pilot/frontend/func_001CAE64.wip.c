/* WIP 83.88: logic identical; loop-invariant hoisting / IV strength reduction differs (target keeps e,s,i as plain bivs, hoists D_001CF288 base and D_001CF0F0 address) */
typedef struct { unsigned short f; unsigned short g; int pad; } S;
extern S *D_001CEF58;
extern int D_001CF0F0[], D_001CF288;
extern int D_001D32B0[];

void func_001CAE64(void)
{
    int i;
    S *s = D_001CEF58;
    int *e = D_001D32B0;
    D_001CF288 = 0;
    for (i = 0; i < 4; i++) {
        if (s->f & 4) goto a;
        if (s->f & 8) {
        a:
            e[0] = 4;
            e[1] = 0;
            e[2] = 0;
            e[3] = s->g;
            D_001CF288++;
        } else if (s->f & 2) {
            e[0] = 8;
            e[1] = 1;
            e[2] = D_001CF0F0[s->g] / 8;
            e[3] = D_001CF0F0[s->g] % 8;
            D_001CF288++;
        }
        e += 4;
        s++;
    }
}
