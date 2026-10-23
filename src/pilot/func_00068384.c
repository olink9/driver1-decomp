/* func_00068384: reset of an object, preserving some fields (Driver 2 name unknown) */
/* NOTE: the 0xB0 block copy loops step the source by two elements (src += 2) - that is what the original does. */
typedef struct { int w[11]; } S44;
typedef struct { int w[4]; } S16;
typedef struct { int w[8]; } S32;

typedef struct {
    char pad0[0x58];
    S44 b;          /* 0x58 */
    S44 a;          /* 0x84 */
    S32 d[1];       /* 0xB0 */
    char padD0[0xDC - 0xD0];
    int dc;         /* 0xDC */
    char padE0[0xE8 - 0xE0];
    S16 c;          /* 0xE8 */
    char padF8[0x11C - 0xF8];
    int flags;      /* 0x11C */
    char pad120[0x2E4 - 0x120];
    char f2e4;      /* 0x2E4 */
} OBJ;

typedef struct {
    int a, b, c, d, e;
    short f;
    int g, h, i, j, k;
} ENT;

extern long long D_0009683C;
extern long long D_000970EC;
extern int D_000BE78C[];
extern char D_000C6EA8[];
extern ENT D_000C7034[];

extern void memset();
extern void func_00068B88();
extern void func_0006B97C();

void func_00068384(OBJ *obj)
{
    S44 a;
    S44 b;
    S16 c;
    S32 d[1];
    ENT *p;
    S32 *src;
    S32 *dst;

    a = obj->a;
    b = obj->b;
    c = obj->c;
    for (src = obj->d, dst = d; src < obj->d + 1; src += 2, dst++)
        *dst = *src;
    memset(obj, 0, 0x2E8);
    for (src = d, dst = obj->d; src < d + 1; src += 2, dst++)
        *dst = *src;
    obj->b = b;
    obj->a = a;
    obj->c = c;
    *(int *)&D_0009683C = 0;
    *(int *)&D_000970EC = 0;
    for (p = D_000C7034; p < D_000C7034 + 6; p++) {
        p->a = 0x2EE;
        p->b = 0x5DC0;
        p->c = 0x1C2;
        p->d = 0;
        p->e = 0;
        p->f = 10;
        p->g = 0;
        p->h = 1000;
        p->i = 0;
        p->j = 0;
        p->k = 0;
    }
    func_00068B88();
    func_0006B97C(D_000C6EA8);
    D_000BE78C[0] = 0;
    obj->dc = 0x7FFFFFFF;
    obj->c.w[1] = 0x7FFFFFFF;
    obj->f2e4 = 1;
    obj->flags |= 4;
}
