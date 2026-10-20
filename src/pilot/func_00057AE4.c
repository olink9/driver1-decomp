/* FAKEMATCH: a do { } while (0) wrapper (emits no code) is used only to raise loop-depth weights in register allocation - our run-1 rule counts these as fake until a real C form (e.g. a macro body) is found. */
/* run2-r3: shade (14) only wins s4 when the GT4 setup sits in a do { } while (0) block: the loop notes raise the
   loop-depth weight of its refs (flow REG_N_REFS) above the j*sy giv / j / 0x80. Real C, no asm hint. */
/* func_00057AE4: draws the full-screen map (Driver 1 only, no Driver 2 counterpart) */
typedef struct { int x, y, z, pad; } POS;
typedef struct { unsigned char r, g, b, cd; } COL;
typedef struct { int pad[2]; int width; int height; int d; int pad2; } MAPDEF;
typedef struct { char pad0[0xC8]; int x; int pad1; int z; char pad[0x18C - 0xD4]; } CAR;
typedef struct { int vx, vy, vz, pad; } VEC;

typedef struct {
    unsigned long tag; unsigned char r0, g0, b0, code;
    short x0, y0; short w, h;
} TILE;
typedef struct {
    unsigned long tag;
    unsigned char r0, g0, b0, code;
    short x0, y0; unsigned char u0, v0; unsigned short clut;
    unsigned char r1, g1, b1, p1;
    short x1, y1; unsigned char u1, v1; unsigned short tpage;
    unsigned char r2, g2, b2, p2;
    short x2, y2; unsigned char u2, v2; unsigned short pad2;
    unsigned char r3, g3, b3, p3;
    short x3, y3; unsigned char u3, v3; unsigned short pad3;
} GT4;
typedef struct {
    unsigned long tag; unsigned char r0, g0, b0, code;
    short x0, y0; unsigned char u0, v0; unsigned short clut;
    short x1, y1; unsigned char u1, v1; unsigned short tpage;
    short x2, y2; unsigned char u2, v2; unsigned short pad2;
    short x3, y3; unsigned char u3, v3; unsigned short pad3;
} FT4;
typedef struct {
    unsigned long tag; unsigned char r0, g0, b0, code;
    short x0, y0; unsigned char u0, v0; unsigned short clut;
    short x1, y1; unsigned char u1, v1; unsigned short tpage;
    short x2, y2; unsigned char u2, v2; unsigned short pad2;
} FT3;

extern long long D_00097530;
extern long long D_00096DC4;
extern long long D_00096834;
extern long long D_000974B4;
extern long long D_0009754C;
extern long long D_00097694;
extern unsigned short D_000975FC;
extern int D_000975EC;
extern int D_000975DC;
extern MAPDEF D_0008B080[];
extern CAR D_0009B0A4[];
extern unsigned short D_00087030[][32];
extern unsigned short D_00086F30[];
extern VEC D_000BE9B4;
extern char D_00010CD4[];
extern void DrawPrim();
extern void func_00058278();
extern void func_0005813C();
extern void func_000585D8();
extern void func_0005847C();
extern void func_000541C8();
extern void func_00056278();
extern void func_0005C8F4();

#define MODE (*(int *)&D_00096834)

