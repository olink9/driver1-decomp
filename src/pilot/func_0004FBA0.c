typedef struct { unsigned addr:24; unsigned len:8; } TAG;
typedef struct { char *primptr; unsigned int *ot; } DB;
extern long long D_00097530;
#define current (*(DB **)&D_00097530)

#define b075_ldv3c(r0) __asm__ volatile ( \
    "lwc2 $0, 0(%0);lwc2 $1, 4(%0);lwc2 $2, 8(%0);lwc2 $3, 12(%0);lwc2 $4, 16(%0);lwc2 $5, 20(%0)" : : "r"(r0))
#define b075_rtpt() __asm__ volatile ("nop;nop;.word 0x4A280030")
#define b075_stsxy3c(r0) __asm__ volatile ( \
    "swc2 $12, 0(%0);swc2 $13, 4(%0);swc2 $14, 8(%0)" : : "r"(r0) : "memory")

typedef struct { short vx, vy, vz, pad; } SVEC;
typedef struct {
    char pad[0xC];
    unsigned short num_vertices;
    unsigned short num_polys;
    SVEC *vertices;
    unsigned char *polys;
} MODEL;

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    unsigned int xy0;
    unsigned int uv0;
    unsigned int xy1;
    unsigned int uv1;
    unsigned int xy2;
    unsigned int uv2;
    unsigned int xy3;
    unsigned int uv3;
} PFT4;

extern int D_0008AABC[][4];
extern int D_000974F0;
extern int D_000974DC;
extern int D_000974F4;
extern short D_000974F8;
extern short D_000BDD20[];
extern unsigned short D_000BDE46[];

void func_0004FBA0(MODEL *m, int idx)
{
    unsigned short *sxy16;
    int *sxy32;
    int *tex;
    SVEC *v;
    int *out;
    int i;
    int r, g, b;
    int tpage;
    PFT4 *prim;
    unsigned char *poly;
    int n;

    sxy16 = (unsigned short *)0x1F800044;
    sxy32 = (int *)0x1F800044;
    tex = D_0008AABC[idx];
    v = m->vertices;
    for (i = 0; i < m->num_vertices; i += 3) {
        b075_ldv3c(&v[i]);
        b075_rtpt();
        b075_stsxy3c(&((int *)0x1F800044)[i]);
    }
    r = D_000974F0 - 20;
    g = D_000974F4 - 20;
    b = D_000974DC - 10;
    tpage = D_000974F8 << 16;
    prim = (PFT4 *)current->primptr;
    poly = m->polys;
    if (r < 0) r = 0;
    if (g < 0) g = 0;
    if (b < 0) b = 0;
    for (i = 0; i < m->num_polys; poly += 20, i++, tex++) {
        if ((sxy16[poly[4] * 2] < 512 || sxy16[poly[5] * 2] < 512 || sxy16[poly[6] * 2] < 512 || sxy16[poly[7] * 2] < 512) &&
            (sxy16[poly[4] * 2 + 1] < 256 || sxy16[poly[5] * 2 + 1] < 256 || sxy16[poly[6] * 2 + 1] < 256 || sxy16[poly[7] * 2 + 1] < 256)) {
            n = *tex;
            ((unsigned char *)prim)[3] = 9;
            prim->code = 0x2C;
            prim->r0 = r;
            prim->g0 = g;
            prim->b0 = b;
            prim->xy0 = sxy32[poly[7]];
            prim->xy1 = sxy32[poly[4]];
            prim->xy2 = sxy32[poly[6]];
            prim->xy3 = sxy32[poly[5]];
            n += 8;
            prim->uv0 = D_000BDE46[n * 4] | (D_000BDD20[n] << 16);
            prim->uv1 = D_000BDE46[n * 4 - 1] | tpage;
            prim->uv2 = D_000BDE46[n * 4 - 2];
            *(unsigned int *)((char *)prim + 0x24) = D_000BDE46[n * 4 - 3];
            ((TAG *)prim)->addr = ((TAG *)&current->ot[0x107F])->addr;
            ((TAG *)&current->ot[0x107F])->addr = (unsigned int)prim;
            prim++;
        }
    }
    current->primptr = (char *)prim;
}
