/* Driver 1 decomp - shared records (Step 3 of pilot/TU-PLAN.md).
 *
 * One definition of each record that recurs across the main-exe translation units, built from the UNION of
 * the fields every TU touches, laid out by offset. Names come from Driver 2 (REDRIVER2 dr2types.h,
 * engine/mdl.h, cell.h, tset.h, replays.h) where the D1 layout agrees, `fXX` (= offset) otherwise.
 * Every field that any TU reads is checked by an OFS_ASSERT below (gcc 2.7 has no _Static_assert or
 * offsetof: the negative-array-size trick fails the compile on a layout drift).
 *
 * Rules for TUs (they keep byte-identical codegen; see pilot/TU1.md Step 3 lessons):
 *  - Include it after any "common.h": the gte base types (VECTOR, SVECTOR, MATRIX, CVECTOR) are only
 *    defined here when common.h (PsyQ libgte) was not included first; both spellings have the same layout.
 *  - Plain char is unsigned (-funsigned-char). Field signedness/width follows the majority of the
 *    original loads (lb/lbu, lh/lhu); a TU whose original code needs the other one casts that access:
 *    `*(short *)&cp->f17C`, `(unsigned short)cp->totalDamage`.
 *  - Nothing here declares a global or a function (per-file headers do that, Step 3 part 2), and no
 *    PsyQ prototype is pulled in (prototypes with char/short parameters change call sites).
 */
#ifndef D1_STRUCTS_H
#define D1_STRUCTS_H

#define D1_CAT_(a, b) a##b
#define D1_CAT(a, b) D1_CAT_(a, b)
/* compile-time check: offsetof(T, f) == o */
#define OFS_ASSERT(T, f, o) typedef char D1_CAT(ofs_assert_, __LINE__)[((int)&((T *)0)->f == (o)) ? 1 : -1]
#define SIZE_ASSERT(T, n) typedef char D1_CAT(size_assert_, __LINE__)[(sizeof(T) == (n)) ? 1 : -1]

/* ---- gte base types (layout of PsyQ libgte.h) ---- */
#ifndef COMMON_H
typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { short m[3][3]; int t[3]; } MATRIX;
typedef struct { unsigned char r, g, b, cd; } CVECTOR;
#endif
typedef struct { int vx, vy, vz; } VECTOR_NOPAD;    /* D2 VECTOR_NOPAD */
typedef struct { short x, y; } SXY;
/* D2 MATRIX2: CompoundMatrix[] (draw.c, tile.c) - a MATRIX with a frame stamp in its padding, 0x20 bytes */
typedef struct { short m[3][3]; short computed; int t[3]; } MATRIX2;
SIZE_ASSERT(MATRIX2, 0x20);

/* ---- car_data[] (0x9B0A4): one car, 0x18C bytes. D_0009B230 = car_data[1]; 20 slots. ----
 * D1's record is not D2's _CAR_DATA (0x29C): the physics state (D2 st/hd) sits at 0x04-0x77 and the
 * draw position at 0xC8. */
