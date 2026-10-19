/* FAKEMATCH: the func_00026F28 / func_00031048 table entries use extra __asm__ alias names so CSE keeps
 * them apart (the original probably referenced distinct asm labels at those addresses). */
/* rcossin prologue (run2-r3): five loads are written as *(short *)((int)D_0008EAAA + i*4 [- 2]) (scalar MEMs, so they
 * conflict with the D_00096998 = 0 store and pin its position); leftcos stays an array ref (MEM_IN_STRUCT, free). */
/* func_0002C8FC: Driver 1 DrawMapPSX (Driver 2: DrawMapPSX, draw.c) - row/column walk of the visible cells */
#include "common.h"
#include "gte.h"

#define gte_ReadTransZ(r0) __asm__ volatile ( \
    "cfc2  $12, $7;" "nop;" "sw    $12, 0( %0 )" : : "r"(r0) : "$12", "memory")

typedef struct {
    int vx, vy, vz;
    unsigned char computed;
    unsigned char yang;
    unsigned short type;
} CELL_OBJECT;

typedef struct {
    unsigned short shape_flags;
    unsigned short flags2;
    int f4;
    short bounding_sphere;
    short fA;
    short fC;
    unsigned short fE;
    int f10;
    int f14;
} MODEL;

typedef struct {
    unsigned short num;
    unsigned short next;
} CELL_DATA;

typedef struct {
    short m[3][3];
    short computed;
    int t[3];
} MATRIX2;

typedef struct { char *primptr; unsigned int *ot; } DB;

extern long long D_00096804, D_00097534, D_0009707C, D_0009733C;
extern long long D_00097318, D_00097310, D_00097328;
extern long long D_00096A24, D_00096A28;
extern long long D_000970E0, D_000970D4, D_00097108;
extern long long D_00097424, D_00096850, D_00096834, D_0009715C, D_00097364, D_00096AA8;
extern long long D_00096C4A;
extern int D_000971B4;

extern long long D_00097530;
extern int D_00096998, D_00096984;
extern CELL_OBJECT **D_0009697C;
extern CELL_OBJECT **D_0009719C;
extern CELL_OBJECT *D_000A5E0C[];
extern CELL_OBJECT *D_000A663C[];
extern CELL_OBJECT *D_000A76AC[];
extern MODEL *D_000A0E20[];
extern MATRIX D_000BBE84;
extern MATRIX D_000AF9C0;
extern MATRIX2 D_000A5E3C[];
extern MATRIX D_000A6E8C[];
extern unsigned char D_000A80C4[];
extern short D_0008EAAA[];
extern VECTOR D_0008A8F0;

extern void func_00053828();
extern void func_0002D7C4();
extern int func_0002C5F0();
extern int func_0002C6DC();
extern void func_0002BDB0();
extern void func_0002C29C();
extern void func_0002C598();
extern void func_000270D4();
extern void func_0002EB94();
extern int func_00030ED0();
extern void func_1CC894();
extern void func_0002C564();
extern void func_00052C10();
extern long long func_00026F28, func_00026DD0, func_00026E54, func_00026C38;
extern long long func_00030380, func_000303E8, func_0003056C, func_00031048;
extern long long func_00030464, func_00030718, func_000307E8, func_00030F60;
extern long long L00030520 __asm__(".L00030520");
extern long long func_00026F28_20 __asm__("func_00026F28");
extern long long func_00026F28_10 __asm__("func_00026F28");
extern long long func_00031048_b __asm__("func_00031048");
extern long long func_00031048_c __asm__("func_00031048");
extern long long func_00031048_d __asm__("func_00031048");

#define current (*(DB **)&D_00097530)
#define cells_across (*(int *)&D_00096A24)
#define cells_down (*(int *)&D_00096A28)
#define region (*(int *)&D_0009733C)
#define frame (*(int *)&D_0009707C)
#define rx_ (*(unsigned char *)&D_00097310)
#define rz_ (*(unsigned char *)&D_00097318)
#define rsel (*(unsigned char *)&D_00097328)

