typedef struct { short x, y, w, h; } RECT;
typedef struct { unsigned short x, y; } XY;
typedef struct { int a, b, c, d; unsigned char e, f, g, h; int i; } ENT;
extern XY D_0008B140[];
extern ENT D_0008B080[];
extern long long D_00096834;
extern int D_00096D5C;
extern int D_000975D8;
extern unsigned short D_000975F0;
extern long long D_000975F2;
extern int D_000975F8;
extern char D_000BE790[];
extern int func_00026140();
extern void func_00074404();
extern void DrawSync();

void func_00056278(int a0, int a1, int a2)
{
    RECT r;
    int x;
    int t;
    r.x = D_000975F0 + D_0008B140[a0].x;
    r.y = *(unsigned short *)&D_000975F2 + D_0008B140[a0].y;
    r.w = 8;
    r.h = 32;
    t = a1 + a2 * D_000975D8;
    x = a1 << 5;
    a1 = x;
    if (t >= D_0008B080[*(int *)&D_00096834].e || t < 0 || (*(int *)&D_00096834 != 3 && x >= D_0008B080[*(int *)&D_00096834].c) || a1 < 0) {
        int i;
        for (i = 0; i < 0x200; i++)
            D_000BE790[i] = D_0008B080[*(int *)&D_00096834].f;
    } else {
        D_00096D5C = func_00026140(D_000975F8 + *(short *)(D_000975F8 + t * 2), D_000BE790);
    }
    func_00074404(&r, D_000BE790);
    DrawSync(0);
}
