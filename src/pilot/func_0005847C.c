typedef struct { int x, z; } P2;
typedef struct {
    unsigned char pad0[3]; unsigned char len; unsigned char r0, g0, b0, code;
    short x0, y0; unsigned char r1, g1, b1, p1; short x1, y1;
    unsigned char r2, g2, b2, p2; short x2, y2;
} PRIM;
typedef struct { PRIM *prim; } DB;
extern long long current;
extern int ratan2();
extern void DrawPrim();

static inline int sub2(int a, int b) { return a - b; }
void func_0005847C(P2 *a, P2 *b)
{
    int c, dz, dx, t;
    PRIM *p;
    dz = sub2(a->z, b->z);
    dx = sub2(a->x, b->x);
    ratan2(dz, dx);
    c = 0x40;
    p = (*(DB **)&current)->prim;
    p->len = 7;
    p->code = 0x24;
    p->x0 = -1; p->y0 = -1;
    p->x1 = -1; p->y1 = -1;
    p->x2 = -1; p->y2 = -1;
    *(short *)((char *)p + 0x16) = c;
    DrawPrim(p);
    p = (*(DB **)&current)->prim;
    t = dz * 3 >> 4;
    p->len = 6;
    p->code = 0x30;
    p->r0 = 0; p->g0 = 0; p->b0 = 0;
    p->r1 = 0; p->g1 = 0; p->b1 = 0;
    p->r2 = c; p->g2 = c; *(char *)((char *)p + 0x16) = c;
    p->x0 = a->x + t;
    dx >>= 3;
    p->y0 = a->z - dx;
    p->x1 = a->x - t;
    p->y1 = a->z + dx;
    p->x2 = b->x;
    p->y2 = b->z;
    p->code |= 2;
    DrawPrim(p);
}
