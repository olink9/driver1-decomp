/* DrawExplosion (Driver 2 name) */
#include "gte.h"

typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { short m[3][3]; short pad; int t[3]; } MATRIX;

typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long D_00097530;
#define current (*(DB **)&D_00097530)

typedef struct {
    unsigned short u0, u1, u2, u3;
    unsigned short tpageid, clutid;
} TEXTURE_DETAILS;

typedef struct {
    unsigned int tag;
    unsigned int rgb;
    short x0, y0;
    unsigned int u0;
    short x1, y1;
    unsigned int u1;
    short x2, y2;
    unsigned int u2;
    short x3, y3;
    unsigned int u3;
} POLY_FT4;

typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;

extern TEXTURE_DETAILS D_000B91B0;
extern int D_00096E74;
extern long long D_000970D8;
#define CameraCnt (*(int *)&D_000970D8)
extern VECTOR D_000C6CD8;
extern VECTOR D_000C6CE8;
extern VECTOR D_0008A8F0;
extern MATRIX D_0008BCC4;
extern MATRIX D_000BBE84;
extern SVECTOR D_000C6CF8[];
extern short D_0008EAA8[];
extern short D_0008EAAA[];

extern void func_00066CCC();
extern void func_0002C564();
extern void MulMatrix0();

#define gte_SetTransVector(r0) __asm__ volatile ( \
    "lw    $12, 0( %0 );" "lw    $13, 4( %0 );" "lw    $14, 8( %0 );" \
    "ctc2  $12, $5;" "ctc2  $13, $6;" "ctc2  $14, $7" : : "r"(r0) : "$12", "$13", "$14")
#define gte_ldv3c(r0) __asm__ volatile ( \
    "lwc2  $0, 0( %0 );" "lwc2  $1, 4( %0 );" "lwc2  $2, 8( %0 );" \
    "lwc2  $3, 12( %0 );" "lwc2  $4, 16( %0 );" "lwc2  $5, 20( %0 )" : : "r"(r0))
#define gte_rtpt() __asm__ volatile ("nop;" "nop;" "rtpt")
#define gte_stsxy(r0) __asm__ volatile ("swc2  $14, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy0(r0) __asm__ volatile ("swc2  $12, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy3(r0, r1, r2) __asm__ volatile ( \
    "swc2  $12, 0( %0 );" "swc2  $13, 0( %1 );" "swc2  $14, 0( %2 )" : : "r"(r0), "r"(r1), "r"(r2) : "memory")

