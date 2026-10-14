/* Sibling of func_000277D4: 4x4-subdivided quad (5x5 grid + UVs), then RotTransPers. */
typedef struct { short x, y, z, pad; } SV;
extern SV D_000A1B00[];
extern unsigned int D_00089030[];
extern int *D_0009693C;
extern unsigned char *D_00096940;
extern short *D_00096944;

#define B76_LDV3(p) __asm__ volatile ( \
    "lwc2 $0, 0(%0);" "lwc2 $1, 4(%0);" "lwc2 $2, 8(%0);" "lwc2 $3, 12(%0);" \
    "lwc2 $4, 16(%0);" "lwc2 $5, 20(%0);" "nop;" "nop;" "rtpt" : : "r"(p))
#define B76_LDV1(p) __asm__ volatile ( \
    "lwc2 $0, 0(%0);" "lwc2 $1, 4(%0);" "nop;" "nop;" "rtps" : : "r"(p))
#define B76_ST3(p) __asm__ volatile ( \
    "swc2 $12, 0(%0);" "swc2 $13, 4(%0);" "swc2 $14, 8(%0)" : : "r"(p) : "memory")
#define B76_STSZ(p) __asm__ volatile ( \
    "swc2 $17, 0(%0);" "swc2 $18, 4(%0);" "swc2 $19, 8(%0)" : : "r"(p) : "memory")
#define B76_ST1(p) __asm__ volatile ("swc2 $14, 0(%0)" : : "r"(p) : "memory")
#define B76_STSZ1(p) __asm__ volatile ("swc2 $19, 0(%0)" : : "r"(p) : "memory")

void func_00027A4C(unsigned int verts, unsigned int uv, unsigned int uv2, SV *v)
{
    short step[4];
    short estep[4];
    unsigned short duv[4];
    char pad[8];
    int sz[3];
    int i, j, k;
    unsigned int bits;
    unsigned short u, w;
    unsigned short ru, rw;
    unsigned short x, y, z;
    unsigned short rx, ry, rz;
    unsigned int *bp;
    SV *s, *c, *e, *p, *q;

    u = (uv & 0xFF) << 4;
    w = (uv & 0xFF00) >> 4;
    i = 0;
    s = (SV *)(((verts & 0xFF) << 3) + (int)v);
    x = s->x;
    y = s->y;
    z = s->z;
    duv[0] = (((uv >> 16 & 0xFF) << 4) - u) >> 2;
    duv[1] = (((uv >> 16 & 0xFF00) >> 4) - w) >> 2;
    uv2 >>= 16;
    duv[2] = (((uv2 & 0xFF) << 4) - u) >> 2;
    duv[3] = (((uv2 & 0xFF00) >> 4) - w) >> 2;
    c = (SV *)(((verts >> 8 & 0xFF) << 3) + (int)v);
    e = (SV *)(((verts >> 24) << 3) + (int)v);
    step[0] = (c->x - (short)x) >> 2;
    step[1] = (c->y - (short)y) >> 2;
    step[2] = (c->z - (short)z) >> 2;
    estep[0] = (e->x - (short)x) >> 2;
    estep[1] = (e->y - (short)y) >> 2;
    estep[2] = (e->z - (short)z) >> 2;
    for (k = 0; i < 5; i++) {
        rx = x;
        ry = y;
        rz = z;
        ru = u;
        rw = w;
        bits = D_00089030[i];
        for (j = 0; j < 5; j++) {
            int n = bits & 0x1F;
            p = &D_000A1B00[n];
            p->x = rx;
            p->y = ry;
            p->z = rz;
            bits = (int)bits >> 5;
            D_00096940[n * 2] = ru >> 4;
            D_00096940[n * 2 + 1] = rw >> 4;
            rx += step[0];
            ry += step[1];
            rz += step[2];
            ru += duv[0];
            rw += duv[1];
        }
        x += estep[0];
        y += estep[1];
        z += estep[2];
        u += duv[2];
        w += duv[3];
        if (i != 0) {
            if (i >= 2) {
                B76_ST3(D_0009693C + k);
                B76_STSZ(sz);
                D_00096944[k] = sz[0];
                D_00096944[k + 1] = sz[1];
                D_00096944[k + 2] = sz[2];
                k += 3;
            }
            B76_LDV3(&D_000A1B00[k]);
        }
    }
    B76_ST3(D_0009693C + k);
    for (; k < 24; k += 3) {
        B76_LDV3(&D_000A1B00[k]);
        B76_ST3(D_0009693C + k);
        B76_STSZ(sz);
        D_00096944[k] = sz[0];
        D_00096944[k + 1] = sz[1];
        D_00096944[k + 2] = sz[2];
    }
    B76_LDV1(&D_000A1B00[k]);
    B76_ST1(D_0009693C + k);
    B76_STSZ1(sz);
    D_00096944[k] = sz[0];
}
