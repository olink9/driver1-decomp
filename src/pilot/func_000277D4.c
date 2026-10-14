/* Sibling of func_00027620 (subdivided quad + UVs, then RotTransPers). */
typedef struct { short x, y, z, pad; } SV;
extern SV D_000A1B00[];
extern unsigned int D_00089044[];
extern int *D_0009693C;
extern unsigned char *D_00096940;
extern short *D_00096944;

#define B57_LDV3(p) __asm__ volatile ( \
    "lwc2 $0, 0(%0);" "lwc2 $1, 4(%0);" "lwc2 $2, 8(%0);" "lwc2 $3, 12(%0);" \
    "lwc2 $4, 16(%0);" "lwc2 $5, 20(%0);" "nop;" "nop;" "rtpt" : : "r"(p))
#define B57_LDV1(p) __asm__ volatile ( \
    "lwc2 $0, 0(%0);" "lwc2 $1, 4(%0);" "nop;" "nop;" "rtps" : : "r"(p))
#define B57_ST3(p) __asm__ volatile ( \
    "swc2 $12, 0(%0);" "swc2 $13, 4(%0);" "swc2 $14, 8(%0)" : : "r"(p) : "memory")
#define B57_STSZ(p) __asm__ volatile ( \
    "swc2 $17, 0(%0);" "swc2 $18, 4(%0);" "swc2 $19, 8(%0)" : : "r"(p) : "memory")
#define B57_ST1(p) __asm__ volatile ("swc2 $14, 0(%0)" : : "r"(p) : "memory")
#define B57_STSZ1(p) __asm__ volatile ("swc2 $19, 0(%0)" : : "r"(p) : "memory")

void func_000277D4(unsigned int verts, unsigned int uv, unsigned int uv2, SV *v)
{
    short step[3];
    char pad[2];
    unsigned short duv[2];
    int sz[3];
    unsigned int du, dv;
    int i, j, k, n;
    int bits;
    short x, y, z;
    short ey, ez;
    int ex, sx;
    unsigned int u, w;
    SV *s;
    SV *e;
    SV *p;
    SV *q;

    for (i = 0; i < 2; i++) {
        u = (uv & 0xFF) << 4;
        w = (uv & 0xFF00) >> 4;
        s = (SV *)(((verts & 0xFF) << 3) + (int)v);
        e = (SV *)(((verts >> 8 & 0xFF) << 3) + (int)v);
        du = ((((uv >> 16) & 0xFF) << 4) - u) >> 2;
        dv = ((((uv >> 16) & 0xFF00) >> 4) - w) >> 2;
        bits = D_00089044[i];
        y = s->y;
        z = s->z;
        ey = e->y;
        ez = e->z;
        ex = e->x;
        x = sx = s->x;
        duv[0] = du;
        duv[1] = dv;
        step[0] = (ex - sx) >> 2;
        step[1] = (ey - y) >> 2;
        step[2] = (ez - z) >> 2;
        for (j = 0; j < 5; j++) {
            n = bits & 0x1F;
            p = &D_000A1B00[n];
            p->x = x;
            p->y = y;
            p->z = z;
            bits >>= 5;
            D_00096940[n * 2] = u >> 4;
            D_00096940[n * 2 + 1] = w >> 4;
            x += step[0];
            y += step[1];
            z += step[2];
            u += duv[0];
            w += duv[1];
        }
        verts >>= 16;
        uv = uv2;
    }
    for (k = 0; k < 9; k += 3) {
        B57_LDV3(&D_000A1B00[k]);
        B57_ST3(D_0009693C + k);
        B57_STSZ(sz);
        D_00096944[k] = sz[0];
        D_00096944[k + 1] = sz[1];
        D_00096944[k + 2] = sz[2];
    }
    B57_LDV1(&D_000A1B00[k]);
    B57_ST1(D_0009693C + k);
    B57_STSZ1(sz);
    D_00096944[k] = sz[0];
}
