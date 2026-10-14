typedef struct { short x, y, z, pad; } SV;
extern SV D_000A1B00[];
extern unsigned int D_00089044[];
extern int *D_0009693C;
extern short *D_00096944;

#define B35_LDV3(p) __asm__ volatile ( \
    "lwc2 $0, 0(%0);" "lwc2 $1, 4(%0);" "lwc2 $2, 8(%0);" "lwc2 $3, 12(%0);" \
    "lwc2 $4, 16(%0);" "lwc2 $5, 20(%0);" "nop;" "nop;" "rtpt" : : "r"(p))
#define B35_LDV1(p) __asm__ volatile ( \
    "lwc2 $0, 0(%0);" "lwc2 $1, 4(%0);" "nop;" "nop;" "rtps" : : "r"(p))
#define B35_ST3(p) __asm__ volatile ( \
    "swc2 $12, 0(%0);" "swc2 $13, 4(%0);" "swc2 $14, 8(%0)" : : "r"(p) : "memory")
#define B35_STSZ(p) __asm__ volatile ( \
    "swc2 $17, 0(%0);" "swc2 $18, 4(%0);" "swc2 $19, 8(%0)" : : "r"(p) : "memory")
#define B35_ST1(p) __asm__ volatile ("swc2 $14, 0(%0)" : : "r"(p) : "memory")

void func_00027620(unsigned int a0, SV *a1)
{
    short step[3];
    char pad[8];
    int sz[3];
    int i, j, k;
    int bits;
    short x;
    unsigned short y, z;
    SV *s;
    SV *e;
    SV *p;
    SV *q;

    for (i = 0; i < 2; i++) {
        s = &a1[a0 & 0xFF];
        e = &a1[a0 >> 8 & 0xFF];
        bits = D_00089044[i];
        x = s->x;
        y = s->y;
        z = s->z;
        step[0] = (e->x - s->x) >> 2;
        step[1] = (e->y - s->y) >> 2;
        step[2] = (e->z - s->z) >> 2;
        for (j = 0; j < 5; j++) {
            p = &D_000A1B00[bits & 0x1F];
            bits >>= 5;
            p->x = x;
            p->y = y;
            p->z = z;
            x += step[0];
            y += step[1];
            z += step[2];
        }
        a0 >>= 16;
    }
    q = D_000A1B00;
    for (k = 0; k < 9; k += 3) {
        B35_LDV3(q);
        B35_ST3(D_0009693C + k);
        B35_STSZ(sz);
        D_00096944[k] = sz[0];
        D_00096944[k + 1] = sz[1];
        D_00096944[k + 2] = sz[2];
        q += 3;
    }
    B35_LDV1(&D_000A1B00[k]);
    B35_ST1(D_0009693C + k);
}
