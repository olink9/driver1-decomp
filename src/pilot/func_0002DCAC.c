/* func_0002DCAC: transform a model's vertices into scratchpad (screen XY + z/8), then
 * draw its polygons through the per-type function table. */
typedef struct { short vx, vy, vz, pad; } SVECTOR;

typedef struct {
    char pad0[0xC];
    unsigned short num_vertices;   /* 0x0C */
    unsigned short num_polys;      /* 0x0E */
    SVECTOR *vertices;             /* 0x10 */
    unsigned char *polys;          /* 0x14 */
} MODEL;

typedef void (*POLYFUNC)(int);
extern POLYFUNC D_00089FFC[];
extern int D_00089F1C[];

#define gte_ldv0(r0) __asm__ volatile ( \
    "lwc2 $0, 0(%0);" "lwc2 $1, 4(%0)" : : "r"(r0))
#define gte_ldv3(r0, r1, r2) __asm__ volatile ( \
    "lwc2 $0, 0(%0);" "lwc2 $1, 4(%0);" "lwc2 $2, 0(%1);" "lwc2 $3, 4(%1);" \
    "lwc2 $4, 0(%2);" "lwc2 $5, 4(%2)" : : "r"(r0), "r"(r1), "r"(r2))
#define gte_rtpt() __asm__ volatile ("nop;" "nop;" "rtpt")
#define gte_rtps() __asm__ volatile ("nop;" "nop;" "rtps")
#define gte_stsxy3(r0, r1, r2) __asm__ volatile ( \
    "swc2 $12, 0(%0);" "swc2 $13, 0(%1);" "swc2 $14, 0(%2)" : : "r"(r0), "r"(r1), "r"(r2) : "memory")
#define gte_stsz3(r0, r1, r2) __asm__ volatile ( \
    "swc2 $17, 0(%0);" "swc2 $18, 0(%1);" "swc2 $19, 0(%2)" : : "r"(r0), "r"(r1), "r"(r2) : "memory")
#define gte_stsxy(r0) __asm__ volatile ("swc2 $14, 0(%0)" : : "r"(r0) : "memory")
#define gte_stsz(r0) __asm__ volatile ("swc2 $19, 0(%0)" : : "r"(r0) : "memory")

void func_0002DCAC(MODEL *model)
{
    char pad[0x60];
    int z0;
    int z1;
    int z2;
    SVECTOR *verts;
    long *sxy;
    short *zbuf;
    unsigned char *poly;
    int i;
    int n;
    int type;

    sxy = (long *)0x1F800044;
    zbuf = (short *)0x1F800240;
    verts = model->vertices;
    i = 0;
    for (n = model->num_vertices / 3; n > 0; ) {
        gte_ldv3(&verts[i], &verts[i + 1], &verts[i + 2]);
        gte_rtpt();
        zbuf[i - 3] = z0 >> 3;
        zbuf[i - 2] = z1 >> 3;
        zbuf[i - 1] = z2 >> 3;
        gte_stsxy3(&sxy[i], &sxy[i + 1], &sxy[i + 2]);
        gte_stsz3(&z0, &z1, &z2);
        n--;
        i += 3;
    }
    switch (model->num_vertices - i) {
    case 0:
        zbuf[i - 3] = z0 >> 3;
        zbuf[i - 2] = z1 >> 3;
        zbuf[i - 1] = z2 >> 3;
        break;
    case 1:
        gte_ldv0(&verts[i]);
        gte_rtps();
        zbuf[i - 3] = z0 >> 3;
        zbuf[i - 2] = z1 >> 3;
        zbuf[i - 1] = z2 >> 3;
        gte_stsxy(&sxy[i]);
        gte_stsz(&z0);
        zbuf[i] = z0 >> 3;
        break;
    case 2:
        gte_ldv0(&verts[i]);
        gte_rtps();
        zbuf[i - 3] = z0 >> 3;
        zbuf[i - 2] = z1 >> 3;
        zbuf[i - 1] = z2 >> 3;
        gte_stsxy(&sxy[i]);
        gte_stsz(&z0);
        gte_ldv0(&verts[i + 1]);
        gte_rtps();
        zbuf[i] = z0 >> 3;
        gte_stsxy(&sxy[i + 1]);
        gte_stsz(&z0);
        zbuf[i + 1] = z0 >> 3;
        break;
    }
    poly = model->polys;
    *(MODEL **)0x1F800018 = model;
    *(short **)0x1F80001C = zbuf;
    for (i = 0; i < model->num_polys; i++) {
        type = *poly & 0x1F;
        *(unsigned char **)0x1F80003C = poly;
        D_00089FFC[type](0);
        poly += D_00089F1C[type];
    }
}