typedef struct {
    unsigned char susCompression;
    unsigned char locked;
    unsigned char onGrass;          /* handling.c StepOneCar */
    unsigned char pad;
} WHEEL;
typedef struct {
    unsigned char id;               /* 0x000 slot/control id (objcoll: controlType; gamesnd: type) */
    unsigned char f1;               /* 0x001 */
    unsigned char model;            /* 0x002 index into car_cosmetics */
    unsigned char f3;               /* 0x003 */
    MATRIX where;                   /* 0x004 orientation + position (where.t = 0x18) */
    int fposition[3];               /* 0x024 D2 st.n.fposition */
    int orientation[4];             /* 0x030 quaternion */
    int linearVelocity[3];          /* 0x040 */
    int angularVelocity[3];         /* 0x04C */
    int acc[3];                     /* 0x058 */
    int f64;                        /* 0x064 */
    int aacc[3];                    /* 0x068 */
    int f74;                        /* 0x074 */
    WHEEL wheel[4];                 /* 0x078 */
    VECTOR shadowPoints[4];         /* 0x088 */
    VECTOR pos;                     /* 0x0C8 drawn position */
    VECTOR vel;                     /* 0x0D8 */
    SVECTOR angle;                  /* 0x0E8 */
    int fF0;                        /* 0x0F0 */
    int fF4;                        /* 0x0F4 */
    int direction;                  /* 0x0F8 heading, 0..4095 */
    int fFC;                        /* 0x0FC */
    int wheelspin;                  /* 0x100 */
    int wheel_speed;                /* 0x104 */
    int speed;                      /* 0x108 */
    int f10C;                       /* 0x10C */
    int f110;                       /* 0x110 */
    int damage[6];                  /* 0x114 per-zone damage (denting) */
    short f12C;                     /* 0x12C */
    unsigned char hndType;          /* 0x12E D2 name; 1 = handling type 1 */
    unsigned char hit;              /* 0x12F collision flag */
    unsigned char f130;             /* 0x130 */
    unsigned char f131;             /* 0x131 */
    unsigned char flash;            /* 0x132 */
    unsigned char f133;             /* 0x133 */
    SXY trails[4][4];               /* 0x134 light trails */
    short old_clock[4];             /* 0x174 */
    short totalDamage;              /* 0x17C */
    signed char autoBrake;          /* 0x17E handling.c (lb) */
    unsigned char f17F;             /* 0x17F */
    short f180;                     /* 0x180 */
    short f182;                     /* 0x182 */
    unsigned char gear;             /* 0x184 */
    unsigned char f185;             /* 0x185 */
    unsigned char pad186[2];
    unsigned int lastPad;           /* 0x188 handling.c func_0001FF24 */
} CAR_DATA;
SIZE_ASSERT(CAR_DATA, 0x18C);
OFS_ASSERT(CAR_DATA, where, 0x04);
OFS_ASSERT(CAR_DATA, where.t, 0x18);
OFS_ASSERT(CAR_DATA, fposition, 0x24);
OFS_ASSERT(CAR_DATA, linearVelocity, 0x40);
OFS_ASSERT(CAR_DATA, wheel, 0x78);
OFS_ASSERT(CAR_DATA, shadowPoints, 0x88);
OFS_ASSERT(CAR_DATA, pos, 0xC8);
OFS_ASSERT(CAR_DATA, vel, 0xD8);
OFS_ASSERT(CAR_DATA, angle, 0xE8);
OFS_ASSERT(CAR_DATA, direction, 0xF8);
OFS_ASSERT(CAR_DATA, wheelspin, 0x100);
OFS_ASSERT(CAR_DATA, speed, 0x108);
OFS_ASSERT(CAR_DATA, damage, 0x114);
OFS_ASSERT(CAR_DATA, f12C, 0x12C);
OFS_ASSERT(CAR_DATA, hit, 0x12F);
OFS_ASSERT(CAR_DATA, flash, 0x132);
OFS_ASSERT(CAR_DATA, trails, 0x134);
OFS_ASSERT(CAR_DATA, old_clock, 0x174);
OFS_ASSERT(CAR_DATA, totalDamage, 0x17C);
OFS_ASSERT(CAR_DATA, f180, 0x180);
OFS_ASSERT(CAR_DATA, gear, 0x184);
OFS_ASSERT(CAR_DATA, autoBrake, 0x17E);
OFS_ASSERT(CAR_DATA, lastPad, 0x188);

