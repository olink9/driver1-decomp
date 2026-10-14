/* Sibling of func_00027A4C: 4x4-subdivided bilinear quad (5x5 grid + UVs), then RotTransPers. */
typedef struct { short x, y, z, pad; } SV;
extern SV D_000A1B00[];
extern unsigned int D_00089030[];
extern int *D_0009693C;
extern unsigned char *D_00096940;

#define B85_LDV3(p) __asm__ volatile ( \
    "lwc2 $0, 0(%0);" "lwc2 $1, 4(%0);" "lwc2 $2, 8(%0);" "lwc2 $3, 12(%0);" \
    "lwc2 $4, 16(%0);" "lwc2 $5, 20(%0);" "nop;" "nop;" "rtpt" : : "r"(p))
#define B85_LDV1(p) __asm__ volatile ( \
    "lwc2 $0, 0(%0);" "lwc2 $1, 4(%0);" "nop;" "nop;" "rtps" : : "r"(p))
#define B85_ST3(p) __asm__ volatile ( \
    "swc2 $12, 0(%0);" "swc2 $13, 4(%0);" "swc2 $14, 8(%0)" : : "r"(p) : "memory")
#define B85_ST1(p) __asm__ volatile ("swc2 $14, 0(%0)" : : "r"(p) : "memory")

void func_00029448(unsigned int verts, unsigned int uv, unsigned int uv2, SV *v)
{
    short step[4];
    short estep[4];
    short cstep[4];
    short dstep[4];
    short duv[4];
    short duv2[4];
    short duv3[4];
    int i, j, k;
    unsigned int bits;
    unsigned short u, w;
    unsigned short ru, rw;
    unsigned short x, y, z;
    unsigned short rx, ry, rz;
    SV *s, *c, *e, *f, *g;
    SV *pp;
    unsigned int *bp;

    u = (uv & 0xFF) << 4;
    w = (uv & 0xFF00) >> 4;
    s = (SV *)(((verts & 0xFF) << 3) + (int)v);
    x = s->x;
    y = s->y;
    z = s->z;
    duv[0] = (((uv >> 16 & 0xFF) << 4) - u) >> 2;
    duv[1] = (((uv >> 16 & 0xFF00) >> 4) - w) >> 2;
    duv[2] = (((uv2 >> 16 & 0xFF) << 4) - u) >> 2;
    duv[3] = (((uv2 >> 16 & 0xFF00) >> 4) - w) >> 2;
    duv2[0] = (((uv2 & 0xFF) << 4) - ((uv2 >> 16 & 0xFF) << 4)) >> 2;
    duv2[1] = (((uv2 & 0xFF00) >> 4) - ((uv2 >> 16 & 0xFF00) >> 4)) >> 2;
    c = (SV *)(((verts >> 8 & 0xFF) << 3) + (int)v);
    e = (SV *)(((verts >> 24) << 3) + (int)v);
    f = (SV *)(((verts >> 16 & 0xFF) << 3) + (int)v);
    g = (SV *)(((verts >> 24 & 0xFF) << 3) + (int)v);
    step[0] = (c->x - (short)x) >> 2;
    step[1] = (c->y - (short)y) >> 2;
    step[2] = (c->z - (short)z) >> 2;
    estep[0] = (e->x - (short)x) >> 2;
    estep[1] = (e->y - (short)y) >> 2;
    estep[2] = (e->z - (short)z) >> 2;
    cstep[0] = (f->x - g->x) >> 2;
    dstep[0] = (cstep[0] - step[0]) >> 2;
    cstep[1] = (f->y - g->y) >> 2;
    dstep[1] = (cstep[1] - step[1]) >> 2;
    cstep[2] = (f->z - g->z) >> 2;
    dstep[2] = (cstep[2] - step[2]) >> 2;
    duv3[0] = (duv2[0] - duv[0]) >> 2;
    duv3[1] = (duv2[1] - duv[1]) >> 2;
    bp = D_00089030;
    pp = D_000A1B00;
    for (k = i = 0; i < 5; i++) {
        rx = x;
        ry = y;
        rz = z;
        ru = u;
        rw = w;
        bits = *bp++;
        for (j = 0; j < 5; j++) {
            int n = bits & 0x1F;
            SV *p = &D_000A1B00[n];
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
        step[0] += dstep[0];
        step[1] += dstep[1];
        step[2] += dstep[2];
        duv[0] += duv3[0];
        duv[1] += duv3[1];
        if (i != 0) {
            if (i >= 2) {
                B85_ST3(D_0009693C + k);
                pp += 3;
                k += 3;
            }
            B85_LDV3(pp);
        }
    }
    B85_ST3(D_0009693C + k);
    for (; k < 24; k += 3) {
        B85_LDV3(&D_000A1B00[k]);
        B85_ST3(D_0009693C + k);
    }
    B85_LDV1(&D_000A1B00[k]);
    B85_ST1(D_0009693C + k);
}
