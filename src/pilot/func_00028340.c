/* no Driver 2 counterpart: emits 4-point primitives (code 5) for a list of packed vertex indices */
typedef struct { char *primptr; unsigned long *ot; } DB;
extern long long current;
#define current (*(DB **)&current)

typedef struct { short x, y; } SXY;

extern unsigned long D_0008904C[];
extern unsigned short *D_00096944;
extern SXY *D_00096938;
extern unsigned long *D_0009693C;

void func_00028340(unsigned long col, int n, unsigned long *ot)
{
    int i;
    unsigned long *prim;
    int v[4];
    unsigned long w;
    unsigned long *tab;

    prim = (unsigned long *)current->primptr;
    tab = D_0008904C;
    for (i = 0; i < n; i++) {
        w = tab[i];
        v[0] = w & 0x1f;
        v[1] = (w >> 8) & 0x1f;
        v[2] = (w >> 16) & 0x1f;
        v[3] = (w >> 24) & 0x1f;

        if (D_00096944[v[0]] == 0 && D_00096944[v[1]] == 0 &&
            D_00096944[v[2]] == 0 && D_00096944[v[3]] == 0)
            continue;
        if (D_00096938[v[0]].x < 0 && D_00096938[v[1]].x < 0 &&
            D_00096938[v[2]].x < 0 && D_00096938[v[3]].x < 0)
            continue;
        if (D_00096938[v[0]].x > 512 && D_00096938[v[1]].x > 512 &&
            D_00096938[v[2]].x > 512 && D_00096938[v[3]].x > 512)
            continue;
        if (D_00096938[v[0]].y > 240 && D_00096938[v[1]].y > 240 &&
            D_00096938[v[2]].y > 240 && D_00096938[v[3]].y > 240)
            continue;

        if (D_00096938[v[1]].x < -250)
            D_00096938[v[1]].x = -250;
        else if (D_00096938[v[1]].x > 762)
            D_00096938[v[1]].x = 762;
        if (D_00096938[v[2]].x < -250)
            D_00096938[v[2]].x = -250;
        else if (D_00096938[v[2]].x > 762)
            D_00096938[v[2]].x = 762;
        if (D_00096938[v[3]].x < -250)
            D_00096938[v[3]].x = -250;
        else if (D_00096938[v[3]].x > 762)
            D_00096938[v[3]].x = 762;

        if (D_00096938[v[0]].y > 506)
            D_00096938[v[0]].y = 506;
        if (D_00096938[v[1]].y > 506)
            D_00096938[v[1]].y = 506;
        if (D_00096938[v[2]].y > 506)
            D_00096938[v[2]].y = 506;
        if (D_00096938[v[3]].y > 506)
            D_00096938[v[3]].y = 506;

        prim[0] = *ot | 0x5000000;
        *ot = (unsigned long)prim;
        prim[1] = col;
        prim[2] = D_0009693C[v[0]];
        prim[3] = D_0009693C[v[1]];
        prim[4] = D_0009693C[v[2]];
        prim[5] = D_0009693C[v[3]];
        prim += 6;
    }
    current->primptr = (char *)prim;
}
