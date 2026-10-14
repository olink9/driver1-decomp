struct A { char pad[0xC8]; int c8, cc, d0; char pad1[0xF8-0xD4]; int f8; char pad2[0x114-0xFC]; int w[6]; char pad3[3]; char f12f; char pad4[0x17C-0x130]; short s17c; };
struct B { int a, b, c, d, e; short h[10]; unsigned short h28; };
extern long long D_00097708, D_00097118;
extern char D_0009B0A4[];
struct C { char pad[0x98]; int f98, f9c; char pad2[0x2A0-0xA0]; int f2a0; };
extern struct C D_000C6F04;
void func_000682C0(struct A *a, struct B *b)
{
    int *p;
    short *q;
    unsigned short t;
    a->c8 = b->a; a->cc = b->b; a->d0 = b->c; a->f8 = b->d;
    p = &a->w[0];
    q = (short *)((char *)b + 0x1C);
    while (p < &a->w[6]) { *p = *q; p++; q++; }
    t = b->h28;
    a->f12f = 1;
    a->s17c = t;
    if ((char *)a == D_0009B0A4) *(int *)&D_00097708 = b->e;
    *(int *)&D_00097118 = D_000C6F04.f98;
    D_000C6F04.f2a0 = D_000C6F04.f9c;
}