/* ---- civ/ped slot D_00098900[] (0x50; D_00098950 = D_00098900[1]): one per car slot ---- */
typedef struct {
    unsigned char id;               /* 0x00 */
    unsigned char f1;               /* 0x01 type/active */
    unsigned char f2;               /* 0x02 */
    unsigned char f3;               /* 0x03 */
    unsigned char f4;               /* 0x04 */
    unsigned char f5;               /* 0x05 */
    unsigned char f6;               /* 0x06 */
    unsigned char f7;               /* 0x07 */
    unsigned char f8;               /* 0x08 */
    unsigned char f9;               /* 0x09 */
    unsigned char fA;               /* 0x0A */
    unsigned char fB;               /* 0x0B */
    unsigned char fC;               /* 0x0C */
    unsigned char fD;               /* 0x0D */
    unsigned char fE;               /* 0x0E */
    unsigned char fF;               /* 0x0F */
    unsigned char f10;              /* 0x10 model */
    unsigned char f11;              /* 0x11 */
    short f12;                      /* 0x12 */
    unsigned char f14;              /* 0x14 */
    unsigned char f15;              /* 0x15 */
    unsigned char f16;              /* 0x16 */
    unsigned char f17;              /* 0x17 */
    int f18;                        /* 0x18 */
    int f1C;                        /* 0x1C */
    int f20;                        /* 0x20 */
    int f24;                        /* 0x24 */
    int f28;                        /* 0x28 */
    int f2C;                        /* 0x2C */
    int f30;                        /* 0x30 */
    int f34;                        /* 0x34 */
    int f38;                        /* 0x38 */
    int f3C;                        /* 0x3C */
    int f40;                        /* 0x40 */
    int f44;                        /* 0x44 */
    int x;                          /* 0x48 */
    int z;                          /* 0x4C */
} CIV_SLOT;
SIZE_ASSERT(CIV_SLOT, 0x50);
OFS_ASSERT(CIV_SLOT, f10, 0x10);
OFS_ASSERT(CIV_SLOT, f12, 0x12);
OFS_ASSERT(CIV_SLOT, f17, 0x17);
OFS_ASSERT(CIV_SLOT, f18, 0x18);
OFS_ASSERT(CIV_SLOT, x, 0x48);

/* ---- D_000C4690[2] (D_000C4E38 = D_000C4690[1]): pursuit/player record, 0x7A8 bytes ---- */
typedef struct {
    int f0;                         /* 0x000 */
    int f4, f8;
    int ang;                        /* 0x00C */
    int dist;                       /* 0x010 */
    int f14, f18;
    int f1C;                        /* 0x01C */
    int f20, f24;
    int speed;                      /* 0x028 */
    int f2C, f30, f34, f38;
    int timer3C;                    /* 0x03C */
    int f40;                        /* 0x040 */
    int timer;                      /* 0x044 */
    int t48, t4c;                   /* 0x048 */
    unsigned char pad50[0x75C - 0x50];
    int x, y, z;                    /* 0x75C */
    unsigned char pad768[0x78C - 0x768];
    int active;                     /* 0x78C */
    int f790, f794;
    int f798;                       /* 0x798 */
    int f79C;                       /* 0x79C */
    int f7A0;                       /* 0x7A0 */
    CAR_DATA *car;                  /* 0x7A4 */
} PURSUER;
SIZE_ASSERT(PURSUER, 0x7A8);
OFS_ASSERT(PURSUER, timer, 0x44);
OFS_ASSERT(PURSUER, x, 0x75C);
OFS_ASSERT(PURSUER, active, 0x78C);
OFS_ASSERT(PURSUER, car, 0x7A4);

/* ---- MODEL (D2 engine/mdl.h, same layout; offsets are relocated to pointers at load) ---- */
typedef struct {
    unsigned short shape_flags;     /* 0x00 */
    unsigned short flags2;          /* 0x02 */
    short instance_number;          /* 0x04 */
    unsigned char tri_verts;        /* 0x06 */
    unsigned char zBias;            /* 0x07 */
    short bounding_sphere;          /* 0x08 */
    unsigned short num_point_normals; /* 0x0A */
    unsigned short num_vertices;    /* 0x0C */
    unsigned short num_polys;       /* 0x0E */
    int vertices;                   /* 0x10 */
    int poly_block;                 /* 0x14 */
    int normals;                    /* 0x18 */
    int point_normals;              /* 0x1C */
    int collision_block;            /* 0x20 */
} MODEL;
SIZE_ASSERT(MODEL, 0x24);
OFS_ASSERT(MODEL, bounding_sphere, 0x08);
OFS_ASSERT(MODEL, num_polys, 0x0E);
OFS_ASSERT(MODEL, collision_block, 0x20);

