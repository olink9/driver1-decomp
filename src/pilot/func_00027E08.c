/* no Driver 2 counterpart: draws textured quads (POLY_FT4) from packed vertex indices, clipping via func_00028FA4 */
typedef struct { char *primptr; unsigned long *ot; } DB;
typedef struct { DB *p; int pad; } CUR;
extern CUR current;
#define current (current.p)

typedef struct { short x, y; } SXY;
typedef struct { unsigned char u, v; } UV;
typedef struct { unsigned long tag, rgbc; unsigned long xy0, uv0, xy1, uv1, xy2, uv2, xy3, uv3; } FT4;

extern unsigned long D_0008904C[];
extern unsigned long D_0008905C[];
extern unsigned short texture_pages[];
extern unsigned short texture_cluts[][32];
extern unsigned short *D_00096944;
extern SXY *D_00096938;
extern SXY *D_00096948;
extern UV *D_00096940;
extern UV *D_0009694C;
extern unsigned long *D_00096950;
extern unsigned long D_00097130;
extern unsigned long D_00097134;
extern void func_00028FA4();

void func_00027E08(int tex, int unused, int n, unsigned long *ot)
{
    int i;
    unsigned long *tab;
    int v[4];
    char pad[32];
    unsigned long w;
    unsigned long *prim, *p;
    DB *db;

    if (n == 16)
        tab = D_0008905C;
    else
        tab = D_0008904C;

    D_00097134 = texture_pages[(tex >> 8) & 0x7f] << 16;
    D_00097130 = texture_cluts[(tex >> 8) & 0x7f][(tex >> 16) & 0x7f] << 16;

    for (i = 0; i < n; i++) {
        w = tab[i];
        v[0] = w & 0x1f;
        v[1] = (w >> 8) & 0x1f;
        v[2] = (w >> 16) & 0x1f;
        v[3] = (w >> 24) & 0x1f;

        if (D_00096944[v[0]] == 0 && D_00096944[v[1]] == 0 &&
            D_00096944[v[2]] == 0 && D_00096944[v[3]] == 0)
            continue;

        D_00096948[0] = D_00096938[v[0]];
        D_00096948[1] = D_00096938[v[1]];
        D_00096948[3] = D_00096938[v[2]];
        D_00096948[2] = D_00096938[v[3]];

        if (0x80008000 & ((unsigned long *)D_00096948)[0] & ((unsigned long *)D_00096948)[1] &
            ((unsigned long *)D_00096948)[2] & ((unsigned long *)D_00096948)[3])
            continue;
        if (D_00096948[0].x >= 512 && D_00096948[1].x >= 512 &&
            D_00096948[2].x >= 512 && D_00096948[3].x >= 512)
            continue;
        if (D_00096948[0].y >= 256 && D_00096948[1].y >= 256 &&
            D_00096948[2].y >= 256 && D_00096948[3].y >= 256)
            continue;

        D_0009694C[0] = D_00096940[v[0]];
        D_0009694C[1] = D_00096940[v[1]];
        D_0009694C[3] = D_00096940[v[2]];
        D_0009694C[2] = D_00096940[v[3]];

        if (D_00096948[0].x < -128 || D_00096948[1].x < -128 ||
            D_00096948[2].x < -128 || D_00096948[3].x < -128 ||
            D_00096948[0].x > 640 || D_00096948[1].x > 640 ||
            D_00096948[2].x > 640 || D_00096948[3].x > 640) {
            func_00028FA4(ot, 1);
            continue;
        }

        D_0009694C[0] = D_00096940[v[0]];
        D_0009694C[1] = D_00096940[v[1]];
        D_0009694C[3] = D_00096940[v[2]];
        db = current;
        D_0009694C[2] = D_00096940[v[3]];

        p = prim = (unsigned long *)db->primptr;
        *p++ = *ot | 0x9000000;
        *ot = (unsigned long)prim;
        *p++ = *D_00096950;
        *p++ = *(unsigned long *)&D_00096948[0];
        *p++ = *(unsigned short *)&D_0009694C[0] | D_00097130;
        *p++ = *(unsigned long *)&D_00096948[1];
        *p++ = *(unsigned short *)&D_0009694C[1] | D_00097134;
        *p++ = *(unsigned long *)&D_00096948[3];
        *p++ = *(unsigned short *)&D_0009694C[3];
        *p++ = *(unsigned long *)&D_00096948[2];
        *p++ = *(unsigned short *)&D_0009694C[2];
        current->primptr = (char *)p;
    }
}
