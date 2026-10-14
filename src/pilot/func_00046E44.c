/* FAKEMATCH: second extern name (__asm__ alias) for the same symbol to defeat CSE */
/* Driver 2: DrawLensFlare (SKY.C) */
#include "gte.h"

typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct { short vx, vy; } DVECTOR;
typedef struct { short x, y, w, h; } RECT16;
typedef struct { unsigned char r, g, b, cd; } CVECTOR;
typedef struct { int m[5]; } MATRIX;
typedef struct {
    char *primptr;
    unsigned int *ot;
    char pad[0x6E - 8];
    unsigned short disp_y;
} DB;

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

typedef struct { unsigned int tag; unsigned int code[5]; } DR_MOVE;

typedef struct {
    unsigned char u0, v0, u1, v1, u2, v2, u3, v3;
    unsigned short tpageid;
    unsigned short clutid;
} TEXINF;

typedef struct {
    short r, g, b, pad;
    unsigned char size;
    char pad2;
    short gapmod;
} FLAREREC;

extern long long D_00097530;
#define current (*(DB **)&D_00097530)
extern long long D_00096850;
#define gTimeOfDay (*(int *)&D_00096850)
extern long long D_00096834;
#define GameLevel (*(int *)&D_00096834)
extern unsigned char D_00096AA8;
extern unsigned char D_000973F8;
extern RECT16 D_00096C34[];
extern SVECTOR D_00096C2C;
extern SVECTOR D_00096C24;
extern MATRIX D_000BBE84;
extern VECTOR D_0008909C;
extern short D_000B722C[];
extern FLAREREC D_0008A878[];
extern TEXINF D_000B9110;
/* u0 read through a second name for the same object: keeps its %hi out of the hoisted &D_000B9110 */
extern TEXINF lf_tex __asm__("D_000B9110");

extern void func_00074464();
extern int SquareRoot0();
extern void func_000473F0();
extern void func_000493CC();
extern void SetDrawMove();

#define gte_stsxy(r0) __asm__ volatile ("swc2  $14, 0( %0 )" : : "r"(r0) : "memory")
#define gte_rtps() __asm__ volatile ("nop;" "nop;" "rtps")

typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
#define setaddr(_p, _addr) (((P_TAG *)(_p))->addr = (unsigned long)(_addr))
#define getaddr(_p) (unsigned long)(((P_TAG *)(_p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)

void func_00046E44(void)
{
    DVECTOR sun_pers_conv_position;
    RECT16 source;
    CVECTOR col;
    int flarez;
    int distance_to_sun;
    int xgap;
    int ygap;
    int flare_col;
    int shade;
    int haze_col;
    int bufferX;
    int bufferY;
    short *pwBuffer;
    FLAREREC *pFlareInfo;
    POLY_FT4 *poly;
    DR_MOVE *sample_sun;

    source = D_00096C34[0];

    if (gTimeOfDay != 0) { col.r = 0x60; col.b = 0x60; col.g = 0x60; } else { col.r = 0xFF; col.b = 0xFF; col.g = 0xFF; }

    gte_SetRotMatrix(&D_000BBE84);
    gte_SetTransVec0(&D_0008909C);

    if (gTimeOfDay != 0)
        gte_ldv0(&D_00096C2C);
    else
        gte_ldv0(&D_00096C24);

    gte_rtps();
    gte_stsxy(&sun_pers_conv_position);
    gte_stsz(&flarez);

    flare_col = 0;

    if (D_000973F8 == 0) {
        func_00074464(&source, D_000B722C);
        pwBuffer = D_000B722C;

        for (bufferY = 2; bufferY < 14; bufferY++) {
            for (bufferX = 0; bufferX < 10; bufferX++) {
                if (*pwBuffer == -1 || *pwBuffer == 0x7FFF)
                    flare_col++;
                pwBuffer++;
            }
            pwBuffer += 4;
        }
    }

    xgap = sun_pers_conv_position.vx - 256;
    ygap = sun_pers_conv_position.vy - 120;

    distance_to_sun = SquareRoot0(xgap * xgap + ygap * ygap);

    if (flarez > 0 && distance_to_sun < 500) {
        if (gTimeOfDay == 0 && D_00096AA8 == 0 && GameLevel != 3) {
            func_000473F0(&sun_pers_conv_position, &col, 11, flare_col);
            haze_col = (60 - (distance_to_sun >> 1)) * flare_col;
            func_000493CC(haze_col >> 5, haze_col >> 6, 7);
        }

        shade = flare_col / 3;
        shade -= (distance_to_sun * 80) / 500;

        if (gTimeOfDay != 0)
            shade >>= 1;

        if (shade >= 0 && flare_col != 0) {
            for (pFlareInfo = D_0008A878; pFlareInfo < D_0008A878 + 8; pFlareInfo++) {
                int xpos, ypos, gapmod, r, g, b;
                int flaresize;

                gapmod = (128 - pFlareInfo->gapmod) * 2;
                xpos = (xgap * gapmod) / 256 + 256;
                gapmod = (ygap * gapmod) / 256;
                ypos = gapmod + 120;
                flaresize = pFlareInfo->size;

                poly = (POLY_FT4 *)current->primptr;
                ((P_TAG *)poly)->len = 9;
                poly->code = 0x2C;

                r = pFlareInfo->r * shade;
                g = pFlareInfo->g * shade;
                b = pFlareInfo->b * shade;

                poly->code |= 2;

                xpos -= (flaresize * 3) >> 2;
                ypos -= (unsigned)flaresize >> 1;
                poly->y0 = ypos;
                poly->y1 = ypos;
                poly->x0 = xpos;
                poly->x1 = xpos + ((flaresize * 3) >> 1);
                poly->x2 = xpos;
                poly->r0 = r >> 6;
                poly->g0 = g >> 6;
                poly->b0 = b >> 6;
                poly->y2 = ypos + pFlareInfo->size;
                poly->x3 = xpos + ((flaresize * 3) >> 1);
                poly->y3 = ypos + pFlareInfo->size;

                poly->u0 = lf_tex.u0;
                poly->v0 = D_000B9110.v0;
                poly->u1 = D_000B9110.u1;
                poly->v1 = D_000B9110.v1;
                poly->u2 = D_000B9110.u2;
                poly->v2 = D_000B9110.v2 - 4;
                poly->u3 = D_000B9110.u3;
                poly->v3 = D_000B9110.v3 - 4;

                poly->clut = D_000B9110.clutid;
                poly->tpage = D_000B9110.tpageid | 0x20;

                addPrim(current->ot + 10, poly);
                current->primptr += sizeof(POLY_FT4);
            }
        }
    }

    source.x = sun_pers_conv_position.vx - 8;
    source.y = sun_pers_conv_position.vy - 4;

    if (source.x < 0 || source.y < 0 || source.x + 16 > 512 || source.y + 10 > 240) {
        D_000973F8 = 1;
    } else {
        D_000973F8 = 0;
        source.y = source.y + current->disp_y;
        sample_sun = (DR_MOVE *)current->primptr;
        SetDrawMove(sample_sun, &source, 1008, 449);
        addPrim(current->ot + 32, sample_sun);
        current->primptr += sizeof(DR_MOVE);
    }
}