/* ---- CELL_OBJECT (D2 engine/cell.h): a placed map object, 16 bytes ---- */
typedef struct {
    VECTOR_NOPAD pos;               /* 0x0 */
    unsigned char computed;         /* 0xC frame stamp (draw.c DrawMapPSX); D2 pad */
    unsigned char yang;             /* 0xD */
    unsigned short type;            /* 0xE model number */
} CELL_OBJECT;
SIZE_ASSERT(CELL_OBJECT, 0x10);
OFS_ASSERT(CELL_OBJECT, type, 0xE);

/* ---- draw buffer: D2 DB. `current` / `last` point at one. The DRAWENV/DISPENV that follow in D2
 * (and in D1's MPBuff) are not touched through DB by any TU yet, so they are not declared here: no TU
 * may index a DB array through this type until they are. ---- */
typedef struct {
    char *primptr;                  /* 0x0 */
    unsigned long *ot;              /* 0x4 */
    char *primtab;                  /* 0x8 */
    int id;                         /* 0xC */
} DB;
OFS_ASSERT(DB, primtab, 0x8);
OFS_ASSERT(DB, id, 0xC);

/* ---- PLAYBACKCAMERA (D2 replays.h, same layout): replay camera change, 0x24 bytes ---- */
typedef struct PLAYBACKCAMERA {
    VECTOR_NOPAD position;          /* 0x00 */
    SVECTOR angle;                  /* 0x0C */
    int FrameCnt;                   /* 0x14 */
    short CameraPosvy;              /* 0x18 */
    short gCameraDistance;          /* 0x1A */
    short gCameraMaxDistance;       /* 0x1C */
    short gCameraAngle;             /* 0x1E */
    unsigned char cameraview;       /* 0x20 */
    unsigned char next;             /* 0x21 */
    unsigned char prev;             /* 0x22 */
    unsigned char idx;              /* 0x23 */
} PLAYBACKCAMERA;
SIZE_ASSERT(PLAYBACKCAMERA, 0x24);
OFS_ASSERT(PLAYBACKCAMERA, FrameCnt, 0x14);
OFS_ASSERT(PLAYBACKCAMERA, cameraview, 0x20);

/* ---- texture records (D2 engine/tset.h) ---- */
typedef struct {
    unsigned short id;              /* 0x0 */
    unsigned short nameoffset;      /* 0x2 */
    unsigned char x, y, width, height; /* 0x4 */
} TEXINF;
SIZE_ASSERT(TEXINF, 8);
typedef struct { unsigned char u0, v0, u1, v1, u2, v2, u3, v3; } UV;
typedef struct {
    UV coords;                      /* 0x0 */
    unsigned short tpageid;         /* 0x8 */
    unsigned short clutid;          /* 0xA */
    unsigned char texture_number;   /* 0xC texture.c GetTextureDetails */
    unsigned char texture_page;     /* 0xD */
} TEXTURE_DETAILS;
OFS_ASSERT(TEXTURE_DETAILS, clutid, 0xA);
OFS_ASSERT(TEXTURE_DETAILS, texture_page, 0xD);

/* ---- menu item (pause menu, FRONTEND): 16 bytes, handlers + neighbour indices ---- */
typedef struct {
    void (*enter)();                /* 0x0 */
    void (*leave)();                /* 0x4 */
    int (*action)();                /* 0x8 */
    unsigned char left, right, up, down; /* 0xC */
} MENU_ITEM;
SIZE_ASSERT(MENU_ITEM, 16);

