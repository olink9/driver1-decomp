typedef struct { short a, b; } P;
typedef struct { int v; } __attribute__((packed)) U;

extern long long D_00096898;
extern int D_00097754, D_00097750, D_00097728, D_00097764, D_00097708;
extern int D_00097734, D_00097730, D_00097704, D_000976F8;
extern int D_0009770C, D_00097744, D_00097738, D_0009773C, D_00097748, D_0009774C;
extern int D_00097724, D_00097760;
extern long long D_00096E5C;
extern long long D_00096FC8;
extern int D_000C4C6C[];
extern int D_000C4E38[];
extern short D_000C4C7C[];
extern short D_00097758;
extern P D_000988C8[];
extern unsigned short D_0008BB6C[];
extern unsigned short D_0008BB84[];
extern U D_0008BBC8[];

static inline void fill(P *dst, unsigned short *src, int n)
{
    P *d;
    for (d = dst; d < dst + n; d++) {
        d->a = 0;
        d->b = *src++;
    }
}

void func_00063C60(void)
{
    int i;
    int k;
    int *q2;
    int *q;
    short *s;
    P *d;
    P *base;
    unsigned short *src;
    U *a, *b, *e;
    int v = *(unsigned char *)&D_00096898;
    short *z;
    short *w;

    q = D_000C4E38;
    D_00097754 = 0x1F;
    D_00097750 = 0x1F;
    D_00097728 = -1;
    D_00097764 = 0;
    D_00097708 = 0;
    D_00097734 = 0;
    D_00097730 = 0;
    *(int *)&D_00096FC8 = 0;
    D_00097704 = 0;
    if (v != 3 && v != 6)
        D_000976F8 = 1;
    else
        D_000976F8 = 0;
    D_000C4C6C[0] = 0;
    D_000C4C6C[1] = 0;
    D_000C4C6C[2] = 0;
    D_0009770C = 0;
    D_00097744 = 0x101;
    D_00097738 = 0x7FFFFFFF;
    D_0009773C = 0x7FFFFFFF;
    D_00097748 = 0;
    D_0009774C = 0;
    *(char *)&D_00096E5C = 0;
    i = 0;
    q2 = q + 0x1E4;
    do {
        q[0] = i;
        i++;
        q2[-1] = 0;
        q2[0] = 0;
        q2 += 0x1EA;
        q += 0x1EA;
    } while (i < 4);
    D_00097724 = 0;
    w = D_000C4C7C;
    s = w + 16;
    for (k = 0; s < w + 20; s++, k += 0x333)
        *s = k;
    *s = 0x7FFF;
    D_00097760 = 1;
    z = &D_00097758;
    z[0] = 0xFA0;
    z[1] = 0x30D4;
    z[2] = 0x200;
    base = D_000988C8;
    fill(base, D_0008BB6C, 11);
    fill(base + 11, D_0008BB84, 11);
    e = D_0008BBC8;
    for (a = e - 11, b = (U *)(base + 22); a < e; a++, b++)
        *b = *a;
}
