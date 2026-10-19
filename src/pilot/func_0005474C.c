/* func_0005474C - Driver 2: DrawOverheadMap (OVERMAP.C) */
#include "gte.h"

typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct { short m[3][3]; int t[3]; } MATRIX;
typedef struct { short x, y, w, h; } RECT;
typedef struct { char *primptr; unsigned int *ot; } DB;
typedef struct { short u, w, v, h; } MAPTEX;
typedef struct { short x, y; } MAPPOS;

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short x1, y1;
    unsigned char u1, v1;
    unsigned short tpage;
    short x2, y2;
    unsigned char u2, v2;
    unsigned short pad1;
    short x3, y3;
    unsigned char u3, v3;
    unsigned short pad2;
} POLY_FT4;

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    short x1, y1;
    short x2, y2;
    short x3, y3;
} POLY_F4;

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short x1, y1;
    unsigned char u1, v1;
    unsigned short tpage;
    short x2, y2;
    unsigned char u2, v2;
    unsigned short pad1;
} POLY_FT3;

typedef struct { unsigned int tag; unsigned int code[2]; } DR_AREA;

typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
#define setaddr(_p, _addr) (((P_TAG *)(_p))->addr = (unsigned long)(_addr))
#define getaddr(_p) (unsigned long)(((P_TAG *)(_p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
#define setlen(p, _len) (((P_TAG *)(p))->len = (unsigned char)(_len))
#define setcode(p, _code) (((POLY_F4 *)(p))->code = (unsigned char)(_code))
#define setSemiTrans(p, abe) ((abe) ? setcode(p, ((POLY_F4 *)(p))->code | 0x02) : setcode(p, ((POLY_F4 *)(p))->code & ~0x02))

typedef struct {
    unsigned char model;
    char pad[0x17];
    VECTOR pos;
    char pad2[0xF8 - 0x28];
    int dir;
} OBJ;

typedef struct {
    char pad[0x78C];
    int active;
    char pad2[8];
    int f798;
    char pad3[8];
    OBJ *obj;
} CARX;

typedef struct { unsigned char b0, type, b2, b3, b4, b5; char pad[80 - 6]; } MODELINF;
typedef struct { char pad[0xF8]; int dir; char pad2[0x18C - 0xFC]; } CAR18C;
typedef struct { char pad[0x2D0]; int f2D0; } TGT;

extern VECTOR D_00010C84;
extern long long D_00097530;
#define current (*(DB **)&D_00097530)
extern long long D_00097534;
extern long long D_0009770C;
extern long long D_00097708;
extern long long D_000970D8;
extern long long D_000972E8;
extern long long D_00096850;
extern long long D_0009754C;
extern long long D_00096888;
extern int D_00096D24;
extern int D_00096D28;
extern int D_00096D54;
extern int D_000975B0;
extern int D_000975D0;
extern int D_000975D4;
extern int D_000975DC;
extern int D_000975EC;
extern unsigned short D_000975FC;
extern unsigned char D_00096D34;
extern int D_0008B278[];
extern int D_0008B2B8[];
extern MAPPOS D_0008B140[];
extern unsigned short D_00086F30[];
extern unsigned char D_000BEEAC[4][4];
extern CARX D_000C4E38[];
extern MODELINF D_00098900[];
extern CAR18C D_0009B0A4[];
extern char D_0009B0BC[];
extern char D_00096D1C[];
extern long long D_00097758;
extern char D_000BE9B4[];
extern TGT D_000C6F04;

extern void func_000563CC();
extern void func_000564A4();
extern void func_00054618();
extern void func_00074CAC();
extern void func_00055530();
extern void func_000573AC();
extern void func_00055A20();
extern void func_00053E54();
extern void func_00055CA0();
extern void func_00057458();
extern void func_00056278();
extern void func_0004EA98();
extern void func_0007EAC4();

