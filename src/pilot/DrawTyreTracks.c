/* DrawTyreTracks (Driver 2 name) */
#include "gte.h"

typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct { int vx, vy, vz; } VECTOR_NOPAD;
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { short m[3][3]; short pad; int t[3]; } MATRIX;

typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long current;
#define current (*(DB **)&current)

typedef struct {
    unsigned char type;
    unsigned char pad1, pad2;
    unsigned char surface;
    VECTOR_NOPAD p1, p2, p3, p4;
} TYRE_TRACK;

typedef struct {
    unsigned char u0, v0, u1, v1, u2, v2, u3, v3;
    unsigned short tpageid, clutid;
} TEXTURE_DETAILS;

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

typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;

extern MATRIX inv_camera_matrix;
extern VECTOR D_0008909C;
extern VECTOR camera_position;
extern int D_000A1CCC[4];
extern int D_000A5D7C[4];
extern TYRE_TRACK track_buffer[4][64];
extern TEXTURE_DETAILS D_000BBE44;

#define gte_SetTransVector(r0) __asm__ volatile ( \
    "lw    $12, 0( %0 );" "lw    $13, 4( %0 );" "lw    $14, 8( %0 );" \
    "ctc2  $12, $5;" "ctc2  $13, $6;" "ctc2  $14, $7" : : "r"(r0) : "$12", "$13", "$14")
#define gte_ldv3c(r0) __asm__ volatile ( \
    "lwc2  $0, 0( %0 );" "lwc2  $1, 4( %0 );" "lwc2  $2, 8( %0 );" \
    "lwc2  $3, 12( %0 );" "lwc2  $4, 16( %0 );" "lwc2  $5, 20( %0 )" : : "r"(r0))
#define gte_ldv0(r0) __asm__ volatile ("lwc2  $0, 0( %0 );" "lwc2  $1, 4( %0 )" : : "r"(r0))
#define gte_rtpt() __asm__ volatile ("nop;" "nop;" "rtpt")
#define gte_rtps() __asm__ volatile ("nop;" "nop;" "rtps")
#define gte_stsz(r0) __asm__ volatile ("swc2  $19, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy(r0) __asm__ volatile ("swc2  $14, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy3(r0, r1, r2) __asm__ volatile ( \
    "swc2  $12, 0( %0 );" "swc2  $13, 0( %1 );" "swc2  $14, 0( %2 )" : : "r"(r0), "r"(r1), "r"(r2) : "memory")

void DrawTyreTracks(void)
{
    VECTOR p[4];
    SVECTOR ps[4];
    int z;
    TYRE_TRACK *tt_p;
    int index, loop;
    int wheel_loop;
    unsigned char last_duff;
    POLY_FT4 *poly;
    POLY_FT4 *lasttyre;
    int t;

    gte_SetRotMatrix(&inv_camera_matrix);
    gte_SetTransVector(&D_0008909C);

    for (wheel_loop = 0; wheel_loop < 4; wheel_loop++) {
        if (D_000A1CCC[wheel_loop] == 0)
            continue;
        lasttyre = 0;
        last_duff = 1;
        index = D_000A5D7C[wheel_loop];

        for (loop = 0; loop < D_000A1CCC[wheel_loop]; loop++) {
            tt_p = &track_buffer[wheel_loop][index];
            index++;
            if (index == 64)
                index = 0;

            if (lasttyre == 0 || tt_p->type == 0 || last_duff) {
                last_duff = 1;
                t = tt_p->p1.vx - camera_position.vx;
                if (t < -9000 || t > 9000)
                    continue;
                ps[0].vx = t;
                t = tt_p->p1.vz - camera_position.vz;
                if (t < -9000 || t > 9000)
                    continue;
                ps[0].vz = t;
                ps[0].vy = -tt_p->p1.vy - camera_position.vy;
                ps[2].vx = tt_p->p3.vx - camera_position.vx;
                ps[2].vz = tt_p->p3.vz - camera_position.vz;
                ps[2].vy = -tt_p->p3.vy - camera_position.vy;
                ps[1].vx = tt_p->p2.vx - camera_position.vx;
                ps[1].vy = -tt_p->p2.vy - camera_position.vy;
                ps[1].vz = tt_p->p2.vz - camera_position.vz;
                poly = (POLY_FT4 *)current->primptr;
                gte_ldv3c(&ps[0]);
                gte_rtpt();
                ps[3].vx = tt_p->p4.vx - camera_position.vx;
                ps[3].vy = -tt_p->p4.vy - camera_position.vy;
                ps[3].vz = tt_p->p4.vz - camera_position.vz;
                gte_stsxy3(&poly->x0, &poly->x1, &poly->x2);
                gte_stsz(&z);
                gte_ldv0(&ps[3]);
                gte_rtps();
                gte_stsxy(&poly->x3);
                if (z <= 50)
                    continue;
                last_duff = 0;
            } else {
                poly = (POLY_FT4 *)current->primptr;
                last_duff = 1;
                t = tt_p->p3.vx - camera_position.vx;
                if (t < -9000 || t > 9000)
                    continue;
                ps[2].vx = t;
                t = tt_p->p3.vz - camera_position.vz;
                if (t < -9000 || t > 9000)
                    continue;
                ps[2].vz = t;
                ps[2].vy = -tt_p->p3.vy - camera_position.vy;
                gte_ldv0(&ps[2]);
                gte_rtps();
                ps[3].vx = tt_p->p4.vx - camera_position.vx;
                ps[3].vy = -tt_p->p4.vy - camera_position.vy;
                ps[3].vz = tt_p->p4.vz - camera_position.vz;
                gte_stsxy(&poly->x2);
                gte_stsz(&z);
                gte_ldv0(&ps[3]);
                gte_rtps();
                if (z <= 50)
                    continue;
                *(unsigned int *)&poly->x0 = *(unsigned int *)&lasttyre->x2;
                *(unsigned int *)&poly->x1 = *(unsigned int *)&lasttyre->x3;
                last_duff = 0;
                gte_stsxy(&poly->x3);
            }

            ((unsigned char *)poly)[3] = 9;
            poly->code = 0x2C;

            if (tt_p->surface == 1) {
                poly->r0 = 26;
                poly->g0 = 26;
                poly->b0 = 26;
            } else {
                poly->r0 = 17;
                poly->g0 = 35;
                poly->b0 = 35;
            }

            poly->u0 = D_000BBE44.u0;
            poly->v0 = D_000BBE44.v0;
            poly->u1 = D_000BBE44.u1;
            poly->v1 = D_000BBE44.v1;
            poly->u2 = D_000BBE44.u2;
            poly->v2 = D_000BBE44.v2;
            poly->u3 = D_000BBE44.u3;
            poly->v3 = D_000BBE44.v3;
            *(unsigned short *)((char *)poly + 0x16) = D_000BBE44.tpageid | 0x40;
            *(unsigned short *)((char *)poly + 0xE) = D_000BBE44.clutid;
            poly->code |= 2;

            ((P_TAG *)poly)->addr = ((P_TAG *)&current->ot[z >> 3])->addr;
            ((P_TAG *)&current->ot[z >> 3])->addr = (unsigned int)poly;
            current->primptr += 0x28;

            lasttyre = poly;
        }
    }
}
