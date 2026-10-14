/* HandleDebris (Driver 2 name) */
#include "gte.h"

typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct { short vx, vy, vz, pad; } SVECTOR;

typedef struct {
    VECTOR position;           /* 0x00 */
    SVECTOR direction;         /* 0x10 */
    unsigned short life;       /* 0x18 */
    unsigned short flags;      /* 0x1A */
    unsigned short num;        /* 0x1C */
    unsigned short pos;        /* 0x1E */
    unsigned char rgb[4];      /* 0x20 */
    unsigned char step;        /* 0x24 */
    unsigned char type;        /* 0x25 */
    short pad;                 /* 0x26 */
} DEBRIS;

typedef struct {
    VECTOR position;           /* 0x00 */
    SVECTOR direction;         /* 0x10 */
    unsigned short life;       /* 0x18 */
    unsigned short flags;      /* 0x1A */
    unsigned short num;        /* 0x1C */
    unsigned short pos;        /* 0x1E */
    unsigned char rgb[4];      /* 0x20 */
    unsigned char step;        /* 0x24 */
    unsigned char type;        /* 0x25 */
    unsigned short sin_index1; /* 0x26 */
    unsigned short sin_index2; /* 0x28 */
    unsigned char sin_addition1; /* 0x2A */
    unsigned char sin_addition2; /* 0x2B */
} LEAF;

typedef struct {
    VECTOR position;           /* 0x00 */
    VECTOR drift;              /* 0x10 */
    SVECTOR drift_change;      /* 0x20 */
    VECTOR final_tail_pos;     /* 0x28 */
    short start_w;             /* 0x38 */
    short final_w;             /* 0x3A */
    short transparency;        /* 0x3C */
    short t_step;              /* 0x3E */
    short step;                /* 0x40 */
    short pad;                 /* 0x42 */
    unsigned char life;        /* 0x44 */
    unsigned char halflife;    /* 0x45 */
    short pad2;                /* 0x46 */
    int flags;                 /* 0x48 */
    unsigned short num;        /* 0x4C */
    short pad3;                /* 0x4E */
} SMOKE;

typedef struct { short type; short height; short objectAngle; short pad; } ROUTE_DATA;
typedef struct { unsigned short shape_flags; } MODEL;
typedef struct { short m[3][3]; int t[3]; } MATRIX;

extern short D_0008EAA8[];         /* rcossin_tbl */
extern long long D_00096854;
extern long long D_0009687C;       /* pauseflag */
extern unsigned char D_00096AA8;
extern unsigned char D_00096AA9;
extern int D_00096AB0;             /* variable_weather */
extern int D_00096C3C;             /* weather_level */
extern long long D_000970D8;       /* CameraCnt */
extern int D_00097414;             /* next_debris */
extern int D_0009742C;             /* next_smoke */
extern int D_00097430;             /* next_leaf */
extern int D_00097488;             /* main_cop_light_pos */
extern MODEL *D_000A0E20[];        /* modelpointers */
extern LEAF D_000B7584[];          /* leaf */
extern short D_000B7EC8[];         /* leaf_alloc */
extern MATRIX D_000B8FE8;
extern short D_000B9048[];         /* debris_alloc */
extern SMOKE D_000B92A0[];         /* smoke */
extern DEBRIS D_000BABC0[];        /* debris */
extern short D_000BB530[];         /* smoke_alloc */
extern MATRIX D_000BBE84;          /* inv_camera_matrix */

extern void func_000217C8();       /* ROADS_GetRouteData */
extern void func_0003F01C();       /* MoveHubcap */
extern void func_00041690();       /* HandleBirds */
extern void func_00045A04();       /* DisplayWater */
extern void func_00045CEC();       /* DisplaySpark */
extern void func_000466D4();       /* GetSmokeDrift */
extern void func_000481C8();       /* DisplayDebris */
extern void func_00048E88();       /* DisplaySmoke */
extern void func_000774C4();       /* memset */
extern void func_00079D20();       /* SetRotMatrix */

#define pauseflag (*(int *)&D_0009687C)