void func_0005474C(void)
{
    int count;
    int count1;
    int x_mod;
    int y_mod;
    int MeshWidth;
    int MeshHeight;
    SVECTOR MapMesh[5][5];
    VECTOR MapMeshO[5][5];
    MATRIX map_matrix;
    MAPTEX MapTex[4];
    SVECTOR direction;
    RECT clipped_size;
    VECTOR translate;
    long flag;
    POLY_FT4 *spt;
    POLY_F4 *sptb;
    POLY_FT3 *null;
    DR_AREA *drarea;
    CARX *cp;
    int intens;
    int tmp;
    int tw, th;

    translate = D_00010C84;
    func_000563CC();
    x_mod = D_00096D24 & 0x1F;
    y_mod = D_00096D28 & 0x1F;
    func_000564A4();

    if (*(int *)&D_0009770C > 0 && *(int *)&D_00097708 >= 0x293) {
        func_00054618(D_0008B278[*(volatile int *)&D_000970D8 & 0xF], 0, D_0008B278[*(volatile int *)&D_000970D8 + 8 & 0xF]);
        if (*(int *)&D_0009770C == 1)
            *(char *)&D_000972E8 = 12;
    } else {
        if (*(int *)&D_0009770C == -1 && D_00096D54 == 0 && *(int *)&D_00097708 >= 0x293)
            D_00096D54 = 48;
        if (D_00096D54) {
            D_00096D54--;
            intens = 47 - D_00096D54;
            intens = (D_0008B2B8[intens / 4] + D_0008B2B8[(intens + 1) / 4] + D_0008B2B8[(intens + 2) / 4] + D_0008B2B8[(intens + 3) / 4]) / 4;
            func_00054618(intens, intens, intens);
        }
    }

    clipped_size.x = 0;
    if (*(int *)&D_00097534 & 1)
        clipped_size.y = 256;
    else
        clipped_size.y = 0;
    clipped_size.w = 512;
    clipped_size.h = 240;
    drarea = (DR_AREA *)current->primptr;
    func_00074CAC(drarea, &clipped_size);
    addPrim(current->ot, drarea);
    current->primptr += sizeof(DR_AREA);

    D_000975B0 = (D_000975B0 + 1) & 0xFF;

    for (cp = D_000C4E38; cp < &D_000C4E38[4]; cp++) {
        if (cp->active) {
            OBJ *o = cp->obj;
            int col = 0xFFFFFF;
            MODELINF *m = &D_00098900[o->model];
            if (m->type == 4 && ((D_000975B0 + m->b5 * 3) & 4) == 0)
                col = 0;
            func_00055530(&o->pos, col);
        }
    }

    func_00055530(D_0009B0BC, 0xFFFFFF);
    func_000573AC(D_0009B0BC, 1, D_00096D1C, 2, 0, 0x40);

    for (cp = D_000C4E38; cp < &D_000C4E38[4]; cp++) {
        if (cp->active && cp->f798 == 0)
            func_00055A20(&D_00097758, &cp->obj->pos, cp->obj->dir);
    }

    func_00053E54();

    if (D_000975DC && D_000C6F04.f2D0 != 2) {
        if (D_000975EC == 1)
            func_00055CA0(D_000BE9B4);
        func_00057458(&D_000C6F04);
    }

    if (x_mod < 16 && D_000975D0 > 16) {
        for (count1 = 0; count1 < 4; count1++) {
            tmp = D_000BEEAC[0][count1];
            D_000BEEAC[0][count1] = D_000BEEAC[1][count1];
            D_000BEEAC[1][count1] = D_000BEEAC[2][count1];
            D_000BEEAC[2][count1] = D_000BEEAC[3][count1];
            D_000BEEAC[3][count1] = tmp;
            func_00056278(tmp, (D_00096D24 >> 5) + 3, (D_00096D28 >> 5) + count1);
        }
    }
    if (x_mod > 16 && D_000975D0 < 16) {
        for (count1 = 0; count1 < 4; count1++) {
            tmp = D_000BEEAC[3][count1];
            D_000BEEAC[3][count1] = D_000BEEAC[2][count1];
            D_000BEEAC[2][count1] = D_000BEEAC[1][count1];
            D_000BEEAC[1][count1] = D_000BEEAC[0][count1];
            D_000BEEAC[0][count1] = tmp;
            func_00056278(tmp, D_00096D24 >> 5, (D_00096D28 >> 5) + count1);
        }
    }
    if (y_mod < 16 && D_000975D4 > 16) {
        for (count1 = 0; count1 < 4; count1++) {
            tmp = D_000BEEAC[count1][0];
            D_000BEEAC[count1][0] = D_000BEEAC[count1][1];
            D_000BEEAC[count1][1] = D_000BEEAC[count1][2];
            D_000BEEAC[count1][2] = D_000BEEAC[count1][3];
            D_000BEEAC[count1][3] = tmp;
            func_00056278(tmp, (D_00096D24 >> 5) + count1, (D_00096D28 >> 5) + 3);
        }
    }
    if (y_mod > 16 && D_000975D4 < 16) {
        for (count1 = 0; count1 < 4; count1++) {
            tmp = D_000BEEAC[count1][3];
            D_000BEEAC[count1][3] = D_000BEEAC[count1][2];
            D_000BEEAC[count1][2] = D_000BEEAC[count1][1];
            D_000BEEAC[count1][1] = D_000BEEAC[count1][0];
            D_000BEEAC[count1][0] = tmp;
            func_00056278(tmp, (D_00096D24 >> 5) + count1, D_00096D28 >> 5);
        }
    }
    D_000975D0 = x_mod;
    D_000975D4 = y_mod;

    MeshWidth = x_mod ? 4 : 3;
    MeshHeight = y_mod ? 4 : 3;

    for (count1 = 0; count1 < 5; count1++) {
        MapMesh[0][count1].vx = -48;
        MapMesh[1][count1].vx = -x_mod - 16;
        MapMesh[2][count1].vx = -x_mod + 16;
        MapMesh[3][count1].vx = -x_mod + 48;
        MapMesh[4][count1].vx = 48;
        MapMesh[count1][0].vy = -48;
        MapMesh[count1][1].vy = -y_mod - 16;
        MapMesh[count1][2].vy = -y_mod + 16;
        MapMesh[count1][3].vy = -y_mod + 48;
        MapMesh[count1][4].vy = 48;
    }

    MapTex[0].w = D_00096D34 - x_mod;
    MapTex[0].h = D_00096D34 - y_mod;
    MapTex[1].w = D_00096D34;
    MapTex[2].w = D_00096D34;
    MapTex[1].h = D_00096D34;
    MapTex[2].h = D_00096D34;
    MapTex[0].u = x_mod;
    MapTex[1].u = 0;
    MapTex[2].u = 0;
    MapTex[3].u = 0;
    MapTex[3].w = x_mod;
    MapTex[0].v = y_mod;
    MapTex[1].v = 0;
    MapTex[2].v = 0;
    MapTex[3].v = 0;
    MapTex[3].h = y_mod;

    direction.vy = 0;
    direction.vx = 0;
    direction.vz = 0xC00 - D_0009B0A4[*(int *)&D_00096888].dir & 0xFFF;
    func_0004EA98(&map_matrix, &direction);

    map_matrix.m[0][0] *= 3;
    map_matrix.m[0][1] *= 3;
    map_matrix.m[0][2] *= 3;
    map_matrix.m[0][0] >>= 1;
    map_matrix.m[0][1] >>= 1;
    map_matrix.m[0][2] >>= 1;

    gte_SetRotMatrix(&map_matrix);
    gte_SetTransVec0(&translate);

    for (count1 = 0; count1 <= MeshWidth; count1++)
        for (count = 0; count <= MeshHeight; count++)
            func_0007EAC4(&MapMesh[count1][count], &MapMeshO[count1][count], &flag);

    for (count1 = 0; count1 < MeshHeight; count1++) {
        for (count = 0; count < MeshWidth; count++) {
            if (D_0008B140[D_000BEEAC[count][count1]].x == 0x38)
                tw = MapTex[count].w - 1;
            else
                tw = MapTex[count].w;
            if (D_0008B140[D_000BEEAC[count][count1]].y == 0x60)
                th = MapTex[count1].h - 1;
            else
                th = MapTex[count1].h;

            spt = (POLY_FT4 *)current->primptr;
            setlen(spt, 9);
            setcode(spt, 0x2C);
            if (*(int *)&D_00096850)
                spt->b0 = spt->g0 = spt->r0 = 50;
            else
                spt->b0 = spt->g0 = spt->r0 = 100;
            setSemiTrans(spt, 1);
            spt->x0 = MapMeshO[count][count1].vx;
            spt->y0 = MapMeshO[count][count1].vy;
            spt->x1 = MapMeshO[count + 1][count1].vx;
            spt->y1 = MapMeshO[count + 1][count1].vy;
            spt->x2 = MapMeshO[count][count1 + 1].vx;
            spt->y2 = MapMeshO[count][count1 + 1].vy;
            spt->x3 = MapMeshO[count + 1][count1 + 1].vx;
            spt->y3 = MapMeshO[count + 1][count1 + 1].vy;
            spt->clut = D_000975FC;
            spt->tpage = D_00086F30[*(int *)&D_0009754C];
            spt->u0 = MapTex[count].u + D_0008B140[D_000BEEAC[count][count1]].x * 4;
            spt->v0 = MapTex[count1].v + D_0008B140[D_000BEEAC[count][count1]].y;
            spt->u1 = MapTex[count].u + D_0008B140[D_000BEEAC[count][count1]].x * 4 + tw;
            spt->v1 = MapTex[count1].v + D_0008B140[D_000BEEAC[count][count1]].y;
            spt->u2 = MapTex[count].u + D_0008B140[D_000BEEAC[count][count1]].x * 4;
            spt->v2 = MapTex[count1].v + D_0008B140[D_000BEEAC[count][count1]].y + th;
            spt->u3 = MapTex[count].u + D_0008B140[D_000BEEAC[count][count1]].x * 4 + tw;
            spt->v3 = MapTex[count1].v + D_0008B140[D_000BEEAC[count][count1]].y + th;
            addPrim(current->ot, spt);
            current->primptr += sizeof(POLY_FT4);
        }
    }

    sptb = (POLY_F4 *)current->primptr;
    setlen(sptb, 5);
    setcode(sptb, 0x28);
    sptb->r0 = 60;
    sptb->g0 = 60;
    sptb->b0 = 60;
    setSemiTrans(sptb, 1);
    sptb->x0 = 400;
    sptb->y0 = 160;
    sptb->x1 = 496;
    sptb->y1 = 160;
    sptb->x2 = 400;
    sptb->y2 = 220;
    sptb->x3 = 496;
    sptb->y3 = 220;
    addPrim(current->ot, sptb);
    current->primptr += sizeof(POLY_FT4);

    null = (POLY_FT3 *)current->primptr;
    setlen(null, 7);
    setcode(null, 0x24);
    null->x0 = -1;
    null->y0 = -1;
    null->x1 = -1;
    null->y1 = -1;
    null->x2 = -1;
    null->y2 = -1;
    null->tpage = 0;
    addPrim(current->ot, null);
    current->primptr += sizeof(POLY_FT3);

    if (*(int *)&D_00097534 & 1) {
        clipped_size.x = 400;
        clipped_size.y = 416;
    } else {
        clipped_size.x = 400;
        clipped_size.y = 160;
    }
    clipped_size.w = 96;
    clipped_size.h = 60;
    drarea = (DR_AREA *)current->primptr;
    func_00074CAC(drarea, &clipped_size);
    addPrim(current->ot, drarea);
    current->primptr += sizeof(DR_AREA);
}
