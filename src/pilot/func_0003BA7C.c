#include "common.h"

#define gte_ldv0(r0) __asm__ volatile ( \
    "lwc2  $0, 0( %0 );" "lwc2  $1, 4( %0 )" : : "r"(r0))
#define gte_rtv0() __asm__ volatile ( \
    "nop;" "nop;" "mvmva 1, 0, 0, 3, 0")
#define gte_stsv(r0) __asm__ volatile ( \
    "mfc2  $12, $9;" "mfc2  $13, $10;" "mfc2  $14, $11;" \
    "sh    $12, 0( %0 );" "sh    $13, 2( %0 );" "sh    $14, 4( %0 )" : : "r"(r0) : "$12", "$13", "$14", "memory")

typedef struct { int f0; MATRIX mat; char pad1[0xC8 - 4 - sizeof(MATRIX)]; int xpos; int pad2; int zpos; char pad3[0x18C - 0xD4]; } CAR;
typedef struct { char pad[0xA]; unsigned short nverts; char pad2[0x10]; SVECTOR *verts; } MODEL;

extern CAR D_0009B0A4[];
extern MODEL *D_000A1AB0[];
extern void func_0004EB10(); /* InvertMatrix */
extern void func_00079D20(); /* SetRotMatrix */

void func_0003BA7C(int car, unsigned char *out)
{
    SVECTOR v;
    int x, z;
    SVECTOR *sv;
    unsigned char *p;
    int i;

    x = (D_0009B0A4[car].xpos >> 8) & 0x3F;
    z = -(D_0009B0A4[car].zpos >> 8) & 0x3F;
    sv = D_000A1AB0[0]->verts;
    func_0004EB10(&D_0009B0A4[car].mat, (MATRIX *)0x1F800344);
    func_00079D20((MATRIX *)0x1F800344);
    for (i = 0; i < D_000A1AB0[0]->nverts; i++) {
        gte_ldv0(&sv[i]);
        gte_rtv0();
        gte_stsv(&v);
        out[i * 2] = (-v.vx + 4096) / 128 + x;
        out[i * 2 + 1] = (-v.vz + 4096) / 128 + z;
    }
    *(unsigned char **)0x1F800038 = out;
}