void func_00048660(void)
{
    int count;
    int i;
    DEBRIS *lpDebris;
    SMOKE *smokeptr;
    VECTOR dummy;
    LEAF *lpLeaf;
    int Height;
    int SinX;
    int CosX;
    VECTOR Drift;

    func_000774C4(&dummy, 0, 16);
    SinX = 0;
    func_000466D4(&Drift);
    func_0003F01C();
    func_00041690();
    func_00079D20(&D_000BBE84);
    gte_SetTransVec0(&dummy);

    if (D_00097414 != 0) {
        count = D_00097414;
        lpDebris = D_000BABC0;
        if (pauseflag != 0) {
            do {
                if (lpDebris->flags & 2) {
                    func_000481C8(lpDebris, 0);
                    count--;
                }
                lpDebris++;
            } while (count != 0);
        } else {
            do {
                if (lpDebris->flags & 2) {
                    func_000481C8(lpDebris, 0);
                    if (--lpDebris->life == 0 || lpDebris->position.vy > 0) {
                        lpDebris->flags = 0;
                        D_000B9048[--D_00097414] = lpDebris->num;
                    } else {
                        lpDebris->position.vx += lpDebris->direction.vx;
                        lpDebris->position.vy += lpDebris->direction.vy;
                        lpDebris->position.vz += lpDebris->direction.vz;
                        lpDebris->pos += lpDebris->step;
                        if (lpDebris->type == 3)
                            lpDebris->direction.vy += 1;
                        else
                            lpDebris->direction.vy += 6;
                    }
                    count--;
                }
                lpDebris++;
            } while (count != 0);
        }
    }

    if (D_00097430 != 0) {
        count = D_00097430;
        lpLeaf = D_000B7584;
        if (pauseflag != 0) {
            do {
                if (lpLeaf->flags & 2) {
                    func_000481C8(lpLeaf, lpLeaf->type);
                    count--;
                }
                lpLeaf++;
            } while (count != 0);
        } else {
            do {
                if (lpLeaf->flags & 2) {
                    if (lpLeaf->life == 1) {
                        lpLeaf->flags = 0;
                        D_000B7EC8[--D_00097430] = lpLeaf->num;
                    } else {
                        if (pauseflag == 0) {
                            int sn1, sn2, cs1, cs2;
                            lpLeaf->sin_index1 += lpLeaf->sin_addition1;
                            lpLeaf->sin_index2 += lpLeaf->sin_addition2;
                            lpLeaf->sin_index1 &= 0xfff;
                            lpLeaf->sin_index2 &= 0xfff;
                            sn1 = D_0008EAA8[(lpLeaf->sin_index1 & 0xfff) * 2];
                            sn2 = D_0008EAA8[(lpLeaf->sin_index2 & 0xfff) * 2];
                            cs1 = D_0008EAA8[(lpLeaf->sin_index1 & 0xfff) * 2 + 1];
                            cs2 = D_0008EAA8[(lpLeaf->sin_index2 & 0xfff) * 2 + 1];
                            SinX = sn1 + sn2;
                            CosX = cs1 + cs2;
                            lpLeaf->direction.vx = (SinX * 5) >> 11;
                            lpLeaf->direction.vz = (CosX * 5) >> 11;
                        }
                        func_000466D4(&Drift);
                        if (*(int *)&D_00096854 != 0)
                            Height = 0;
                        else
                            Height = -50;
                        if (lpLeaf->position.vy >= Height) {
                            lpLeaf->position.vy = Height;
                        } else {
                            lpLeaf->position.vx += lpLeaf->direction.vx + Drift.vx;
                            lpLeaf->position.vy += lpLeaf->direction.vy + Drift.vy;
                            lpLeaf->position.vz += lpLeaf->direction.vz + Drift.vz;
                            lpLeaf->pos += lpLeaf->step;
                            lpLeaf->pos &= 0xff;
                        }
                        lpLeaf->direction.vy = (SinX >> 11) + 4;
                    }
                    count--;
                    func_000481C8(lpLeaf, lpLeaf->type);
                }
                lpLeaf++;
            } while (count != 0);
        }
    }

    D_000B8FE8.m[0][0] = 0x1000;
    D_000B8FE8.m[1][0] = 0;
    D_000B8FE8.m[2][0] = 0;
    D_000B8FE8.m[0][1] = 0;
    D_000B8FE8.m[1][1] = 0x1000;
    D_000B8FE8.m[2][1] = 0;
    D_000B8FE8.m[0][2] = 0;
    D_000B8FE8.m[1][2] = 0;
    D_000B8FE8.m[2][2] = 0x1000;

    smokeptr = D_000B92A0;
    for (i = 0; i < 80; i++) {
        if (smokeptr->flags & 2) {
            if (smokeptr->flags & 4) {
                if (smokeptr->flags & 0x8000)
                    func_00045A04(smokeptr);
                else
                    func_00048E88(smokeptr);
                if (pauseflag == 0) {
                    if (smokeptr->flags & 0x8000) {
                        int offshore;
                        ROUTE_DATA routeData;
                        func_000217C8(smokeptr->position.vx - smokeptr->start_w,
                                      smokeptr->position.vz - smokeptr->start_w, &routeData);
                        offshore = D_000A0E20[routeData.type]->shape_flags & 0x80;
                        func_000217C8(smokeptr->position.vx + smokeptr->start_w,
                                      smokeptr->position.vz + smokeptr->start_w, &routeData);
                        offshore &= D_000A0E20[routeData.type]->shape_flags & 0x80;
                        if (smokeptr->start_w < 800 && offshore)
                            smokeptr->start_w += smokeptr->step;
                        else
                            smokeptr->start_w -= smokeptr->step;
                    } else {
                        if (smokeptr->start_w < 800)
                            smokeptr->start_w += smokeptr->step;
                        smokeptr->position.vx += smokeptr->drift.vx;
                        smokeptr->position.vy += smokeptr->drift.vy;
                        smokeptr->position.vz += smokeptr->drift.vz;
                        if (smokeptr->life > smokeptr->halflife) {
                            smokeptr->drift.vx -= smokeptr->drift_change.vx;
                            smokeptr->drift.vy -= smokeptr->drift_change.vy;
                            smokeptr->drift.vz -= smokeptr->drift_change.vz;
                        }
                    }
                }
            } else if (smokeptr->flags & 8) {
                func_00045CEC(smokeptr);
                if (pauseflag == 0) {
                    smokeptr->final_tail_pos = smokeptr->drift;
                    smokeptr->drift = smokeptr->position;
                    smokeptr->position.vx += smokeptr->drift_change.vx;
                    smokeptr->position.vy += smokeptr->drift_change.vy;
                    smokeptr->position.vz += smokeptr->drift_change.vz;
                    smokeptr->drift_change.vy += 6;
                    if (smokeptr->position.vy > 0) {
                        smokeptr->flags = 0;
                        D_000BB530[--D_0009742C] = smokeptr->num;
                    }
                }
            }
            if ((smokeptr->flags & 0x900C) && pauseflag == 0) {
                smokeptr->transparency -= smokeptr->t_step;
                if (smokeptr->transparency < 1) {
                    smokeptr->transparency = 0;
                    smokeptr->life = 1;
                }
                if (--smokeptr->life == 0) {
                    smokeptr->flags = 0;
                    D_000BB530[--D_0009742C] = smokeptr->num;
                }
            }
        }
        smokeptr++;
    }

    D_00097488 = (D_00097488 + 1) & 7;

    if (D_00096AB0 != 0) {
        D_00096C3C = D_0008EAA8[(*(int *)&D_000970D8 & 0xfff) * 2] +
                     D_0008EAA8[((*(int *)&D_000970D8 >> 2) & 0xfff) * 2];
        if (D_00096C3C > 0) {
            D_00096AA8 = D_00096C3C >> 8;
            D_00096AA9 = 0;
            if (D_00096AA8 > 180)
                D_00096AA8 = 180;
        } else {
            D_00096AA9 = -D_00096C3C >> 8;
            if (D_00096AA9 > 180)
                D_00096AA9 = 180;
            D_00096AA8 = 0;
        }
    }
}