/* ---- D_000BFA7C[20]: one pedestrian, 0x58 bytes (civ_draw.c draws it, pedest.c moves it). D1's own record,
 * not D2's PEDESTRIAN (0x94); the D2 name is kept for the type only. ---- */
typedef struct {
    VECTOR_NOPAD pos;               /* 0x00 */
    VECTOR_NOPAD delta;             /* 0x0C target - pos (pedest.c func_000604F8) */
    SVECTOR dir;                    /* 0x18 vy = heading */
    SVECTOR vel;                    /* 0x20 */
    VECTOR target;                  /* 0x28 next cell centre (pedest.c func_0005FBE0) */
    void *bones;                    /* 0x38 civ_draw BONE ** */
    int flags;                      /* 0x3C */
    short f40;                      /* 0x40 head yaw (DrawCiv) / heading to the camera (pedest.c) */
    short slot;                     /* 0x42 own index (pedest.c free list D_000C0340) */
    short route;                    /* 0x44 chosen direction bit 1/2/4/8 (pedest.c func_0005FBE0) */
    short f46;                      /* 0x46 */
    unsigned char frame;            /* 0x48 */
    unsigned char f49;
    unsigned char f4A;
    unsigned char f4B;
    unsigned char *motion;          /* 0x4C civ_draw MOTION * */
    unsigned char speed;            /* 0x50 */
    signed char f51;                /* 0x51 */
    signed char f52;                /* 0x52 */
    unsigned char idx;              /* 0x53 */
    unsigned char f54;              /* 0x54 */
    unsigned char pal;              /* 0x55 */
    unsigned char type;             /* 0x56 model/state */
    unsigned char f57;
} PEDESTRIAN;
SIZE_ASSERT(PEDESTRIAN, 0x58);
OFS_ASSERT(PEDESTRIAN, dir, 0x18);
OFS_ASSERT(PEDESTRIAN, target, 0x28);
OFS_ASSERT(PEDESTRIAN, route, 0x44);
OFS_ASSERT(PEDESTRIAN, bones, 0x38);
OFS_ASSERT(PEDESTRIAN, flags, 0x3C);
OFS_ASSERT(PEDESTRIAN, frame, 0x48);
OFS_ASSERT(PEDESTRIAN, motion, 0x4C);
OFS_ASSERT(PEDESTRIAN, pal, 0x55);
OFS_ASSERT(PEDESTRIAN, type, 0x56);

/* ---- D_000C6F04: the mission (DMS script) state, 0x2E8 bytes (mission.c; also main.c, glaunch.c, pause.c,
 * draw.c, civ_ai.c). Names are mission.c's; D2's mission code differs too much to borrow its names. ---- */