void func_00066FF0(int time)
{
    int i, j;
    POLY_FT4 *poly;
    SVECTOR *src;
    VECTOR v;
    int rgb;
    int z;
    int sf, sf1, sf2;
    unsigned int u0, u1, u2, u3;
    int transparency, red;

    u0 = D_000B91B0.u0;
    u1 = D_000B91B0.u1;
    u2 = D_000B91B0.u2 - 0x800;
    u3 = D_000B91B0.u3 - 0x800;

    if (D_00096E74 == 0) {
        func_00066CCC();
        D_00096E74 = 1;
    }

    u0 += 0x200;
    u1 += 0x200;
    u0 |= D_000B91B0.clutid << 16;
    u1 |= (D_000B91B0.tpageid | 0x20) << 16;

    transparency = 255 - (time >> 4);
    rgb = ((255 - transparency) * (transparency >> 2) + transparency * (transparency >> 1) >> 8 |
           transparency * transparency >> 10 << 8) << 8 |
          transparency >> 1 | 0x2e000000;

    v.vx = (D_000C6CD8.vx + D_000C6CE8.vx) / 2 - D_0008A8F0.vx;
    v.vy = (D_000C6CD8.vy + D_000C6CE8.vy) / 2 - D_0008A8F0.vy;
    v.vz = (D_000C6CD8.vz + D_000C6CE8.vz) / 2 - D_0008A8F0.vz;

    func_0002C564(&v);
    gte_SetTransVector(&v);

    for (i = 0; i < 2; i++) {
        MATRIX workmatrix;
        int s, c;

        sf1 = ((time * (14240 - time) + 0x800) >> 12) + 12;
        sf = CameraCnt * (64 - i * 90);

        c = (sf1 * D_0008EAAA[(sf & 0xfff) * 2] + 0x800) >> 12;
        s = (sf1 * D_0008EAA8[(sf & 0xfff) * 2] + 0x800) >> 12;
        D_0008BCC4.m[1][1] = sf1;
        D_0008BCC4.m[2][2] = D_0008BCC4.m[0][0] = c;
        D_0008BCC4.m[0][2] = -(D_0008BCC4.m[2][0] = s);

        MulMatrix0(&D_000BBE84, &D_0008BCC4, &workmatrix);
        gte_SetRotMatrix(&workmatrix);

        src = D_000C6CF8;

        for (j = 0; j < 12; j++) {
            poly = (POLY_FT4 *)current->primptr;

            gte_ldv3c(src);
            src += 3;
            gte_rtpt();

            poly[0].rgb = rgb;
            poly[1].rgb = rgb;

            gte_stsxy3(&poly[0].x0, &poly[0].x1, &poly[0].x2);
            gte_stsxy(&poly[1].x0);
            gte_stsz(&z);

            gte_ldv3c(src);
            src += 1;
            gte_rtpt();

            if ((j & 3) == 3)
                src += 2;

            if (z > 32) {
                poly[0].u0 = u0;
                poly[0].u1 = u1;
                poly[0].u2 = u2;
                poly[0].u3 = u3;
                poly[1].u0 = u0;
                poly[1].u1 = u1;
                poly[1].u2 = u2;
                poly[1].u3 = u3;

                ((unsigned char *)&poly[0])[3] = 9;
                ((P_TAG *)&poly[0])->addr = ((P_TAG *)&current->ot[z >> 3])->addr;
                ((P_TAG *)&current->ot[z >> 3])->addr = (unsigned int)&poly[0];
                ((unsigned char *)&poly[1])[3] = 9;
                ((P_TAG *)&poly[1])->addr = ((P_TAG *)&current->ot[z >> 3])->addr;
                ((P_TAG *)&current->ot[z >> 3])->addr = (unsigned int)&poly[1];

                gte_stsxy3(&poly[1].x1, &poly[1].x2, &poly[1].x3);
                gte_stsxy0(&poly[0].x3);

                current->primptr += sizeof(POLY_FT4) * 2;
            }
        }
    }

    transparency = 255 - (time >> 4);
    red = transparency >> 1;
    rgb = (red + ((255 - transparency) * (transparency >> 2) + transparency * red >> 8) >> 1 |
           red + (transparency * transparency >> 10) >> 1 << 8) << 8 |
          red | 0x2e000000;

    for (i = 0; i < 2; i++) {
        MATRIX workmatrix;
        int s, c;

        sf2 = ((time * (26240 - time) + 0x800) >> 12) + 12;
        sf = CameraCnt * (64 - i * 90);

        c = (sf2 * D_0008EAAA[(sf & 0xfff) * 2] + 0x800) >> 12;
        s = (sf2 * D_0008EAA8[(sf & 0xfff) * 2] + 0x800) >> 12;
        D_0008BCC4.m[2][2] = D_0008BCC4.m[0][0] = c;
        D_0008BCC4.m[0][2] = -(D_0008BCC4.m[2][0] = s);
        D_0008BCC4.m[1][1] = sf2 / 16;

        MulMatrix0(&D_000BBE84, &D_0008BCC4, &workmatrix);
        gte_SetRotMatrix(&workmatrix);

        src = D_000C6CF8;

        for (j = 0; j < 8; j++) {
            poly = (POLY_FT4 *)current->primptr;

            gte_ldv3c(src);
            src += 3;
            gte_rtpt();

            poly[0].rgb = rgb;
            poly[1].rgb = rgb;

            gte_stsxy3(&poly[0].x0, &poly[0].x1, &poly[0].x2);
            gte_stsxy(&poly[1].x0);
            gte_stsz(&z);

            gte_ldv3c(src);
            src += 1;
            gte_rtpt();

            if ((j & 3) == 3)
                src += 2;

            if (z > 32) {
                poly[0].u0 = u0;
                poly[0].u1 = u1;
                poly[0].u2 = u2;
                poly[0].u3 = u3;
                poly[1].u0 = u0;
                poly[1].u1 = u1;
                poly[1].u2 = u2;
                poly[1].u3 = u3;

                ((unsigned char *)&poly[0])[3] = 9;
                ((P_TAG *)&poly[0])->addr = ((P_TAG *)&current->ot[z >> 3])->addr;
                ((P_TAG *)&current->ot[z >> 3])->addr = (unsigned int)&poly[0];
                ((unsigned char *)&poly[1])[3] = 9;
                ((P_TAG *)&poly[1])->addr = ((P_TAG *)&current->ot[z >> 3])->addr;
                ((P_TAG *)&current->ot[z >> 3])->addr = (unsigned int)&poly[1];

                gte_stsxy3(&poly[1].x1, &poly[1].x2, &poly[1].x3);
                gte_stsxy0(&poly[0].x3);

                current->primptr += sizeof(POLY_FT4) * 2;
            }
        }
    }
}