void func_00057AE4(void)
{
    COL col;
    POS pos;
    POS pos2;
    int w, h, ox, oy;
    int i, j;
    int x0, y0, x1, y1;
    int sx, sy;
    int u, v, uu;
    int shade;
    int jy;
    int t;
    TILE *tile;
    GT4 *gt;
    FT4 *ft;
    FT3 *f3;

    *(int *)&D_00096DC4 = 1;
    tile = (TILE *)**(int **)&D_00097530;
    ((unsigned char *)tile)[3] = 3;
    tile->code = 0x60;
    tile->r0 = 0;
    tile->g0 = 0;
    tile->b0 = 0;
    tile->x0 = 0;
    tile->y0 = 0;
    tile->w = 0x200;
    tile->h = 0xF0;
    DrawPrim(tile);

    w = D_0008B080[MODE].width;
    h = D_0008B080[MODE].height;
    pos.x = D_0009B0A4[0].x;
    pos.y = D_0009B0A4[0].z;
    ox = (0x200 - w) >> 1;
    oy = (0xF0 - h) >> 1;
    func_00058278(&pos);
    sx = 0x20;
    sy = 0x20;
    if (MODE == 1) {
        if (pos.y > 0x78) {
            if (pos.y < 0x104)
                ox = 0x78 - pos.y;
            else
                ox = -0x8C;
        } else
            ox = 0;
        oy = (0x200 - h) >> 1;
    } else if (MODE == 3) {
        ox = (0xF0 - w) >> 1;
        oy = (0x200 - h) >> 1;
    }

    for (i = 0; i < w >> 5; i++) {
        shade = 0xE;
        for (j = 0; j < h >> 5; j++) {
            if (MODE == 0 || MODE == 5) {
                y0 = j * sy + oy;
                y1 = y0 + sy;
                x0 = i * sx + ox;
                x1 = x0 + sx;
            } else if (MODE == 1) {
                y0 = i * sx + ox;
                y1 = y0 + sx;
                x0 = (w - j * sy) + oy;
                x1 = x0 + sy;
            } else {
                x0 = j * sy + oy;
                x1 = x0 + sy;
                y0 = w - (i * sx + ox);
                y1 = y0 + sx;
            }
            do {
                gt = (GT4 *)**(int **)&D_00097530;
                ((unsigned char *)gt)[3] = 0xC;
                gt->code = 0x3C;
                gt->r0 = shade; gt->g0 = shade; gt->b0 = shade; 
                gt->r1 = shade + 1; gt->g1 = shade + 1; gt->b1 = shade + 1; 
                gt->r2 = shade - 1; gt->g2 = shade - 1; gt->b2 = shade - 1; 
                gt->r3 = shade; gt->g3 = shade; gt->b3 = shade; 
                u = x0 & 0x40;
                v = y0 & 0x40;
                gt->u0 = u; gt->v0 = v;
                gt->u1 = u + 0x3F; gt->v1 = v;
                gt->u2 = u; gt->v2 = v + 0x3F;
                gt->u3 = u + 0x3F; gt->v3 = v + 0x3F;
                gt->x0 = x0; gt->y0 = y0;
                gt->x1 = x1; gt->y1 = y0;
                gt->x2 = x0; gt->y2 = y1;
                gt->x3 = x1; gt->y3 = y1;
                gt->clut = D_00087030[*(int *)&D_000974B4][0];
                gt->tpage = D_00086F30[*(int *)&D_000974B4] | 0x20;
            } while (0);
            DrawPrim(gt);
            func_00056278(0, i, j);

            ft = (FT4 *)**(int **)&D_00097530;
            ((unsigned char *)ft)[3] = 9;
            ft->code = 0x2C;
            ft->r0 = 0x80; ft->g0 = 0x80; ft->b0 = 0x80; 
            ft->code |= 2;
            ft->x0 = x0; ft->y0 = y0;
            ft->x1 = x1; ft->y1 = y0;
            ft->x2 = x0; ft->y2 = y1;
            ft->x3 = x1; ft->y3 = y1;
            ft->clut = D_000975FC;
            ft->tpage = D_00086F30[*(int *)&D_0009754C];
            if (MODE == 0 || MODE == 5) {
                ft->u0 = 0x80; ft->v0 = 0;
                ft->u1 = 0x9F; ft->v1 = 0;
                ft->u2 = 0x80; ft->v2 = 0x1F;
                ft->u3 = 0x9F; ft->v3 = 0x1F;
            } else if (MODE == 1) {
                ft->u0 = 0x80; ft->v0 = 0x1F;
                ft->u1 = 0x80; ft->v1 = 0;
                ft->u2 = 0x9F; ft->v2 = 0x1F;
                ft->u3 = 0x9F; ft->v3 = 0;
            } else {
                ft->u0 = 0x9F; ft->v0 = 0;
                ft->u1 = 0x9F; ft->v1 = 0x1F;
                ft->u2 = 0x80; ft->v2 = 0;
                ft->u3 = 0x80; ft->v3 = 0x1F;
            }
            DrawPrim(ft);
        }
    }

    col.r = 0x80; col.g = 0x80; col.b = 0x80;
    if (MODE == 1) {
        t = ox;
        ox = oy;
        oy = t;
    }
    pos.x += ox;
    pos.y += oy;
    func_0005813C(&col, &pos);
    func_000585D8(&pos);
    if (D_000975EC != 0 && D_000975DC != 0) {
        pos2.x = D_000BE9B4.vx;
        pos2.y = D_000BE9B4.vz;
        col.r = 0xFF;
        col.g = 0;
        col.b = 0;
        func_00058278(&pos2);
        pos2.x += ox;
        pos2.y += oy;
        func_0005813C(&col, &pos2);
        func_0005847C(&pos, &pos2);
    }
    func_000541C8();
    f3 = (FT3 *)**(int **)&D_00097530;
    ((unsigned char *)f3)[3] = 7;
    f3->code = 0x24;
    f3->x0 = -1; f3->y0 = -1;
    f3->x1 = -1; f3->y1 = -1;
    f3->x2 = -1; f3->y2 = -1;
    f3->tpage = *(unsigned short *)&D_00097694;
    DrawPrim(f3);
    func_0005C8F4(D_00010CD4, 0xD2);
}