typedef struct {
    int *ptr;                       /* 0x0 */
    short s4, s6, s8;               /* 0x4 */
    unsigned char a, b, c;          /* 0xA */
    unsigned char pad[3];
} MS_SOUND;                         /* 0x10 */
typedef struct {
    int x, y, z;                    /* 0x00 */
    int dir;                        /* 0x0C */
    int g, h, i;                    /* 0x10 */
    short s[6];                     /* 0x1C car damage[] */
    short s17C;                     /* 0x28 car totalDamage */
    short pad;
} MS_SAVE;                          /* 0x2C: player checkpoint (saved/cur) */
typedef struct {
    int x, y, z;                    /* 0x00 */
    int dir;                        /* 0x0C */
    short s[6];                     /* 0x10 */
    short s1C;                      /* 0x1C */
    unsigned char b1E, b1F;         /* 0x1E civ slot f10/f11 */
} MS_CARSAVE;                       /* 0x20 */
typedef struct { int f0, f4, f8, fC, f10, f14, f18, f1C, f20, f24; } MS_STATS;
typedef struct { int h, i, j, k; } MS_Q4;
typedef struct {
    unsigned int a;                 /* 0x00 */
    unsigned int b;                 /* 0x04 */
    int c;                          /* 0x08 */
    int d;                          /* 0x0C flags */
    VECTOR *e;                      /* 0x10 target position */
    short f;                        /* 0x14 */
    int *g;                         /* 0x18 */
    MS_Q4 q;                        /* 0x1C */
} MS_TARGET;                        /* 0x2C */
typedef struct {
    int *label[5];                  /* 0x000 */
    MS_SOUND snd[3];                /* 0x014 */
    int f44, f48, f4C;              /* 0x044 */
    int *f50;                       /* 0x050 */
    int f54;                        /* 0x054 */
    MS_SAVE saved;                  /* 0x058 */
    MS_SAVE cur;                    /* 0x084 */
    MS_CARSAVE d[1];                /* 0x0B0 */
    MS_STATS st;                    /* 0x0D0 */
    CAR_DATA *fF8;                  /* 0x0F8 */
    int *fFC;                       /* 0x0FC */
    int f100;                       /* 0x100 */
    int f104;                       /* 0x104 mission state (0xFF0 = over, 0x7FFF = passed) */
    int f108;
    int f10C;                       /* 0x10C end reason */
    int f110;
    int f114;
    int f118;
    int f11C;                       /* 0x11C flags */
    int f120;
    int *f124;
    int *f128;
    int f12C;                       /* 0x12C */
    MS_TARGET ent[6];               /* 0x130 */
    VECTOR tgt[6];                  /* 0x238 */
    int f298;                       /* 0x298 target count */
    int f29C;                       /* 0x29C */
    int f2A0;                       /* 0x2A0 */
    int x, y, z;                    /* 0x2A4 */
    int f2B0;
    int *end;                       /* 0x2B4 */
    int *pc;                        /* 0x2B8 script pointer */
    int f2BC;
    int f2C0;                       /* 0x2C0 */
    int f2C4;                       /* 0x2C4 */
    int f2C8;
    int f2CC;
    int f2D0;                       /* 0x2D0 */
    int f2D4;
    int f2D8;
    short f2DC;                     /* 0x2DC */
    unsigned char f2DE, f2DF, f2E0, f2E1, f2E2, f2E3;
    unsigned char f2E4;             /* 0x2E4 */
    unsigned char f2E5, f2E6, f2E7;
} MISSION;
SIZE_ASSERT(MS_SOUND, 0x10);
SIZE_ASSERT(MS_SAVE, 0x2C);
SIZE_ASSERT(MS_CARSAVE, 0x20);
SIZE_ASSERT(MS_TARGET, 0x2C);
SIZE_ASSERT(MISSION, 0x2E8);
OFS_ASSERT(MISSION, saved, 0x58);
OFS_ASSERT(MISSION, st, 0xD0);
OFS_ASSERT(MISSION, f104, 0x104);
OFS_ASSERT(MISSION, ent, 0x130);
OFS_ASSERT(MISSION, tgt, 0x238);
OFS_ASSERT(MISSION, pc, 0x2B8);
OFS_ASSERT(MISSION, f2DC, 0x2DC);
OFS_ASSERT(MISSION, f2E4, 0x2E4);

/* ---- PERCENTAGE_BAR (D2 overmap.h names; D1's is 0x1C): D_000BE774, D_000BE998, D_000BE9C4, D_000BE9E0
 * (overmap.c draws them; mission.c, glaunch.c, main.c fill them) ---- */
typedef struct {
    char *tag;                      /* 0x00 */
    short xpos, ypos;               /* 0x04 */
    short width, height;            /* 0x08 */
    short position, max;            /* 0x0C */
    void *pColourBand;              /* 0x10 */
    int flags;                      /* 0x14 */
    int active;                     /* 0x18 */
} PERCENTAGE_BAR;
SIZE_ASSERT(PERCENTAGE_BAR, 0x1C);
OFS_ASSERT(PERCENTAGE_BAR, position, 0xC);
OFS_ASSERT(PERCENTAGE_BAR, active, 0x18);

#endif
