typedef struct { int vx, vy, vz, pad; } VEC;
typedef struct {
    unsigned long tag; unsigned char r0, g0, b0, code;
    short x0, y0; unsigned char r1, g1, b1, p1;
    short x1, y1;
} LG2;
extern short D_0008B268[];
extern short D_0008EAA8[];
extern long long D_00096834;
extern long long D_00097530;
extern void DrawPrim();
extern void func_00053C88();

void func_000585D8(int *pos)
{
    VEC v;
    VEC pts[4];
    VEC *pp;
    LG2 *p;
    unsigned char i;
    int x, z;
    short ang;
    x = pos[0];
    ang = D_0008B268[*(int *)&D_00096834];
    pts[1].vx = x;
    z = pos[1];
    pts[1].vy = z;
    pts[0].vx = x + ((D_0008EAA8[(ang & 0xfff) * 2] * 25) >> 12);
    pts[0].vy = z + ((D_0008EAA8[(ang & 0xfff) * 2 + 1] * 25) >> 12);
    v.vx = x + ((D_0008EAA8[(ang & 0xfff) * 2] * 15) >> 11);
    v.vz = z + ((D_0008EAA8[(ang & 0xfff) * 2 + 1] * 15) >> 11);
    pts[2].vx = x + ((D_0008EAA8[((ang - 200) & 0xfff) * 2] * 5) >> 10);
    pts[2].vy = z + ((D_0008EAA8[((ang - 200) & 0xfff) * 2 + 1] * 5) >> 10);
    pts[3].vx = x + ((D_0008EAA8[((ang + 200) & 0xfff) * 2] * 5) >> 10);
    pts[3].vy = z + ((D_0008EAA8[((ang + 200) & 0xfff) * 2 + 1] * 5) >> 10);
    pp = &pts[1];
    for (i = 0; i < 3; i++, pp++) {
        p = **(LG2 ***)&D_00097530;
        ((unsigned char *)p)[3] = 4;
        p->code = 0x50;
        p->x0 = pts[0].vx;
        p->y0 = pts[0].vy;
        p->x1 = pp->vx;
        p->y1 = pp->vy;
        p->r0 = 255; p->g0 = 255; p->b0 = 255;
        p->r1 = 64; p->g1 = 64; p->b1 = 64;
        p->code |= 2;
        DrawPrim(p);
        **(int **)&D_00097530 += 20;
    }
    func_00053C88(&v, 1);
}