void func_0002C8FC(void)
{
    VECTOR pos;
    int cellx;
    int cellz;
    int ambient;
    int z;
    CELL_DATA *cd;
    int modz;
    int row;
    int lim;
    int rsq;
    unsigned char *pvs;
    int tiles;
    int nbuild;
    int boff;
    CELL_OBJECT **blist;
    CELL_OBJECT **slist;
    int rightcos, rightsin, leftcos, leftsin, backcos, backsin;
    int rightPlane, leftPlane, backPlane;
    int cr, cl, cb;
    int start;
    int vx;
    int col;
    int modx;
    int nanim;
    int nspr;
    CELL_OBJECT **ap;
    CELL_OBJECT *cop;
    MODEL *model;
    int flags;
    int i;
    unsigned int idx;
    int *p;
    int *q;

    blist = (CELL_OBJECT **)0x1F800170;
    slist = (CELL_OBJECT **)0x1F800200;

    rightcos = *(short *)((int)D_0008EAAA + ((*(short *)&D_00096C4A - D_000971B4 / 2) & 0xfff) * 4);
    rightsin = *(short *)((int)D_0008EAAA + ((*(short *)&D_00096C4A - D_000971B4 / 2) & 0xfff) * 4 - 2);
    leftsin = *(short *)((int)D_0008EAAA + ((*(short *)&D_00096C4A + D_000971B4 / 2) & 0xfff) * 4 - 2);
    backsin = *(short *)((int)D_0008EAAA + ((*(short *)&D_00096C4A + 0x400) & 0xfff) * 4 - 2);
    D_00096998 = 0;
    backcos = *(short *)((int)D_0008EAAA + ((*(short *)&D_00096C4A + 0x400) & 0xfff) * 4);
    rightPlane = -((rightcos + rightsin) * 9) - 8144;
    leftcos = D_0008EAAA[((*(short *)&D_00096C4A + D_000971B4 / 2) & 0xfff) * 2];
    leftPlane = -((leftcos + leftsin) * 9) + 8144;
    backPlane = -((backcos + backsin) * 9) + 6144;


    if (*(int *)&D_00096804 == 0)
        func_00053828();

    frame = *(volatile int *)&D_00097534 & 0xff;

    func_0002D7C4(&cellx, &cellz, &ambient);

    rz_ = cellz / region;
    modz = cellz - rz_ * region;
    rsq = region * region;
    nanim = 0;
    nspr = 0;
    nbuild = 0;
    boff = 0;
    tiles = 0;
    pvs = D_000A80C4;

    for (row = 0; row < 19; row++) {
        cr = rightPlane;
        cl = leftPlane;
        cb = backPlane;
        rightPlane += rightsin;
        leftPlane += leftsin;
        backPlane += backsin;

        start = 0;
        if (row < 4)
            start = 4 - row;
        else if (18 - row < 4)
            start = row - 14;

        if (start) {
            cr += start * rightcos;
            cl += start * leftcos;
            cb += start * backcos;
        }

        vx = start + cellx;
        rx_ = vx / region;
        modx = vx - rx_ * region;

        lim = 19 - start;
        for (col = start; col < lim; col++, vx++) {
            if (cr < 0 && cl > 0 && cb > 0 && vx >= 0 && cellz >= 0 &&
                vx < cells_across && cellz < cells_down && pvs[col]) {
                rsel = (rx_ & 1) + ((rz_ & 1) << 1);
                idx = (*(unsigned short **)&D_000970E0)[rsel * rsq + modx + modz * region];
                if (idx != 0xFFFF) {
                    ap = (CELL_OBJECT **)(nanim * 4 + (int)D_000A663C);
                    do {
                        cd = &(*(CELL_DATA **)&D_000970D4)[idx];
                        cop = &(*(CELL_OBJECT **)&D_00097108)[cd->num];
                        if (cop->computed != frame) {
                            model = D_000A0E20[cop->type];
                            flags = model->shape_flags;
                            if (cop->vx != 0xFD46FEC0 && !(flags & 0x10) &&
                                func_0002C5F0(cop, model->bounding_sphere) != -1) {
                                if (flags & 0x4000) {
                                    current->primptr = (char *)func_0002C6DC(model, current->primptr, current->ot, cop->yang);
                                    gte_ReadTransZ(&z);
                                    if (*(int *)&D_00097424 == 0 && *(int *)&D_00096850 == 0 &&
                                        !(cop->yang & 0x20) && z < 10000) {
                                        gte_SetRotMatrix(&D_000BBE84);
                                        if (z < 2000) {
                                            func_0002BDB0(D_000A0E20[cop->type]->f14, D_000A0E20[cop->type]->f10);
                                            if (model->fE == 2)
                                                func_0002BDB0(D_000A0E20[cop->type]->f14 + 0x14, D_000A0E20[cop->type]->f10);
                                        } else {
                                            func_0002C29C(D_000A0E20[cop->type]->f14, D_000A0E20[cop->type]->f10);
                                            if (model->fE == 2)
                                                func_0002C29C(D_000A0E20[cop->type]->f14 + 0x14, D_000A0E20[cop->type]->f10);
                                        }
                                        gte_SetRotMatrix(&D_000AF9C0);
                                    }
                                    if (model->flags2 & 0x2000) {
                                        if ((D_00096998++ & 0xf) == 0) {
                                            if ((int)D_0009697C < (int)D_000A5E0C)
                                                *D_0009697C++ = cop;
                                        }
                                    }
                                    if (flags & 4) {
                                        if (nanim == 20) {
                                            i = 19;
 ap = &D_000A663C[i];
 nanim = i;
                                        }
                                        *ap++ = cop;
                                        nanim++;
                                    }
                                } else if (*(unsigned int *)model & 0xC0000480) {
                                    if (model->flags2 & 0x80) {
                                        if ((D_00096984++ & 0xf) == 0) {
                                            if ((int)D_0009697C < (int)D_000A5E0C)
                                                *D_0009697C++ = cop;
                                        }
                                    }
                                    if (nspr < 128)
                                        slist[nspr] = cop;
                                    else
                                        (*(CELL_OBJECT ***)&D_0009715C)[nspr - 128] = cop;
                                    nspr++;
                                } else if (flags & 8) {
                                    D_0009719C[tiles++] = cop;
                                } else {
                                    if (cop->yang & 0xf) {
                                        if (D_000A5E3C[cop->yang].computed != frame) {
                                            D_000A5E3C[cop->yang].computed = frame;
                                            MulMatrix0(&D_000BBE84, &D_000A6E8C[cop->yang], (MATRIX *)&D_000A5E3C[cop->yang]);
                                        }
                                        gte_SetRotMatrix(&D_000AF9C0);
                                    }
                                    if (flags & 4) {
                                        *ap++ = cop;
                                        nanim++;
                                        if (nanim >= 11) {
                                            i = 11;
 ap = &D_000A663C[i];
 nanim = i;
                                        }
                                    }
                                    blist[nbuild & 0x1f] = cop;
                                    nbuild++;
                                    if ((nbuild & 0x1f) == 0) {
                                        func_0002C598((char *)D_000A76AC + boff, blist, 0x80);
                                        boff += 0x80;
                                    }
                                }
                            }
                            cop->computed = frame;
                        }
                        idx = cd->next;
                    } while (idx != 0xFFFF);
                }
            }
            if (++modx == region) {
                modx = 0;
                rx_++;
            }
            cl += leftcos;
            cb += backcos;
            cr += rightcos;
        }
        if (++modz == region) {
            modz = 0;
            rz_++;
        }
        pvs += *(int *)&D_00097364;
        cellz++;
    }

    if (nbuild & 0x1f)
        func_0002C598((char *)D_000A76AC + boff, blist, (nbuild & 0x1f) * 4);

    if (nspr) {
        q = (int *)0x1F800000;
        q[8] = (int)(char *)&func_00026F28_20 + 0x20;
        q[9] = (int)&func_00026F28;
        q[10] = (int)&func_00026DD0;
        q[11] = (int)(char *)&func_00026F28_10 + 0x10;
        q[12] = (int)&func_00026E54;
        q[13] = (int)&func_00026C38;
        func_000270D4(nspr);
    }

    if (nbuild) {
        if (*(int *)&D_00096804 == 1) {
            func_1CC894(D_000A76AC, nbuild);
        } else {
            *(unsigned int *)0x1F800000 = current->ot[0x107E] & 0xFFFFFF;
            p = (int *)0x1F800024;
            if (*(unsigned char *)&D_00096AA8 == 0 && *(int *)&D_00096850 == 0) {
                p[0] = 0x808080;
                p[1] = 0x707070;
                p[2] = 0x606060;
                p[3] = 0x505050;
                p[4] = 0x404040;
                p[5] = 0x505050;
                p[6] = 0x606060;
                p[7] = 0x707070;
            } else {
                p[0] = ambient;
                p[1] = ambient + 0x10101;
                p[2] = ambient + 0x20202;
                p[3] = ambient + 0x30303;
                p[4] = ambient + 0x40404;
                p[5] = ambient + 0x30303;
                p[6] = ambient + 0x20202;
                p[7] = ambient + 0x10101;
            }
            p = (int *)0x1F800080;
            p[0] = (int)&func_00030380;
            p[1] = (int)&func_000303E8;
            p[2] = (int)&L00030520;
            p[3] = (int)&func_0003056C;
            p[4] = (int)&func_00031048;
            p[5] = (int)&func_00031048_b;
            p[6] = (int)&func_00030464;
            p[7] = (int)&func_00030718;
            p[8] = (int)&func_00031048_c;
            p[9] = (int)&func_00031048_d;
            p[10] = (int)&func_000307E8;
            p[11] = (int)&func_00030F60;
            p = (int *)0x1F8001AC;
            if (*(int *)&D_00096834 == 1 || *(int *)&D_00096834 == 5)
                p[0] = (int)current->ot + 0xC8;
            else
                p[0] = (int)current->ot;
            p = (int *)0x1F8001B0;
            p[0] = 0x1C609D8E;
            p[1] = 0x1F3C596;
            p[2] = 0x3D8B1B2C;
            p[3] = 0x330;
            p[4] = 0x182FC2C6;
            p[5] = 0xA54BD50;
            p[6] = 0x41492398;
            p[7] = 0x2B62;
            current->primptr = (char *)func_00030ED0(D_000A76AC, nbuild, current);
        }
    }

    for (i = 0; i < nanim; i++) {
        cop = D_000A663C[i];
        pos.vx = cop->vx - D_0008A8F0.vx;
        pos.vy = cop->vy - D_0008A8F0.vy;
        pos.vz = cop->vz - D_0008A8F0.vz;
        func_0002C564(&pos);
        gte_SetRotMatrix(&D_000A6E8C[cop->yang]);
        func_00052C10(D_000A0E20[cop->type], cop, &pos);
    }

    if (tiles)
        func_0002EB94(tiles);
}

