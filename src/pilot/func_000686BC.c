/* func_000686BC: func_00068928(start, end) takes two args */
typedef struct { short a, b; } E;
extern E D_0008C1E8[];
extern int D_000C6F04[];
extern int D_0009778C;
extern int D_000977A0;
extern int D_0001101C[];
extern int D_000C71B8[];
extern long long D_00096899;
extern long long D_000967DC;
extern int func_00050EC4();
extern void func_00068928();

void func_000686BC(short a)
{
    char pad[0x80];
    int v[2];
    E *p;
    int r, e, b;
    int *q = D_0001101C;

    D_000C6F04[0x40] = a;
    D_000C6F04[0xAE] = D_0009778C;
    p = D_0008C1E8;
    while (p->a < a && p->a != 0x7FFF) p++;
    func_00050EC4(q, v, (short)(p->b - (p->a - a)) * 4, 8);
    r = func_00050EC4(q, D_0009778C, v[0], v[1] - v[0]);
    b = D_0009778C; D_000C71B8[0] = e = b + r;
    if (*(unsigned char *)&D_00096899 == 0) *(char *)&D_000967DC = 0;
    func_00068928(b, e);
    D_000977A0 = (a + 3) & 7;
}
