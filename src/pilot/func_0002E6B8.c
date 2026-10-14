/* func_0002E6B8: like func_0002E230, but z-sorts the model by its average depth:
 * returns 0 if too far, else shifts the z values and draws the polygons. */
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { short m[3][3]; int t[3]; } MATRIX;
typedef struct { int vx, vy, vz; } VECTOR;

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
extern MATRIX D_000BBE84;
extern long long D_000972F0;
extern void MulMatrix0();
extern void func_00079D20();

#define gte_SetTransVector(r0) __asm__ volatile ( \
    "lw $12, 0(%0);" "lw $13, 4(%0);" "lw $14, 8(%0);" \
    "ctc2 $12, $5;" "ctc2 $13, $6;" "ctc2 $14, $7" : : "r"(r0) : "$12", "$13", "$14")
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

int func_0002E6B8(MODEL *model, MATRIX *rot, VECTOR *trans)
{
    char pad[0x20];
    MATRIX m;
    int z0;
    int z1;
    int z2;
    SVECTOR *verts;
    long *sxy;
    short *zbuf;
    unsigned char *poly;
    int i;
    int n;
    int sum;

    sxy = (long *)0x1F800044;
    zbuf = (short *)0x1F800240;
    sum = 0;
    gte_SetTransVector(trans);
    MulMatrix0(&D_000BBE84, rot, &m);
    func_00079D20(&m);
    verts = model->vertices;
    i = 0;
    for (n = model->num_vertices / 3; n > 0; ) {
        gte_ldv3(&verts[i], &verts[i + 1], &verts[i + 2]);
        gte_rtpt();
        zbuf[i - 3] = z0 >> 3;
        zbuf[i - 2] = z1 >> 3;
        zbuf[i - 1] = z2 >> 3;
        if (n != model->num_vertices / 3)
            sum += zbuf[i - 3] + zbuf[i - 2] + zbuf[i - 1];
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
        sum += zbuf[i - 3] + zbuf[i - 2] + zbuf[i - 1];
        break;
    case 1:
        gte_ldv0(&verts[i]);
        gte_rtps();
        zbuf[i - 3] = z0 >> 3;
        zbuf[i - 2] = z1 >> 3;
        zbuf[i - 1] = z2 >> 3;
        sum += zbuf[i - 3] + zbuf[i - 2] + zbuf[i - 1];
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
        sum += zbuf[i - 3] + zbuf[i - 2] + zbuf[i - 1];
        gte_stsxy(&sxy[i]);
        gte_stsz(&z0);
        gte_ldv0(&verts[i + 1]);
        gte_rtps();
        zbuf[i] = z0 >> 3;
        sum += zbuf[i];
        gte_stsxy(&sxy[i + 1]);
        gte_stsz(&z0);
        zbuf[i + 1] = z0 >> 3;
        sum += zbuf[i + 1];
        break;
    }
    sum /= model->num_vertices;
    if (sum > 0x12D8)
        return 0;
    if (sum > 0xDC4) {
        for (i = 0; i < model->num_vertices; i++)
            zbuf[i] += 0xDC4 - sum;
    }
    poly = model->polys;
    *(MODEL **)0x1F800018 = model;
    *(short **)0x1F80001C = zbuf;
    for (i = 0; i < model->num_polys; i++) {
        *(unsigned char **)0x1F80003C = poly;
        if (D_00089FFC[*poly])
            D_00089FFC[*poly](1);
        poly += D_00089F1C[*poly];
        (*(int *)&D_000972F0)++;
    }
    return 1;
}
